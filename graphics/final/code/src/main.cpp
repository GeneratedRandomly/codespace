#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <omp.h>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"
#include "path_tracing.hpp"
#include "whitted_style.hpp"

#include <string>

// using namespace std;
using std::cout;
using std::endl;
using std::string;

constexpr bool GAMMA_CORRECTION = false;

int main(int argc, char *argv[])
{
    // Begin：继承自PA1
    for (int argNum = 1; argNum < argc; ++argNum)
    {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }
    if (argc != 3)
    {
        cout << "Usage: ./build/final <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2]; // only bmp is allowed.

    SceneParser sp = SceneParser(inputFile.c_str());
    Camera *camera = sp.getCamera();
    int w = camera->getWidth();
    int h = camera->getHeight();
    Image img = Image(w, h);
    Group *baseGroup = sp.getGroup();
    // End：继承自PA1
    // Begin：修改自PA1
#pragma omp parallel for schedule(dynamic, 1)
    for (int x = 0; x < w; ++x)
    {
        printf("\rRendering %5.2f%%", 100.0 * x / (w - 1));
        for (int y = 0; y < h; ++y)
        {
            Ray camRay = camera->generateRay(Vector2f(x, y));
            Vector3f color = pathTracing(camRay, baseGroup, sp, 0);
            img.SetPixel(x, y, color);
        }
    }
    // End：修改自PA1
    // Begin：后处理环节，可以调用伽马校正等函数。
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            Vector3f &pixelColor = img.pixel(x, y);
            tieDyeMapping(pixelColor);
        }
    }
    cout << "Hello! Computer Graphics!" << endl;
    img.SaveBMP(outputFile.c_str());
    return 0;
}
