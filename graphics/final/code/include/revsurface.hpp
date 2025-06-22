// 参考了https://github.com/Guangxuan-Xiao/THU-Computer-Graphics-2020/blob/master/project/final/include/revsurface.hpp
// 由于看不懂原作者代码，只保留了其结构，重写了牛顿法。
#ifndef REVSURFACE_HPP
#define REVSURFACE_HPP

#include "object3d.hpp"
#include "curve.hpp"
#include "bvh.hpp"
#include "bound.hpp"
#include <tuple>

constexpr int NEWTON_STEPS = 20;    // Newton iteration steps
constexpr float NEWTON_EPS = 1e-6f; // Newton iteration convergence threshold

// Definition for drawable surface.
typedef std::tuple<unsigned, unsigned, unsigned> Tup3u;

class RevSurface : public Object3D
{
    Bound b;

    Curve *pCurve;

    struct Surface
    {
        std::vector<Vector3f> VV; // VV[i] is position of vertex i
        std::vector<Vector3f> VN; // VN[i] is the normal of vertex i
        std::vector<Tup3u> VF;    // VF element: index tuple of 3 vertices in a triangle
    } surface;

    Triangle_BVH *bvh = nullptr;

    float distance = 0.0f;
    float miny = FLT_MAX;
    float maxy = -FLT_MAX;

public:
    RevSurface(Curve *pCurve, Material *material) : pCurve(pCurve), Object3D(material)
    {
        for (const auto &cp : pCurve->getControls())
        {
            assert(cp.z() == 0.0f);
            float x = fabs(cp.x());
            distance = x > distance ? x : distance;
            miny = cp.y() < miny ? cp.y() : miny;
            maxy = cp.y() > maxy ? cp.y() : maxy;
        }
        b.set(Vector3f(-distance, miny, -distance), Vector3f(distance, maxy, distance));

        setSurface();
    }

    ~RevSurface() override { delete pCurve; }

    bool intersect(const Ray &r, Hit &h, float tmin) override
    {
        // return meshIntersect(r, h, tmin);
        return newtonIntersect(r, h, tmin);
    }

    bool newtonIntersect(const Ray &r, Hit &h, float tmin)
    {
        float t, dummy;
        if (b.intersect(r, t, dummy) == false)
            return false;

        if (t > h.getT() || t < tmin)
            return false;

        Vector3f point = r.at(t);
        float theta = atan2(point.z(), point.x());
        theta = fmod(theta + 2 * M_PI, 2 * M_PI);
        float mu = (point.y() - miny) / (maxy - miny);
        Vector3f normal;

        if (newton(r, t, theta, mu, normal) == false)
            return false;

        if (t > h.getT() || t < tmin)
            return false;

        h.set(t, material, normal);

        return true;
    }

    bool newton(const Ray &r, float &t, float &theta, float &mu, Vector3f &normal)
    {
        Vector3f dmu, dtheta, point;
        for (int i = 0; i < NEWTON_STEPS; ++i)
        {
            theta = fmod(theta + 2 * M_PI, 2 * M_PI);
            mu = mu < 0 ? 0 : (mu > 1 ? 1 : mu); // Clamp mu to [0, 1]

            point = getPoint(theta, mu, dtheta, dmu);
            Vector3f f = r.at(t) - point;
            float dist2 = f.squaredLength();

            normal = Vector3f::cross(dmu, dtheta);
            if (dist2 < NEWTON_EPS)
                return true;

            Vector3f dir = r.getDirection();
            Vector3f df_dt = dir;
            Vector3f df_dtheta = -dtheta;
            Vector3f df_dmu = -dmu;

            Matrix3f J;
            J.setCol(0, df_dt);
            J.setCol(1, df_dtheta);
            J.setCol(2, df_dmu);

            Vector3f delta = J.inverse() * f;

            // 更新变量
            t -= delta[0];
            theta -= delta[1];
            mu -= delta[2];
        }
        return false;
    }

    Vector3f getPoint(const float &theta, const float &mu, Vector3f &dtheta, Vector3f &dmu)
    {
        Quat4f rot;
        rot.setAxisAngle(theta, Vector3f::UP);
        Matrix3f R = Matrix3f::rotation(rot);
        CurvePoint cp = pCurve->getBase()->evaluate(mu);
        dmu = R * cp.T;
        float x = cp.V.x();
        dtheta = Vector3f(-x * sin(theta), 0, x * cos(theta));
        return R * cp.V;
    }

    void setSurface()
    {
        std::vector<CurvePoint> curvePoints;
        pCurve->discretize(30, curvePoints);
        const int steps = 40;
        for (unsigned int ci = 0; ci < curvePoints.size(); ++ci)
        {
            const CurvePoint &cp = curvePoints[ci];
            Vector3f pNormal = Vector3f::cross(cp.T, -Vector3f::FORWARD);
            for (unsigned int i = 0; i < steps; ++i)
            {
                float t = (float)i / steps;
                Quat4f rot;
                rot.setAxisAngle(t * 2 * 3.14159, Vector3f::UP);
                Matrix3f R = Matrix3f::rotation(rot);
                Vector3f pnew = R * cp.V;
                Vector3f nnew = R * pNormal;
                surface.VV.push_back(pnew);
                surface.VN.push_back(nnew);
                int i1 = (i + 1 == steps) ? 0 : i + 1;
                if (ci != curvePoints.size() - 1)
                {
                    surface.VF.emplace_back((ci + 1) * steps + i, ci * steps + i1, ci * steps + i);
                    surface.VF.emplace_back((ci + 1) * steps + i, (ci + 1) * steps + i1, ci * steps + i1);
                }
            }
        }
        auto VV = surface.VV;
        auto VN = surface.VN;
        auto VF = surface.VF;
        std::vector<Triangle *> triangles;
        triangles.reserve(VF.size());
        for (const auto &f : VF)
        {
            unsigned i0, i1, i2;
            std::tie(i0, i1, i2) = f;
            triangles.push_back(new Triangle(VV[i0], VV[i1], VV[i2], material));
        }
        bvh = new Triangle_BVH(triangles);
    }

    inline bool meshIntersect(const Ray &r, Hit &h, float tmin)
    {
        if (bvh == nullptr)
            return false;
        Hit tempHit;
        if (bvh->intersect(r, tempHit) == false)
            return false;

        float temp_t = tempHit.getT();
        if (temp_t >= h.getT() || temp_t < tmin)
            return false;

        h = tempHit;
        return true;
    }
};

#endif // REVSURFACE_HPP
