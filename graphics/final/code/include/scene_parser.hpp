// 总体继承自PA1，增加了景深相机parseFocusCamera、
// 蒙特卡洛面光源parseCircleLight，并且从PA2处复制了参数曲面的函数
#ifndef SCENE_PARSER_H
#define SCENE_PARSER_H

#include <cassert>
#include <vecmath.h>

class Camera;
class Light;
class Material;
class Object3D;
class Group;
class Sphere;
class Plane;
class Triangle;
class Transform;
class Mesh;
class Curve;
class RevSurface;
#define MAX_PARSER_TOKEN_LENGTH 1024

class SceneParser
{
public:
    SceneParser() = delete;
    SceneParser(const char *filename);

    ~SceneParser();

    Camera *getCamera() const
    {
        return camera;
    }

    Vector3f getBackgroundColor() const
    {
        return background_color;
    }

    int getNumLights() const
    {
        return num_lights;
    }

    Light *getLight(int i) const
    {
        assert(i >= 0 && i < num_lights);
        return lights[i];
    }

    int getNumMaterials() const
    {
        return num_materials;
    }

    Material *getMaterial(int i) const
    {
        assert(i >= 0 && i < num_materials);
        return materials[i];
    }

    Group *getGroup() const
    {
        return group;
    }

private:
    void parseFile();
    void parsePerspectiveCamera();
    void parseFocusCamera();
    void parseBackground();
    void parseLights();
    Light *parsePointLight();
    Light *parseDirectionalLight();
    Light *parseCircleLight();
    void parseMaterials();
    Material *parseMaterial();
    Material *parsePhongMaterial();
    Object3D *parseObject(char token[MAX_PARSER_TOKEN_LENGTH]);
    Group *parseGroup();
    Sphere *parseSphere();
    Plane *parsePlane();
    Triangle *parseTriangle();
    Mesh *parseTriangleMesh();
    Transform *parseTransform();
    Curve *parseBezierCurve();
    Curve *parseBsplineCurve();
    RevSurface *parseRevSurface();

    int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);

    Vector3f readVector3f();

    float readFloat();
    int readInt();

    FILE *file;
    Camera *camera;
    Vector3f background_color;
    int num_lights;
    Light **lights;
    int num_materials;
    Material **materials;
    Material *current_material;
    Group *group;
};

#endif // SCENE_PARSER_H
