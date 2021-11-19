#include <SDL.h>
#include <iostream>
#include "Cat.hpp"

using namespace std;

class block
{
private:
    SDL_Renderer *ren;
    Animation *a;
    SDL_Rect *src;
    SDL_Rect dest;
    double x, y, w, h, vx, vy, ax, ay;
    SDL_Point center;
    int minx, maxx;
    SDL_Rect StompBox;

public:
    block(SDL_Renderer *newRen, Animation *newA, SDL_Rect *newSrc,
          double newx = 0.0, double newy = 0.0, double newvx = 0.0, double newvy = 0.0)
    {
        src = newSrc;
        ren = newRen;
        a = newA;
        dest.w = src->w; // img width
        dest.h = src->h; // img height
        dest.x = newx;   // starting x pos
        dest.y = newy;   // starting y pos
        vx = newvx;
        vy = newvy;
        // border.w = newW - 2;
        // border.h = newH - 2;
        // border.x = newx + 2;
        // border.y = newy;
        x = newx;
        y = newy;
        center = {dest.w / 2, dest.h / 2};
        w = dest.w;
        h = dest.h;

        if (dest.y + center.y < 243)
        {
            StompBox.x = dest.x;
            StompBox.y = dest.y;
            StompBox.w = dest.w;
            StompBox.h = 10;
        }
        else
        {
            StompBox.x = dest.x;
            StompBox.y = dest.y + dest.h - 10;
            StompBox.w = dest.w;
            StompBox.h = 10;
        }
        // SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    }
    // collision(int Obx, int Oby)
    // {
    // 	if (Obx < x && Oby < y)
    // 		cout << " test" << endl;
    // }
    void update(double dt)
    {
        if (maxx != minx)
        {
            if (x <= minx)
            {
                vx *= (-1);
            }
            else if (x >= maxx)
            {
                vx *= (-1);
            }
        }
        x += vx * dt;
        y += vy * dt;
        dest.x = (int)x;
        dest.y = (int)y;
        if (vx > 0)
            vx -= ax * dt;
        if (vx < 0)
            vx += ax * dt;
        if (vy < 0)
            vy += ay * dt;
        if (vy > 0)
            vy -= ay * dt;
        StompBox.x = dest.x;
        StompBox.y = dest.y;
        a->update(dt);
        SDL_RenderCopy(ren, a->getTexture(), src, &dest);
        // SDL_RenderDrawRect(ren, &dest);
        // SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        // SDL_RenderFillRect(ren, &border);
        //  SDL_RenderDrawRect(ren, &dest);
    }
    void setBound(int newMinX, int newMaxX) // bounds based on window size
    {
        minx = newMinX;
        maxx = newMaxX;
    }
    bool detectCollision(wall *aWall)
    {
        return ((inside(aWall->getx(), aWall->gety())) ||
                (inside(aWall->getx() + aWall->getw(), aWall->gety() + aWall->geth())) ||
                (inside(aWall->getx() + aWall->getw(), aWall->gety())) ||
                (inside(aWall->getx(), aWall->gety() + aWall->geth())));
    }
    void handleCollision(vector<wall *> &walls, double dt)
    {
        for (auto aWall : walls)
        {
            if (detectCollision(aWall))
            {
                if (vx < 0)
                    dest.x += 3;
                else
                    dest.x -= 3;
                vx = -vx;
            }
        }
    }
    void sety(int newy)
    {
        y = newy;
    }
    void setx(int newx)
    {
        x = newx;
    }
    double getx()
    {
        return x;
    }
    double gety()
    {
        return y;
    }
    double centerx()
    {
        return center.x;
    }
    double centery()
    {
        return center.y;
    }
    double getw()
    {
        return w;
    }
    double geth()
    {
        return h;
    }
    double getvx()
    {
        return vx;
    }
    double getvy()
    {
        return vy;
    }
    bool insideStompBox(int x, int y)
    {
        return (StompBox.x <= x && x <= StompBox.x + StompBox.w &&
                StompBox.y <= y && y <= StompBox.y + StompBox.h);
    }
    void collided(double newvx)
    {
        vx = newvx;
        ax = 10;
    }
    bool inside(int x, int y)
    {
        return (dest.x <= x && x <= dest.x + dest.w &&
                dest.y <= y && y <= dest.y + dest.h);
    }
};
