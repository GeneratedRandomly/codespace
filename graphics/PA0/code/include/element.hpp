#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>

class Element
{
public:
    virtual void draw(Image &img) = 0;
    virtual ~Element() = default;
};

class Line : public Element
{
public:
    int xA, yA;
    int xB, yB;
    Vector3f color;
    void draw(Image &img) override
    {
        int x, y, dx, dy, error;
        bool is_steep = abs(yB - yA) > abs(xB - xA); // k > 1
        if (is_steep)
        {
            std::swap(xA, yA);
            std::swap(xB, yB);
        }
        if (xA > xB)
        {
            std::swap(xA, xB);
            std::swap(yA, yB);
        }
        dx = xB - xA, dy = abs(yB - yA), x = xA, y = yA;
        error = -dx;
        int ystep = yA < yB ? 1 : -1;
        for (int i = 0; i <= dx; i++)
        {
            is_steep ? img.SetPixel(y, x, color) : img.SetPixel(x, y, color);
            x++;
            error += dy << 1;
            while (error >= 0)
            {
                error -= dx << 1;
                y += ystep;
            }
        }
    }
};

class Circle : public Element
{
    void circlepoints(Image &img, int x, int y)
    {
        img.SetPixel(x + cx, y + cy, color);
        img.SetPixel(y + cx, x + cy, color);
        img.SetPixel(-x + cx, y + cy, color);
        img.SetPixel(-y + cx, x + cy, color);
        img.SetPixel(-x + cx, -y + cy, color);
        img.SetPixel(-y + cx, -x + cy, color);
        img.SetPixel(x + cx, -y + cy, color);
        img.SetPixel(y + cx, -x + cy, color);
    }

public:
    int cx, cy;
    int radius;
    Vector3f color;
    void draw(Image &img) override
    {
        int x = 0, y = radius, d = 3 - (radius << 1);
        circlepoints(img, x, y);
        while (x <= y)
        {
            if (d < 0)
                d += (x << 2) + 6;
            else
            {
                d += ((x - y) << 2) + 10;
                y--;
            }
            x++;
            circlepoints(img, x, y);
        }
    }
};

class Fill : public Element
{
    bool unfillable(Image &img, int x, int y, Vector3f &oldcolor)
    {
        if (x < 0 || x >= img.Width() || y < 0 || y >= img.Height())
            return true;
        if (img.GetPixel(x, y) != oldcolor)
            return true;
        return false;
    }

    void fillcolor(Image &img, int x, int y, Vector3f &oldcolor, Vector3f &newcolor)
    {
        if (unfillable(img, x, y, oldcolor))
            return;
        std::queue<std::pair<int, int>> q;
        q.push({x, y});
        while (!q.empty())
        {
            std::pair<int, int> p = q.front();
            x = p.first, y = p.second;
            q.pop();
            if (unfillable(img, x, y, oldcolor))
                continue;
            img.SetPixel(x, y, newcolor);
            q.push({x + 1, y});
            q.push({x - 1, y});
            q.push({x, y + 1});
            q.push({x, y - 1});
        }
    }

public:
    int cx, cy;
    Vector3f color;
    void draw(Image &img) override
    {
        Vector3f oldcolor = img.GetPixel(cx, cy);
        fillcolor(img, cx, cy, oldcolor, color);
    }
};