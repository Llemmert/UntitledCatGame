#pragma once
#include <SDL.h>
#include <iostream>
using namespace std;

class spike
{
private:
    SDL_Renderer *ren;
    Animation *a;
    SDL_Rect *src;
    SDL_Rect dest;
    double x, y, w, h;
    SDL_Point center;
    SDL_Point top, left, right;
    double area;

public:
    spike(SDL_Renderer *newRen, Animation *newA, SDL_Rect *newSrc,
          double newx = 0.0, double newy = 0.0 /*, double newW = 0, double newH = 0*/)
    {
        src = newSrc;
        ren = newRen;
        a = newA;
        dest.w = src->w; // img width
        dest.h = src->h; // img height
        dest.x = newx;   // starting x pos
        dest.y = newy;   // starting y pos
        // border.w = newW - 2;
        // border.h = newH - 2;
        // border.x = newx + 2;
        // border.y = newy;
        x = newx;
        y = newy;
        center = {dest.w / 2, dest.h / 2};
        top = {(int)(x + (dest.w / 2)), (int)y};
        left = {(int)x, (int)(y + dest.h)};
        right = {(int)(x + dest.w), (int)(y + dest.h)};
        w = dest.w;
        h = dest.h;
        area = abs((right.x - left.x) * (top.y - left.y) - (top.x - left.x) * (right.y - left.y));
        // SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    }

    void update(double dt)
    {
        // cout << inside(dest.x + center.x, dest.y + center.y);
        SDL_RenderCopy(ren, a->getTexture(), src, &dest);
    }

    // bool inside(int x, int y)
    // {
    //     // return true;
    //     //  double area1 = abs((left.x - x) * (right.y - y) - (right.x - x) * (left.y - y));
    //     //  return true;

    //     // double area2 = abs((right.x - x) * (top.y - y) - (top.x - x) * (right.y - y));
    //     // double area3 = abs((top.x - x) * (left.y - y) - (left.x - x) * (top.y - y));
    //     // cout << area << " " << area1 << " " << area2 << " " << area3 << endl;

    //     // if (area1 + area2 + area3 <= area + 1 && area1 + area2 + area3 >= area - 1)
    //     // if (area1 + area2 + area3 == area)
    //     //     return true;
    //     // return false;

    //     // float area1 =    abs( (x1-px)*(y2-py) - (x2-px)*(y1-py) );
    //     // float area2 =    abs( (x2-px)*(y3-py) - (x3-px)*(y2-py) );
    //     // float area3 =    abs( (x3-px)*(y1-py) - (x1-px)*(y3-py) );
    // }
};