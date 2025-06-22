// DirectionalLight和PointLight继承自PA1，CircleLight系自主实现。
#ifndef LIGHT_H
#define LIGHT_H

#include <Vector3f.h>
#include "util.hpp"
#include "hit.hpp"

class Light
{
public:
    Light() = default;

    virtual ~Light() = default;

    /// @brief get the direction to the light source
    /// @param p input: object point
    /// @param dir output: direction to the light source
    /// @param col output: color of light
    virtual void getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col) const = 0;
};

class DirectionalLight : public Light
{
public:
    DirectionalLight() = delete;

    DirectionalLight(const Vector3f &d, const Vector3f &c)
    {
        direction = d.normalized();
        color = c;
    }

    ~DirectionalLight() override = default;

    void getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col) const override
    {
        // the direction to the light is the opposite of the
        // direction of the directional light source
        dir = -direction;
        col = color;
    }

private:
    Vector3f direction;
    Vector3f color;
};

class PointLight : public Light
{
public:
    PointLight() = delete;

    PointLight(const Vector3f &p, const Vector3f &c)
    {
        position = p;
        color = c;
    }

    ~PointLight() override = default;

    void getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col) const override
    {
        // the direction to the light is the opposite of the
        // direction of the directional light source
        dir = (position - p);
        dir = dir / dir.length();
        col = color;
    }

    Vector3f getPosition() const { return position; }

private:
    Vector3f position;
    Vector3f color;
};

class CircleLight : public Light
{
private:
    Vector3f center; // 圆心
    Vector3f normal; // 法向量
    float radius;    // 半径
    Vector3f color;  // 光源颜色
    float area;      // 圆面积
public:
    CircleLight() = delete;

    CircleLight(const Vector3f &c, const Vector3f &n, float r, const Vector3f &col)
        : center(c), normal(n.normalized()), radius(r), color(col) { area = M_PI * radius * radius; }

    ~CircleLight() override = default;

    void getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col) const override {}

    bool intersect(const Ray &ray, Hit &hit, float tmin) const // 判断射线是否与面光源相交
    {
        // 计算光源平面方程
        Vector3f ori = ray.getOrigin();
        Vector3f dir = ray.getDirection();
        float t = Vector3f::dot(normal, center - ori) / Vector3f::dot(normal, dir);
        if (t < tmin)
            return false;

        Vector3f intersection = ray.at(t);
        Vector3f toCenter = intersection - center;
        if (toCenter.squaredLength() > radius * radius)
            return false;

        hit.set(t, nullptr, normal); // 设置hit信息
        return true;
    }

    Vector3f getColor() const { return color; }

    Vector3f getSamplePoint() // NEE 对光源均匀采样
    {
        float x = getRan();
        float y = getRan();
        float r = radius * sqrt(x);
        float t = 2 * M_PI * y;
        Vector3f temp = fabsf(normal.x()) > 0.1f ? Vector3f(0, 1, 0) : Vector3f(1, 0, 0);
        Vector3f u = Vector3f::cross(temp, normal).normalized();
        Vector3f v = Vector3f::cross(normal, u);
        Vector3f point = center + r * (u * cos(t) + v * sin(t));
        return point;
    }

    Vector3f getNormal() const { return normal; }

    float getArea() const { return area; }
};

#endif // LIGHT_H
