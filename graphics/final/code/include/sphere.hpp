#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <glut.h>

class Sphere : public Object3D
{
public:
    Sphere() : center(Vector3f::ZERO), radius(1.0) {}

    Sphere(const Vector3f &center,
           float radius,
           Material *material)
        : Object3D(material),
          center(center),
          radius(radius) {}

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override
    {
        Vector3f origin = r.getOrigin();
        Vector3f direction = r.getDirection().normalized(); // 光线方向单位化
        Vector3f OC = center - origin;
        float OC_2 = OC.squaredLength();
        float CT_2 = radius * radius;
        float OH = Vector3f::dot(OC, direction);
        float TH = std::sqrt(CT_2 - OC_2 + OH * OH);

        float t = OH + TH;
        if (OC_2 > CT_2) // 光源在球体外部
            t = OH - TH;
        else if (OC_2 == CT_2 && OH <= 0)
            t = 0;

        if (t >= tmin && t < h.getT())
        {
            h.set(t, material, (direction * t - OC).normalized());
            return true;
        }
        return false;
    }

    void drawGL() override
    {
        Object3D::drawGL();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(center.x(), center.y(), center.z());
        glutSolidSphere(radius, 80, 80);
        glPopMatrix();
    }

protected:
    Vector3f center;
    float radius;
};

#endif
