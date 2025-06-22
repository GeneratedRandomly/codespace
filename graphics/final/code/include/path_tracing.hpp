// OpenMP 部分是结合了https://www.cnblogs.com/Chang-LeHung/p/16874200.html和copilot的建议实现的。
#ifndef PATH_TRACING_H
#define PATH_TRACING_H

#include <vecmath.h>
#include "mcpt.hpp"

Vector3f pathTracing(const Ray &ray, Group *group, SceneParser &sp, int choice)
{
    int sample = 16;
    Vector3f color(0, 0, 0);
    // 在头文件或 path_tracing.hpp 顶部添加
#pragma omp declare reduction(+ : Vector3f : omp_out += omp_in) initializer(omp_priv = Vector3f(0, 0, 0))
#pragma omp parallel for reduction(+ : color)
    for (int i = 0; i < sample; ++i)
        color += mcpt(ray, group, sp, 0);
    color = color / sample;
    return color;
}
// End：独立实现，公式部分来自于课程PPT

#endif // PATH_TRACING_H