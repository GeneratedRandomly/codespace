// 包围盒参考自https://github.com/Guangxuan-Xiao/THU-Computer-Graphics-2020/blob/master/project/final/include/bound.hpp
// intersect函数是自己实现的，原作者的代码比较困难。
#ifndef BOUND_HPP
#define BOUND_HPP
#include <vecmath.h>
#include <cfloat>
#include "ray.hpp"

class Bound
{
    friend class Triangle_BVH;

private:
    Vector3f min;
    Vector3f max;

public:
    Bound() : min(Vector3f(FLT_MAX)), max(Vector3f(-FLT_MAX)) {};

    Bound(const Vector3f &_min, const Vector3f &_max)
        : min(_min), max(_max) {}

    ~Bound() = default;

    void set(const Vector3f &_min, const Vector3f &_max)
    {
        this->min = _min;
        this->max = _max;
    }

    void update(const Vector3f &point)
    {
        if (point[0] < min[0])
            min[0] = point[0];
        if (point[1] < min[1])
            min[1] = point[1];
        if (point[2] < min[2])
            min[2] = point[2];

        if (point[0] > max[0])
            max[0] = point[0];
        if (point[1] > max[1])
            max[1] = point[1];
        if (point[2] > max[2])
            max[2] = point[2];
    }

    bool intersect(const Ray &ray, float &t_min, float &t_max) const
    {
        t_min = -FLT_MAX;
        t_max = FLT_MAX;
        Vector3f origin = ray.getOrigin();
        Vector3f dir = ray.getDirection();
        for (int i = 0; i < 3; ++i)
        {
            if (fabs(dir[i]) < 1e-8)
            {
                if (origin[i] < min[i] || origin[i] > max[i])
                    return false;
                continue;
            }
            float invD = 1.0f / dir[i];
            float t0 = (min[i] - origin[i]) * invD;
            float t1 = (max[i] - origin[i]) * invD;
            if (t0 > t1)
                std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        return true;
    }
};

#endif // BOUND_HPP