// 继承自PA1
#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// INFO: Plane class, representing a plane in 3D space

class Plane : public Object3D
{
public:
    Plane() {}

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m), normal(normal.normalized()), d(d) {}

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override
    {
        Vector3f r0 = r.getOrigin();
        Vector3f rd = r.getDirection();
        float denominator = Vector3f::dot(normal, rd); // 计算分母
        if (denominator == 0)                          // 光线与平面平行
            return false;
        float t = (d - Vector3f::dot(normal, r0)) / denominator; // 计算交点
        if (t < tmin || t >= h.getT())                           // 交点在有效范围内
            return false;
        h.set(t, material, normal);
        return true;
    }

protected:
    // function: ax + by + cz - d = 0
    Vector3f normal = Vector3f::ZERO;
    float d = 0.0f;
};

#endif // PLANE_H
