#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>

// INFO: 材质类，包含漫反射和镜面反射系数
class Material
{
    float clamp(float a) { return a < 0 ? 0 : a; }

public:
    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0)
        : diffuseColor(d_color), specularColor(s_color), shininess(s)
    {
    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const { return diffuseColor; }

    Vector3f Shade(const Ray &ray,
                   const Hit &hit,
                   const Vector3f &dirToLight,
                   const Vector3f &lightColor)
    {
        Vector3f N = hit.getNormal().normalized();
        Vector3f V = -ray.getDirection().normalized();
        Vector3f L = dirToLight.normalized();
        float L_by_N = Vector3f::dot(L, N);
        Vector3f R = (2 * L_by_N * N - L).normalized();
        Vector3f shaded =
            lightColor *
            (diffuseColor * clamp(L_by_N) +
             specularColor * (pow(clamp(Vector3f::dot(V, R)), shininess)));
        return shaded;
    }

protected:
    Vector3f diffuseColor;  // 漫反射系数
    Vector3f specularColor; // 镜面反射系数
    float shininess;        // 光泽度
};

#endif // MATERIAL_H
