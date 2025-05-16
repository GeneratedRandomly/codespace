#ifndef REVSURFACE_HPP
#define REVSURFACE_HPP

#include "object3d.hpp"
#include "curve.hpp"
#include <tuple>

// Definition for drawable surface.
typedef std::tuple<unsigned, unsigned, unsigned> Tup3u;

class RevSurface : public Object3D
{

    Curve *pCurve;

    struct Surface
    {
        std::vector<Vector3f> VV; // VV[i] is position of vertex i
        std::vector<Vector3f> VN; // VN[i] is the normal of vertex i
        std::vector<Tup3u> VF;    // VF element: index tuple of 3 vertices in a triangle
    } surface;

public:
    RevSurface(Curve *pCurve, Material *material) : pCurve(pCurve), Object3D(material)
    {
        // Check flat.
        for (const auto &cp : pCurve->getControls())
        {
            if (cp.z() != 0.0)
            {
                printf("Profile of revSurface must be flat on xy plane.\n");
                exit(0);
            }
        }
        setSurface();
    }

    ~RevSurface() override
    {
        delete pCurve;
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override
    {
        // (PA2 optional TODO): implement this for the ray-tracing routine using G-N iteration.
        return false;
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
    }

    void drawGL() override
    {
        Object3D::drawGL();
        glBegin(GL_TRIANGLES);
        for (unsigned i = 0; i < surface.VF.size(); i++)
        {
            glNormal3fv(surface.VN[std::get<0>(surface.VF[i])]);
            glVertex3fv(surface.VV[std::get<0>(surface.VF[i])]);
            glNormal3fv(surface.VN[std::get<1>(surface.VF[i])]);
            glVertex3fv(surface.VV[std::get<1>(surface.VF[i])]);
            glNormal3fv(surface.VN[std::get<2>(surface.VF[i])]);
            glVertex3fv(surface.VV[std::get<2>(surface.VF[i])]);
        }
        glEnd();
    }
};

#endif // REVSURFACE_HPP
