// 修改了材料，新增了适合于蒙特卡洛光线追踪的材料MonteMaterial。
#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <iostream>
#include <vecmath.h>
#include "ray.hpp"
#include "hit.hpp"

inline float clamp(float a) { return a < 0 ? 0 : a; }

class Material
{
};

class PhongMaterial : public Material
{
    Vector3f Kd;   // 漫反射系数
    Vector3f Ks;   // 镜面反射系数
    float s;       // 光泽度/反射指数
    Vector3f Kr;   // 折射系数（以下独立实现）
    Vector3f type; // 材质类型
    float refr;    // 绝对折射率

public:
    PhongMaterial(const Vector3f &d_color,
                  const Vector3f &s_color,
                  float s,
                  Vector3f r_color,
                  Vector3f t,
                  float r)
        : Kd(d_color),
          Ks(s_color),
          s(s),
          Kr(r_color),
          type(t),
          refr(r) {}

    ~PhongMaterial() = default;

    Vector3f getKd() const { return Kd; }

    Vector3f getKs() const { return Ks; }

    Vector3f getKr() const { return Kr; }

    Vector3f getType() const { return type; }

    float getRefr() const { return refr; }

    Vector3f Shade(const Ray &ray,
                   const Hit &hit,
                   const Vector3f &dirToLight,
                   const Vector3f &lightColor)
    {
        Vector3f N = hit.getNormal().normalized();
        Vector3f V = -ray.getDirection().normalized();
        Vector3f L = dirToLight.normalized();
        Vector3f H = (L + V).normalized();
        Vector3f shaded =
            lightColor *
            (Kd * clamp(Vector3f::dot(L, N)) +
             Ks * (pow(clamp(Vector3f::dot(N, H)), s)));
        return shaded;
    }
};

class MonteMaterial : public Material
{
    Vector3f albedo;
    Vector3f type; // 材质类型
    float refr;    // 绝对折射率
    bool glossy;
    Vector3f Kd;
    Vector3f Ks;
    float roughness;

public:
    MonteMaterial(const Vector3f &albe,
                  const Vector3f &t,
                  float r,
                  bool g,
                  const Vector3f &d_color,
                  const Vector3f &s_color,
                  float rough)
        : albedo(albe),
          type(t),
          refr(r),
          glossy(g),
          Kd(d_color),
          Ks(s_color),
          roughness(rough) {}

    ~MonteMaterial() = default;

    Vector3f getAlbedo() const { return albedo; }

    Vector3f getType() const { return type; }

    float getRefr() const { return refr; }

    bool isGlossy() const { return glossy; }

    Vector3f getKd() const { return Kd; }

    Vector3f getKs() const { return Ks; }

    float getRoughness() const { return roughness; }
};

// End：细微修改自PA1
#endif // MATERIAL_H
