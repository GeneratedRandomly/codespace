// 继承自PA1
#ifndef GROUP_H
#define GROUP_H

#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>

// INFO: Group is a collection of objects that can be treated as a single object for intersection tests.
class Group : public Object3D
{

public:
    Group() = default;

    explicit Group(int num_objects) { objects.reserve(num_objects); }

    ~Group() override
    {
        for (auto &object : objects)
            delete object;
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override
    {
        bool result = false;
        for (Object3D *object : objects)
            result |= object->intersect(r, h, tmin);
        return result;
    }

    void addObject(int index, Object3D *obj)
    {
        objects.insert(objects.begin() + index, obj);
    }

    int getGroupSize() { return objects.size(); }

private:
    std::vector<Object3D *> objects; // list of objects in the group
};

#endif
