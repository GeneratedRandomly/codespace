#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

// INFO: Triangle class, representing a triangle in 3D space
class Triangle : public Object3D
{

public:
	Triangle() = delete;

	// a b c are three vertex positions of the triangle
	Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, Material *m) : Object3D(m)
	{
		vertices[0] = a;
		vertices[1] = b;
		vertices[2] = c;
		normal = Vector3f::cross(b - a, c - a).normalized(); // 计算法向量
	}

	bool intersect(const Ray &ray, Hit &hit, float tmin) override
	{
		Vector3f r0 = ray.getOrigin();				   // 光线起点
		Vector3f rd = ray.getDirection(); // 光线方向单位化
		Vector3f e1 = vertices[0] - vertices[1];	   // 边 1
		Vector3f e2 = vertices[0] - vertices[2];	   // 边 2
		Vector3f s = vertices[0] - r0;				   // 光线起点到三角形顶点的向量
		Vector3f tbg =
			(1 / Matrix3f(rd, e1, e2).determinant()) *
			Vector3f(Matrix3f(s, e1, e2).determinant(),
					 Matrix3f(rd, s, e2).determinant(),
					 Matrix3f(rd, e1, s).determinant()); // 计算重心坐标
		float t = tbg[0];
		float b = tbg[1];
		float g = tbg[2];
		if (t > tmin && t < hit.getT() && 0 <= b && 0 <= g && b + g <= 1)
		{
			hit.set(t, material, normal); // 设置交点信息
			return true;
		}
		return false;
	}
	Vector3f normal;
	Vector3f vertices[3];

protected:
};

#endif // TRIANGLE_H
