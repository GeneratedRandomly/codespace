#include "mesh.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <sstream>

bool Mesh::intersect(const Ray &r, Hit &h, float tmin)
{
    assert(bvh != nullptr);
    Hit tempHit;
    if (bvh->intersect(r, tempHit) == false)
        return false;
    float temp_t = tempHit.getT();
    if (temp_t >= h.getT() || temp_t < tmin)
        return false;
    h = tempHit;
    return true;
}

Mesh::Mesh(const char *filename, Material *material) : Object3D(material)
{

    // Optional: Use tiny obj loader to replace this simple one.
    std::ifstream f;
    f.open(filename);
    if (!f.is_open())
    {
        std::cout << "Cannot open " << filename << "\n";
        return;
    }
    std::string line;
    std::string vTok("v");
    std::string fTok("f");
    std::string texTok("vt");
    char bslash = '/', space = ' ';
    std::string tok;
    int texID;
    while (true)
    {
        std::getline(f, line);
        if (f.eof())
        {
            break;
        }
        if (line.size() < 3)
        {
            continue;
        }
        if (line.at(0) == '#')
        {
            continue;
        }
        std::stringstream ss(line);
        ss >> tok;
        if (tok == vTok)
        {
            Vector3f vec;
            ss >> vec[0] >> vec[1] >> vec[2];
            v.push_back(vec);
        }
        else if (tok == fTok)
        {
            if (line.find(bslash) != std::string::npos)
            {
                std::replace(line.begin(), line.end(), bslash, space);
                std::stringstream facess(line);
                TriangleIndex trig;
                facess >> tok;
                for (int ii = 0; ii < 3; ii++)
                {
                    facess >> trig[ii] >> texID;
                    trig[ii]--;
                }
                t.push_back(trig);
            }
            else
            {
                TriangleIndex trig;
                for (int ii = 0; ii < 3; ii++)
                {
                    ss >> trig[ii];
                    trig[ii]--;
                }
                t.push_back(trig);
            }
        }
        else if (tok == texTok)
        {
            Vector2f texcoord;
            ss >> texcoord[0];
            ss >> texcoord[1];
        }
    }
    computeNormal();

    f.close();

    std::vector<Triangle *> tris;
    for (size_t triId = 0; triId < t.size(); ++triId)
    {
        TriangleIndex &triIndex = t[triId];
        Triangle *tri = new Triangle(
            v[triIndex[0]],
            v[triIndex[1]],
            v[triIndex[2]],
            material);
        tri->normal = n[triId];
        tris.push_back(tri);
    }
    // 构建BVH
    bvh = new Triangle_BVH(tris);
}

void Mesh::computeNormal()
{
    n.resize(t.size());
    for (int triId = 0; triId < (int)t.size(); ++triId)
    {
        TriangleIndex &triIndex = t[triId];
        Vector3f a = v[triIndex[1]] - v[triIndex[0]];
        Vector3f b = v[triIndex[2]] - v[triIndex[0]];
        b = Vector3f::cross(a, b);
        n[triId] = b / b.length();
    }
}
