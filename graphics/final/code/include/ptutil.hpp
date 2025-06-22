// 本文档中，列到的数学公式和beckmann、GGX重要性采样（sampleGGXAniso等）是PPT上（beckmann）或网上（GGX）的，其他全部为自主实现。
#ifndef PTUTIL_HPP
#define PTUTIL_HPP

#include "scene_parser.hpp"
#include "group.hpp"
#include "light.hpp"

enum class SampleType
{
    UNIFORM,
    BECKMANN,
    GGX,
};
enum class BRDFType
{
    COOK_TORRANCE,
    GGX,
};

const Vector3f F0_GOLD = Vector3f(1.000, 0.782, 0.344);   // 金属的菲涅尔反射率
const Vector3f F0_SILVER = Vector3f(0.972, 0.960, 0.915); // 银的菲涅尔反射率
const Vector3f F0_COPPER = Vector3f(0.955, 0.638, 0.538); // 铜的菲涅尔反射率
const Vector3f F0_IRON = Vector3f(0.562, 0.565, 0.578);
constexpr bool GLOSSY_ISO = false; // default true, 是否使用等向 BRDF
SampleType sampleType = SampleType::GGX;
BRDFType brdfType = BRDFType::GGX;

inline Vector3f rot_to_normal(const float &x, const float &y, const float &z, const Vector3f &normal)
{
    // 构造局部坐标系
    Vector3f w = normal;
    Vector3f temp = fabsf(w.x()) > 0.1f ? Vector3f(0, 1, 0) : Vector3f(1, 0, 0);
    Vector3f u = Vector3f::cross(temp, w).normalized();
    Vector3f v = Vector3f::cross(w, u);

    return (u * x + v * y + w * z).normalized();
}

// 均匀随机采样
inline Vector3f sample(const Vector3f &normal)
{
    float u1 = getRan();
    float u2 = getRan();
    float r = sqrt(1.0f - u1 * u1);
    float theta = 2.0f * M_PI * u2;
    float x = r * cos(theta);
    float y = r * sin(theta);
    float z = u1;

    return rot_to_normal(x, y, z, normal);
}

// 生成余弦加权半球采样，Lambertian BRDF
inline Vector3f cosSample(const Vector3f &normal)
{
    float r1 = getRan() * 2 * M_PI; // [0, 2pi)
    float r2 = getRan();            // [0, 1)
    float r2s = sqrt(r2);           // [0, 1)

    float x = cos(r1) * r2s;
    float y = sin(r1) * r2s;
    float z = sqrt(1 - r2);

    return rot_to_normal(x, y, z, normal);
}

// 采样 Beckmann 半程向量
inline Vector3f sampleBeckmann(const Vector3f &normal, float roughness)
{
    float phi = 2 * M_PI * getRan();
    float tan2Theta = -roughness * roughness * log(getRan());
    float cosTheta = 1 / sqrt(1 + tan2Theta);
    float sinTheta = sqrt(max(0.0f, 1 - cosTheta * cosTheta));

    float x = sinTheta * cos(phi);
    float y = sinTheta * sin(phi);
    float z = cosTheta;

    return rot_to_normal(x, y, z, normal);
}

inline float beckmann_D(float ndoth, float roughness)
{
    float cosTheta2 = ndoth * ndoth;
    float tanTheta2 = (1 - cosTheta2) / (cosTheta2);
    float m2 = roughness * roughness;
    float denom = M_PI * m2 * cosTheta2 * cosTheta2;
    return exp(-tanTheta2 / m2) / denom;
}

inline float beckmann_G(float ndoth, float ndotv, float ndoti, float vdoth)
{
    return min(1.0f, 2 * min(ndoth * ndotv / vdoth, ndoth * ndoti / vdoth)); // 几何遮蔽函数
}

inline float beckmann_pdf(float ndoth, float D)
{
    return D * ndoth; // (4.0f * vdoth);
}

inline Vector3f sampleGGX(const Vector3f &normal, float roughness)
{
    float u = getRan();
    float phi = 2.0f * M_PI * getRan();
    float cosTheta = sqrt((1.0f - u) / (1.0f + (roughness * roughness - 1.0f) * u));
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

    float x = sinTheta * cos(phi);
    float y = sinTheta * sin(phi);
    float z = cosTheta;

    return rot_to_normal(x, y, z, normal);
}

inline float ggx_D(float ndoth, float roughness)
{
    float a2 = roughness * roughness;
    // float a2 = a * a;
    float ndoth2 = ndoth * ndoth;
    float b = ndoth2 * (a2 - 1) + 1;
    float denom = M_PI * b * b;
    return a2 / denom;
}

inline float ggx_G(float ndotv, float ndoti, float roughness)
{
    // 直接光照：k = (m + 1) ^ 2 / 8
    // 基于图像的光照：k = m ^ 2 / 2
    float k = (roughness + 1) * (roughness + 1) / 8.0f;
    float g1v = ndotv / (ndotv * (1 - k) + k);
    float g1i = ndoti / (ndoti * (1 - k) + k);
    return g1v * g1i;
}

inline float ggx_pdf(float ndoth, float D)
{
    return D * ndoth;
}

inline Vector3f sampleGGXAniso(const Vector3f &N,
                               const Vector3f &T,
                               const Vector3f &B,
                               float ax,
                               float ay)
{
    float u1 = getRan();
    float u2 = getRan();

    float phi = atan(ay / ax * tan(2.0f * M_PI * u1));
    if (u1 > 0.5f)
        phi += M_PI;
    float sinPhi = sin(phi);
    float cosPhi = cos(phi);

    float ax2 = ax * ax;
    float ay2 = ay * ay;

    float denom = cosPhi * cosPhi / ax2 + sinPhi * sinPhi / ay2;
    float tanTheta2 = u2 / ((1.0f - u2) * denom);
    float cosTheta = 1.0f / sqrt(1.0f + tanTheta2);
    float sinTheta = sqrt(max(0.0f, 1.0f - cosTheta * cosTheta));

    Vector3f H = (T * cosPhi + B * sinPhi) * sinTheta + N * cosTheta;
    return H.normalized();
}

inline float ggx_aniso_D(const Vector3f &H,
                         const float &ndoth,
                         const Vector3f &T,
                         const Vector3f &B,
                         float ax,
                         float ay)
{
    float hx = Vector3f::dot(H, T) / ax;
    float hy = Vector3f::dot(H, B) / ay;
    float hz = ndoth;
    float h = hx * hx + hy * hy + hz * hz;
    float denom = M_PI * ax * ay * h * h;
    return 1.0f / denom;
}

inline float smithG1_aniso(const Vector3f &v,
                           const Vector3f &N,
                           const Vector3f &T,
                           const Vector3f &B,
                           float ax,
                           float ay)
{
    float ndotv = max(0.01f, Vector3f::dot(N, v));
    float t = Vector3f::dot(T, v) / ax;
    float b = Vector3f::dot(B, v) / ay;
    float lambda = -1.0f + std::sqrt(1.0f + t * t + b * b) / ndotv;
    return 1.0f / (1.0f + lambda);
}

inline float ggx_aniso_G(const Vector3f &V,
                         const Vector3f &L,
                         const Vector3f &T,
                         const Vector3f &B,
                         const Vector3f &N,
                         float ax,
                         float ay)
{
    float g1_v = smithG1_aniso(V, N, T, B, ax, ay);
    float g1_l = smithG1_aniso(L, N, T, B, ax, ay);
    return g1_v * g1_l;
}

inline float ggx_aniso_pdf(const float &D, const float &ndoth)
{
    return D * ndoth;
}

inline void buildSphereTangentBasis(const Vector3f &N, Vector3f &X, Vector3f &Y)
{
    Vector3f up = Vector3f(0.2f, 0.4f, 0.6f).normalized();
    if (Vector3f::dot(N, up) > 0.999f)       // N is close to up
        up = Vector3f(1.0f, 0.0f, 0.0f);     // use a different up vector
    X = Vector3f::cross(N, up).normalized(); // X is orthogonal
    Y = Vector3f::cross(N, X).normalized();  // Y is orthogonal
}

inline float F_schlick(float refr, float vdoth)
{
    float r0 = (1 - refr) / (1 + refr);
    r0 *= r0;
    return r0 + (1 - r0) * vdoth * vdoth * vdoth * vdoth * vdoth;
}

inline Vector3f iso_brdf(const Vector3f &N,
                         const Vector3f &I,
                         const Vector3f &Kd,
                         const Vector3f &coefficient,
                         const float &r,
                         Vector3f &dir,
                         float &pdf)
{
    Vector3f H;
    switch (sampleType)
    {
    case SampleType::UNIFORM:
        H = sample(N);
        break;
    case SampleType::BECKMANN:
        H = sampleBeckmann(N, r);
        break;
    case SampleType::GGX:
        H = sampleGGX(N, r);
        break;
    default:
        break;
    }

    Vector3f V = 2 * Vector3f::dot(I, H) * H - I;

    float ndoth = max(0.01f, Vector3f::dot(N, H));
    float vdoth = max(0.01f, Vector3f::dot(V, H));
    float ndotv = max(0.01f, Vector3f::dot(N, V));
    float ndoti = max(0.01f, Vector3f::dot(N, I));

    Vector3f F0 = F0_GOLD;
    float b = 1 - vdoth; // == hdoti
    Vector3f F = F0 + (1 - F0) * b * b * b * b * b;

    float D, G;
    switch (brdfType)
    {
    case BRDFType::COOK_TORRANCE:
        D = beckmann_D(ndoth, r);
        G = beckmann_G(ndoth, ndotv, ndoti, vdoth);
        break;
    case BRDFType::GGX:
        D = ggx_D(ndoth, r);
        G = ggx_G(ndotv, ndoti, r);
        break;
    default:
        break;
    }

    switch (sampleType)
    {
    case SampleType::UNIFORM:
        pdf = 1.0f / (2.0f * M_PI);
        break;
    case SampleType::BECKMANN:
        pdf = beckmann_pdf(ndoth, D);
        break;
    case SampleType::GGX:
        pdf = ggx_pdf(ndoth, D);
        break;
    default:
        break;
    }

    Vector3f brdf = Kd * coefficient / M_PI + F * D * G / (4 * ndotv * ndoti);

    dir = V;
    return brdf * ndotv / pdf;
}

inline Vector3f aniso_brdf(const Vector3f &N,
                           const Vector3f &I,
                           const Vector3f &Kd,
                           const Vector3f &coefficient,
                           const float &ax,
                           const float &ay,
                           Vector3f &dir,
                           float &pdf)
{
    Vector3f X, Y;
    buildSphereTangentBasis(N, X, Y);
    Vector3f H = sampleGGXAniso(N, X, Y, ax, ay);

    Vector3f V = 2 * Vector3f::dot(I, H) * H - I;

    float ndoth = Vector3f::dot(N, H);
    float vdoth = max(0.01f, Vector3f::dot(V, H));
    float ndotv = max(0.01f, Vector3f::dot(N, V));
    float ndoti = max(0.01f, Vector3f::dot(N, I));

    Vector3f F0 = F0_GOLD;
    float b = 1 - vdoth; // == hdoti
    Vector3f F = F0 + (1 - F0) * b * b * b * b * b;

    float D = ggx_aniso_D(H, ndoth, X, Y, ax, ay);
    float G = ggx_aniso_G(V, I, N, X, Y, ax, ay);
    pdf = ggx_aniso_pdf(D, ndoth);

    Vector3f brdf = Kd * coefficient / M_PI + F * D * G / (4 * ndotv * ndoti);

    dir = V;
    return brdf * ndotv / pdf;
}

inline Vector3f glossy_brdf(const MonteMaterial *mat,
                            const Ray &ray,
                            const Vector3f &normal,
                            const Vector3f &coefficient,
                            Vector3f &dir,
                            float &pdf)
{
    Vector3f Kd = mat->getKd();
    Vector3f Ks = mat->getKs();
    float r = max(0.001f, mat->getRoughness());
    float refr = mat->getRefr();
    Vector3f N = normal.normalized();
    Vector3f I = -ray.getDirection().normalized();

    return GLOSSY_ISO ? iso_brdf(N, I, Kd, coefficient, r, dir, pdf) : aniso_brdf(N, I, Kd, coefficient, 0.1, 0.9, dir, pdf);
}

inline Vector3f getDirectIllumi(const SceneParser &sp,
                                const Vector3f &objpoint,
                                Group *group,
                                Hit &hit,
                                const Vector3f &coefficient,
                                const Vector3f &normal,
                                float &pdf)
{
    Vector3f light_directly = Vector3f::ZERO;
    int numlights = sp.getNumLights();
    for (size_t i = 0; i < numlights; i++)
    {
        CircleLight *circleLight = (CircleLight *)(sp.getLight(i));

        Vector3f obj_to_light = circleLight->getSamplePoint() - objpoint;
        Vector3f otl = obj_to_light.normalized();
        float otl_squlen = obj_to_light.squaredLength();
        Ray light_ray(objpoint + eps * otl, otl);
        if (group->intersect(light_ray, hit, eps))
            if (eps < sqrt(otl_squlen) - hit.getT())
                continue;

        Vector3f color = circleLight->getColor();
        Vector3f light_normal = circleLight->getNormal();
        float cos1 = max(0.0f, Vector3f::dot(otl, normal));
        float cos2 = max(0.0f, -Vector3f::dot(otl, light_normal));
        light_directly += color * cos1 * cos2 * circleLight->getArea() / otl_squlen;
        pdf += circleLight->getArea() * cos2 / otl_squlen;
    }
    Vector3f brdf = coefficient / M_PI;
    return light_directly * brdf;
}

#endif