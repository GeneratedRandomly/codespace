#ifndef UTIL_HPP
#define UTIL_HPP

#include <random>
using std::max;
using std::min;

// 独立实现
constexpr float eps = 1e-3;
// 随机数生成器来自于copilot推荐
static thread_local std::mt19937 gen(std::random_device{}());
static thread_local std::uniform_real_distribution<float> dist(0.0f, 1.0f);
inline float getRan() { return dist(gen); }
// 伽马校正系独立实现
inline void gammaCorrect(Vector3f &color, float gamma = 2.2f)
{
    float invGamma = 1.0f / gamma;
    color[0] = powf(color[0], invGamma);
    color[1] = powf(color[1], invGamma);
    color[2] = powf(color[2], invGamma);
}
// 以下colorMapping、celShading、saturationMapping、unbiasedMapping、tieDyeMapping函数的
// 亮度处理相关公式来自于风格化渲染论文，代码是自主实现
inline void colorMapping(Vector3f &color)
{
    float u = (color[0] + color[1] + color[2]) / 3.0f;
    u = std::max(0.0f, std::min(1.0f, u));

    Vector3f palette[] = {
        Vector3f(0.2f, 0.4f, 1.0f), // 蓝
        Vector3f(0.2f, 1.0f, 1.0f), // 青
        Vector3f(0.2f, 1.0f, 0.2f), // 绿
        Vector3f(1.0f, 1.0f, 0.2f), // 黄
        Vector3f(1.0f, 0.3f, 0.2f)  // 红
    };
    const int n = sizeof(palette) / sizeof(Vector3f);

    float pos = u * (n - 1);
    int idx = pos;
    float t = pos - idx;

    if (idx >= n - 1)
        color = palette[n - 1];
    else
        color = palette[idx] * (1.0f - t) + palette[idx + 1] * t;
}

inline void celShading(Vector3f &color)
{
    float u = (color[0] + color[1] + color[2]) / 3.0f;
    float mu = u < 0.75f ? 0.4 : 0.95;
    color = color * mu / u;
}

inline void saturationMapping(Vector3f &color, float w = 0.1f)
{
    float u = (color[0] + color[1] + color[2]) / 3.0f;

    Vector3f Imin(u, u, u);

    float cmin = std::min(color[0], std::min(color[1], color[2]));
    float cmax = std::max(color[0], std::max(color[1], color[2]));

    Vector3f Imax = (color - cmin) / cmax;

    float up = (Imax[0] + Imax[1] + Imax[2]) / 3.0f;
    if (up < 1e-6f)
    {
        color = w * Imin + (1.0f - w) * color;
        return;
    }

    Imax = Imax * (u / up);

    color = w * Imin + (1.0f - w) * Imax;
}

inline void unbiasedMapping(Vector3f &color)
{
    float c = 1.5f;
    float p = 0.5f;
    color = c * (color - p) + p;
}

inline void tieDyeMapping(Vector3f &color)
{
    for (int i = 0; i < 3; ++i)
        color[i] = std::min(4.0f, std::max(-1.0f, color[i]));

    color[0] = (-cosf(2.0f * M_PI * (color[0] + 1.0f)) + 1.0f) * 0.5f;
    color[1] = (-cosf(2.15f * M_PI * (color[1] + 1.13f)) + 1.0f) * 0.5f;
    color[2] = (-cosf(2.3f * M_PI * (color[2] + 1.29f)) + 1.0f) * 0.5f;
}
#endif // UTIL_HPP