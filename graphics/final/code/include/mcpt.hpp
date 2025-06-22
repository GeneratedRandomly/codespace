#ifndef NEE_HPP
#define NEE_HPP
#include <vecmath.h>
#include "ray.hpp"
#include "group.hpp"
#include "scene_parser.hpp"
#include "light.hpp"
#include "ptutil.hpp"

constexpr bool MIS = false;        // default false, only used when NEE is true
constexpr bool FRENSEL = true;     // default true
constexpr bool COSINE = true;      // default true
constexpr bool NEE = true;         // default true
constexpr bool GLOSSY_MIS = true; // default true

Vector3f mcpt(const Ray &ray, Group *group, SceneParser &sp, int depth)
{
    bool toodeep = depth >= 5;
    float p_RR = toodeep ? 0.9f : 1.0f;
    if (toodeep)
    {
        float random = getRan();
        if (random > p_RR)
            return Vector3f::ZERO; // drop
    }

    Hit hit;
    if (!group->intersect(ray, hit, eps))
        return sp.getBackgroundColor() / p_RR;

    MonteMaterial *mat = (MonteMaterial *)hit.getMaterial();
    Vector3f objpoint = ray.at(hit.getT());
    Vector3f coefficient = mat->getAlbedo() / p_RR;

    Vector3f lightColor(0, 0, 0);
    bool hitLight = false;
    int numLights = sp.getNumLights();
    for (size_t i = 0; i < numLights; i++)
    {
        CircleLight *circleLight = (CircleLight *)(sp.getLight(i));
        if (circleLight->intersect(ray, hit, eps))
        {
            float t = hit.getT();
            float t_group = group->intersect(ray, hit, eps) ? hit.getT() : 1e38f;
            if ((t < t_group) && (hitLight = true)) // 没有少打等号
                lightColor += circleLight->getColor() * coefficient;
        }
    }
    if (hitLight)
        return lightColor;

    Vector3f normal = hit.getNormal();
    Vector3f type = mat->getType();

    if (mat->isGlossy())
    {
        float light_pdf = 0.0f;
        Vector3f light_directly = getDirectIllumi(sp, objpoint, group, hit, coefficient, normal, light_pdf);
        Vector3f dir;

        float brdf_pdf = 0.0f;
        Vector3f brdf = glossy_brdf(mat, ray, normal, mat->getAlbedo(), dir, brdf_pdf) / p_RR;
        Vector3f light_indirect = brdf * mcpt(Ray(objpoint + eps * dir, dir), group, sp, depth + 1);

        float a = light_pdf / (brdf_pdf + light_pdf);

        return GLOSSY_MIS ? a * light_directly + (1 - a) * light_indirect : light_indirect;
    }
    else
    {
        float x = type.x(), y = type.y();
        float random = getRan();
        if (random < x) // 漫反射
        {
            float light_pdf = 0.0f;
            Vector3f light_directly = getDirectIllumi(sp, objpoint, group, hit, coefficient, normal, light_pdf);
            light_directly = NEE ? light_directly : 0;

            Vector3f dir = COSINE ? cosSample(normal) : sample(normal);
            float cos_theta = Vector3f::dot(normal, dir);
            float brdf_pdf = COSINE ? cos_theta / M_PI : 1.0f / (2 * M_PI);
            Ray diffuseRay = Ray(objpoint + eps * dir, dir);

            Vector3f L_i = mcpt(diffuseRay, group, sp, depth + 1);
            Vector3f light_indirect = COSINE ? L_i * coefficient : 2 * L_i * coefficient * cos_theta;

            float a = light_pdf / (brdf_pdf + light_pdf);

            if (NEE)
            {
                int numLights = sp.getNumLights();
                for (size_t i = 0; i < numLights; i++)
                {
                    CircleLight *circleLight = (CircleLight *)(sp.getLight(i));
                    if (circleLight->intersect(diffuseRay, hit, eps))
                    {
                        float t = hit.getT();
                        float t_group = group->intersect(diffuseRay, hit, eps) ? hit.getT() : 1e38f;
                        if (t < t_group)
                            return MIS && NEE ? a * light_directly + (1 - a) * light_indirect : light_directly;
                    }
                }
            }

            return MIS && NEE ? a * light_directly + (1 - a) * light_indirect : light_directly + light_indirect;
        }
        else if (random < x + y)
        {
            Vector3f N = normal;
            Vector3f I = ray.getDirection();
            Vector3f reflDir = I - 2 * Vector3f::dot(I, N) * N;
            Ray reflectRay = Ray(objpoint + eps * reflDir, reflDir);
            return coefficient * mcpt(reflectRay, group, sp, depth + 1);
        }
        else
        {
            float n_i = 1.0f, n_t = mat->getRefr();
            Vector3f I = ray.getDirection();
            Vector3f N = normal;
            float cosi = Vector3f::dot(-I, N);
            if (bool outward = cosi < 0)
            {
                std::swap(n_i, n_t);
                N = -N;
                cosi = -cosi;
            }
            float k = n_i / n_t;
            float sin2t = k * k * (1.0f - cosi * cosi);

            if (sin2t > 1.0f) // 全反射
            {
                Vector3f reflDir = I + 2 * cosi * N;
                Ray reflectRay = Ray(objpoint + eps * reflDir, reflDir);
                return coefficient * mcpt(reflectRay, group, sp, depth + 1);
            }
            else
            {
                float a = ((n_i - n_t) / (n_i + n_t));
                float R0 = a * a;
                float b = 1.0f - cosi;
                float R = R0 + (1 - R0) * b * b * b * b * b;

                float cost = sqrt(1.0f - sin2t);
                Vector3f reflDir = I + 2 * cosi * N;
                Ray reflectRay = Ray(objpoint + eps * reflDir, reflDir);
                Vector3f refrDir = k * I + (k * cosi - cost) * N;
                Ray refractRay = Ray(objpoint + eps * refrDir, refrDir);
                bool goRefl = getRan() < R;
                if (goRefl && FRENSEL)
                    return coefficient * mcpt(reflectRay, group, sp, depth + 1);
                else
                    return coefficient * mcpt(refractRay, group, sp, depth + 1);
            }
        }
    }
    return Vector3f::ZERO; // not implemented
}

#endif // NEE_HPP