// PerspectiveCamera系继承自PA1，FocusCamera系自主实现，用来实现景深效果。
#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include "util.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>

class Camera
{
public:
    Camera(const Vector3f &center,
           const Vector3f &direction,
           const Vector3f &up,
           int imgW,
           int imgH)
    {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up).normalized();
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

// INFO: Perspective camera
class PerspectiveCamera : public Camera
{
    float cx, cy, fx, fy;

public:
    PerspectiveCamera(const Vector3f &center,
                      const Vector3f &direction,
                      const Vector3f &up,
                      int imgW,
                      int imgH,
                      float angle)
        : Camera(center, direction, up, imgW, imgH)
    {
        cx = imgW >> 1;
        cy = imgH >> 1;
        float tan_half_ang = tan(angle / 2);
        fx = cx / tan_half_ang;
        fy = cy / tan_half_ang;
    }

    Ray generateRay(const Vector2f &point) override
    {
        Vector3f rayDirection =
            (point[0] - cx) / fx * horizontal + (point[1] - cy) / fy * up + direction;
        rayDirection.normalize();
        return Ray(center, rayDirection);
    }
};
class FocusCamera : public Camera
{
    float cx, cy, fx, fy;
    float focusDistance;
    float aperture; // 光圈大小

public:
    FocusCamera(const Vector3f &center,
                const Vector3f &direction,
                const Vector3f &up,
                int imgW,
                int imgH,
                float angle,
                float focusDist,
                float apertureSize)
        : Camera(center, direction, up, imgW, imgH),
          focusDistance(focusDist),
          aperture(apertureSize)
    {
        cx = imgW >> 1;
        cy = imgH >> 1;
        float tan_half_ang = tan(angle / 2);
        fx = cx / tan_half_ang;
        fy = cy / tan_half_ang;
    }

    Ray generateRay(const Vector2f &point) override
    {
        Vector3f rayDir = (point[0] - cx) / fx * horizontal + (point[1] - cy) / fy * up + direction;
        rayDir.normalize();

        Vector3f focusPoint = center + rayDir * focusDistance;

        float r = sqrt(getRan());
        float theta = getRan() * 2 * M_PI;

        float dx = r * cos(theta) * aperture * 0.5f;
        float dy = r * sin(theta) * aperture * 0.5f;
        Vector3f offset = dx * horizontal + dy * up;

        Vector3f origin = center + offset;
        Vector3f newDir = (focusPoint - origin).normalized();

        return Ray(origin, newDir);
    }
};

#endif // CAMERA_H
