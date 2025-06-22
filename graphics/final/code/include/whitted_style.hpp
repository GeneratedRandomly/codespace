// 本文件中的反射和折射公式来自于课程PPT，其余大部分系自主完成。
#ifndef WHITTED_STYLE_H
#define WHITTED_STYLE_H

#include <vecmath.h>
#include "ray.hpp"
#include "group.hpp"
#include "scene_parser.hpp"
#include "light.hpp"
#include "util.hpp"

Vector3f whittedStyle(const Ray &ray, Group *group, SceneParser &sp, int depth) // Whitted-Style 光线追踪
{
    if (depth == 100)
        return sp.getBackgroundColor();

    Hit hit;
    if (!group->intersect(ray, hit, eps))
        return sp.getBackgroundColor();

    PhongMaterial *mat = (PhongMaterial *)hit.getMaterial();
    Vector3f color(0, 0, 0);

    float origin_t = hit.getT();
    Vector3f objpoint = ray.at(origin_t);
    Vector3f type = mat->getType();
    float x = type.x(), y = type.y(), z = type.z();

    if (x > y && x > z) // 漫反射，Phong着色+阴影
    {
        for (int li = 0; li < sp.getNumLights(); ++li)
        {
            Light *light = sp.getLight(li);
            Vector3f dirToLight, lightcolor;
            light->getIllumination(objpoint, dirToLight, lightcolor);
            // 阴影检测
            Ray shadowRay(objpoint + eps * dirToLight, dirToLight);
            Hit shadowHit;
            bool inShadow = group->intersect(shadowRay, shadowHit, eps);
            if (auto lightptr = dynamic_cast<PointLight *>(light))
                inShadow &= (shadowHit.getT() < (lightptr->getPosition() - objpoint).length());
            if (inShadow == false)
                color += mat->Shade(ray, hit, dirToLight, lightcolor);
        }
    }
    else if (y > x && y > z) // 完美反射
    {
        Vector3f N = hit.getNormal();
        Vector3f I = ray.getDirection().normalized();
        Vector3f R = I - 2 * Vector3f::dot(I, N) * N;
        Ray reflectRay = Ray(objpoint + eps * R, R);
        color = mat->getKs() * whittedStyle(reflectRay, group, sp, depth + 1);
    }
    else if (z > x && z > y) // 完美折射
    {
        float n_i = 1.0f, n_t = mat->getRefr();
        Vector3f I = ray.getDirection().normalized();
        Vector3f N = hit.getNormal().normalized();
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
            Vector3f R = I + 2 * cosi * N;
            Ray reflectRay(objpoint + eps * R, R);
            color = mat->getKs() * whittedStyle(reflectRay, group, sp, depth + 1);
        }
        else
        {
            float cost = sqrt(1.0f - sin2t);
            Vector3f T = k * I + (k * cosi - cost) * N;
            Ray refractRay(objpoint + eps * T, T);
            color = mat->getKr() * whittedStyle(refractRay, group, sp, depth + 1);
        }
    }
    return color;
}

#endif // WHITTED_STYLE_H