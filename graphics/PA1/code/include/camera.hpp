#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>

class Camera
{
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH)
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
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
                      const Vector3f &up, int imgW, int imgH, float angle)
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

#endif // CAMERA_H
