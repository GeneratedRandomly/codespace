#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

class Plane : public Object3D
{
public:
    Plane() {}

    Plane(const Vector3f &norm, float d, Material *m) : Object3D(m), norm(norm), d(d) {}

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override
    {
        Vector3f r0 = r.getOrigin();
        Vector3f rd = r.getDirection();
        float denominator = Vector3f::dot(norm, rd); // 计算分母
        if (denominator == 0)                        // 光线与平面平行
            return false;
        float t = (d - Vector3f::dot(norm, r0)) / denominator; // 计算交点
        if (t < tmin || t >= h.getT())                         // 交点在有效范围内
            return false;
        h.set(t, material, norm);
        return true;
    }

    void drawGL() override
    {
        Object3D::drawGL();
        Vector3f xAxis = Vector3f::RIGHT;
        Vector3f yAxis = Vector3f::cross(norm, xAxis);
        xAxis = Vector3f::cross(yAxis, norm);
        const float planeSize = 10.0;
        glBegin(GL_TRIANGLES);
        glNormal3fv(norm);
        glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        glVertex3fv(d * norm + planeSize * xAxis - planeSize * yAxis);
        glNormal3fv(norm);
        glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        glEnd();
    }

protected:
    Vector3f norm;
    float d;
};

#endif // PLANE_H
