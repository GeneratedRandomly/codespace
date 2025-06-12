#ifndef GROUP_H
#define GROUP_H

#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>

class Group : public Object3D
{

public:
    Group()
    {
    }

    explicit Group(int num_objects)
    {
        objects.reserve(num_objects);
    }

    ~Group() override {}

    bool intersect(const Ray &r, Hit &h, float tmin) override
    {
        bool hit = false;
        for (auto obj : objects)
            hit |= obj->intersect(r, h, tmin);
        return hit;
    }

    void addObject(int index, Object3D *obj)
    {
        objects.insert(objects.begin() + index, obj);
    }

    int getGroupSize()
    {
        return objects.size();
    }

    void drawGL() override
    {
        for (auto obj : objects)
            obj->drawGL();
    }

private:
    std::vector<Object3D *> objects;
};

#endif
