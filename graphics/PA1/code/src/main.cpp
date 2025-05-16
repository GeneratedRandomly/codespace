#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    for (int argNum = 1; argNum < argc; ++argNum)
    {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3)
    {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2]; // only bmp is allowed.

    // INFO
    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    SceneParser sp = SceneParser(inputFile.c_str());
    Camera *camera = sp.getCamera();
    int w = camera->getWidth();
    int h = camera->getHeight();
    Image img = Image(w, h);
    Group *baseGroup = sp.getGroup();
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; y++)
        {
            Ray camRay = camera->generateRay(Vector2f(x, y));
            Hit hit;
            bool isIntersect = baseGroup->intersect(camRay, hit, 0);
            if (isIntersect)
            {
                Vector3f finalColor = Vector3f::ZERO;
                for (int li = 0; li < sp.getNumLights(); ++li)
                {
                    Light *light = sp.getLight(li);
                    Vector3f L, lightcolor;
                    light->getIllumination(camRay.pointAtParameter(hit.getT()), L, lightcolor);
                    finalColor += hit.getMaterial()->Shade(camRay, hit, L, lightcolor);
                }
                img.SetPixel(x, y, finalColor);
            }
            else
                img.SetPixel(x, y, sp.getBackgroundColor());
        }
    }

    cout << "Hello! Computer Graphics!" << endl;
    img.SaveBMP(outputFile.c_str());
    return 0;
}
