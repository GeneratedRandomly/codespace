// BVH系自主实现。
#ifndef BVH_HPP
#define BVH_HPP
#include <vector>
#include <memory>
#include <algorithm>
#include "bound.hpp"
#include "vecmath.h"
#include "object3d.hpp"
#include "triangle.hpp"
#include "util.hpp"

// 这里用不了make_unique的补救办法是copilot提供。
#if __cplusplus <= 201103L
namespace std
{
    template <typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&...args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif

struct BVHNode
{
    Bound bound;
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;
    std::vector<Triangle *> objects; // 叶节点存三角形指针

    bool isLeaf() const { return left == nullptr && right == nullptr; }

    bool intersect(const Ray &ray, float &t_min, float &t_max) const
    {
        return bound.intersect(ray, t_min, t_max);
    }
};

class Triangle_BVH
{
public:
    std::unique_ptr<BVHNode> root;

    Triangle_BVH(std::vector<Triangle *> &objects)
    {
        std::vector<Triangle *> tris = objects;
        root = build(tris, 0);
    }

    bool intersect(const Ray &ray, Hit &hit) const
    {
        return intersectNode(root.get(), ray, hit);
    }

private:
    std::unique_ptr<BVHNode> build(std::vector<Triangle *> &tris, int depth)
    {
        auto node = std::make_unique<BVHNode>();
        node->bound = computeBound(tris);

        if (tris.size() <= 4 || depth > 20)
        {
            node->objects = tris;
            return node;
        }

        // 选择最长轴
        Vector3f diag = node->bound.max - node->bound.min;
        int axis = 0;
        if (diag[1] > diag[0])
            axis = 1;
        if (diag[2] > diag[axis])
            axis = 2;

        // 按三角形中心排序
        std::vector<Triangle *> sorted = tris;
        std::sort(sorted.begin(), sorted.end(), [axis](const Triangle *a, const Triangle *b)
                  {
            float ca = (a->vertices[0][axis] + a->vertices[1][axis] + a->vertices[2][axis]) / 3.0f;
            float cb = (b->vertices[0][axis] + b->vertices[1][axis] + b->vertices[2][axis]) / 3.0f;
            return ca < cb; });

        size_t mid = sorted.size() / 2;
        std::vector<Triangle *> leftTris(sorted.begin(), sorted.begin() + mid);
        std::vector<Triangle *> rightTris(sorted.begin() + mid, sorted.end());

        node->left = build(leftTris, depth + 1);
        node->right = build(rightTris, depth + 1);
        return node;
    }

    Bound computeBound(std::vector<Triangle *> &tris)
    {
        Bound b;
        for (auto tri : tris)
        {
            b.update(tri->vertices[0]);
            b.update(tri->vertices[1]);
            b.update(tri->vertices[2]);
        }
        return b;
    }

    bool intersectNode(BVHNode *node, const Ray &ray, Hit &hit) const
    {
        float tmin, tmax;
        if (node == nullptr)
            return false;
        if (node->bound.intersect(ray, tmin, tmax) == false)
            return false;

        if (node->isLeaf())
        {
            bool hitFound = false;
            for (Triangle *tri : node->objects)
            {
                Hit tempHit;
                if (tri->intersect(ray, tempHit, eps))
                {
                    if (tempHit.getT() < hit.getT())
                    {
                        hit = tempHit;
                        hitFound = true;
                    }
                }
            }
            return hitFound;
        }

        bool leftHit = intersectNode(node->left.get(), ray, hit);
        bool rightHit = intersectNode(node->right.get(), ray, hit);
        return leftHit || rightHit;
    }
};

#endif // BVH_HPP