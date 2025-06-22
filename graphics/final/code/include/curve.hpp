// 在PA2的基础上，将两个基函数上建立了共同的父类
// 改变了分段三次bezier曲线为一段任意次bezier曲线。
#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>

#include <algorithm>

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in all the data.
struct CurvePoint
{
    Vector3f V = Vector3f::ZERO; // Vertex
    Vector3f T = Vector3f::ZERO; // Tangent  (unit)
};

class Base;
class BezierCurve;
class BsplineCurve;
class BezierBase;
class BspBase;

class base
{
private:
public:
    base() = default;
    ~base() = default;
    virtual CurvePoint evaluate(float t) = 0; // Evaluate the curve at parameter t
};

class BspBase : public base
{
    int n;                    // number of control points - 1
    int k;                    // degree of the curve
    std::vector<float> knots; // knot vector, n+k+2 elements
    const std::vector<Vector3f> controls;

public:
    BspBase() = delete;
    BspBase(int n, int k, std::vector<float> knots, const std::vector<Vector3f> &controls)
        : n(n), k(k), knots(std::move(knots)), controls(controls)
    {
        for (int i = 0; i < k; i++) // padding
            knots.push_back(1.0);
    };
    ~BspBase() {};
    CurvePoint evaluate(float t) override
    {
        CurvePoint cp;
        std::vector<float> B(n + k + 1, 0.0);
        for (int i = 0; i < n + k + 1; i++)
            if (t >= knots[i] && t < knots[i + 1])
                B[i] = 1.0;
        for (int p = 1; p < k; p++)
            for (int i = 0; i < n + k - p + 1; i++)
                B[i] = (t - knots[i]) * B[i] / (knots[i + p] - knots[i]) +
                       (knots[i + p + 1] - t) * B[i + 1] / (knots[i + p + 1] - knots[i + 1]);
        for (int i = 0; i < n + 1; i++)
        {
            cp.V += ((t - knots[i]) * B[i] / (knots[i + k] - knots[i]) +
                     (knots[i + k + 1] - t) * B[i + 1] / (knots[i + k + 1] - knots[i + 1])) *
                    controls[i];
            cp.T += k *
                    (B[i] / (knots[i + k] - knots[i]) -
                     B[i + 1] / (knots[i + k + 1] - knots[i + 1])) *
                    controls[i];
        }
        return cp;
    }
    const std::vector<float> &getKnots() const { return knots; }
};

class BezierBase : public base
{
private:
    std::vector<Vector3f> controls;

public:
    BezierBase() = delete;
    BezierBase(std::vector<Vector3f> controls) : controls(std::move(controls)) {}
    ~BezierBase() {}
    CurvePoint evaluate(float t) override
    {
        CurvePoint cp;
        std::vector<Vector3f> B = controls;
        std::vector<Vector3f> dB;
        int n = controls.size();
        for (int i = 0; i < n - 1; i++)
            dB.push_back(n * (B[i + 1] - B[i]));
        for (int k = 1; k < n; k++)
            for (int i = 0; i < n - k; i++)
                B[i] = (1 - t) * B[i] + t * B[i + 1];
        for (int k = 1; k < n - 1; k++)
            for (int i = 0; i < n - k - 1; i++)
                dB[i] = (1 - t) * dB[i] + t * dB[i + 1];
        cp.V = B[0];
        cp.T = dB[0];
        return cp;
    }
};

class Curve : public Object3D
{
protected:
    std::vector<Vector3f> controls;

public:
    explicit Curve(std::vector<Vector3f> controls) : controls(std::move(controls)) {}

    bool intersect(const Ray &r, Hit &h, float tmin) override { return false; }

    std::vector<Vector3f> &getControls() { return controls; }

    virtual void discretize(int resolution, std::vector<CurvePoint> &data) = 0;

    virtual base *getBase() = 0;
};

class BezierCurve : public Curve
{
    BezierBase bernstein;

public:
    explicit BezierCurve(const std::vector<Vector3f> &ctrl_points)
        : Curve(ctrl_points), bernstein(ctrl_points)
    {
        assert(ctrl_points.size() > 2);
    }

    void discretize(int resolution, std::vector<CurvePoint> &data) override
    {
        data.clear();
        float step = 1.0f / resolution;
        for (int j = 0; j < resolution; j++)
            data.push_back(bernstein.evaluate(j * step));
        data.push_back(bernstein.evaluate(1.0f));
    }

    base *getBase() override { return &bernstein; }
};

class BsplineCurve : public Curve
{
    // 0 ... 1 均匀 n+k+2个
    int n = 0; // number of control points - 1
    int k = 0; // degree of the curve
    BspBase *b = nullptr;

public:
    BsplineCurve(const std::vector<Vector3f> &controls) : Curve(controls)
    {
        if (controls.size() < 4)
        {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }
        n = controls.size() - 1;
        k = 3;                     // 题目要求
        std::vector<float> points; // std::move
        for (int i = 0; i <= n + k + 1; i++)
            points.push_back(i / (float)(n + k + 1));
        b = new BspBase(n, k, points, controls);
    }

    ~BsplineCurve() { delete b; }

    void discretize(int resolution, std::vector<CurvePoint> &data) override
    {
        data.clear();
        auto knots = b->getKnots();
        for (int i = k; i < n + 1; i++)
        {
            for (int j = 0; j < resolution; j++)
            {
                float t = knots[i] + (knots[i + 1] - knots[i]) * (float)j / resolution;
                data.push_back(b->evaluate(t));
            }
        }
        data.push_back(b->evaluate(knots[n + 1]));
    }

    base *getBase() override { return b; }
};

#endif // CURVE_HPP
