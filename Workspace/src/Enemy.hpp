#pragma once
#include <SDL.h>
#include <cmath>
#include <iostream>
using namespace std;

class enemy
{
private:
    SDL_Renderer *ren;
    Animation *a;
    SDL_Rect *src;
    SDL_Rect dest;
    SDL_Rect StompBox;
    double x, y, vx, vy, w, h;
    SDL_Point center;
    SDL_RendererFlip FlipState = SDL_FLIP_NONE;

public:
    enemy(SDL_Renderer *newRen, Animation *newA, SDL_Rect *newSrc,
          double newx = 0.0, double newy = 0.0, double newvx = 0.0, double newvy = 0.0 /*, double newW = 0, double newH = 0*/)
    {
        src = newSrc;
        ren = newRen;
        a = newA;
        dest.w = src->w / 6; // img width
        dest.h = src->h / 6; // img height
        dest.x = newx;       // starting x pos
        dest.y = newy;       // starting y pos

        center = {dest.w / 2, dest.h / 2};

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

        x = newx;
        y = newy;
        vx = newvx;
        vy = newvy;
    }
    void update(double dt)
    {
        x += vx * dt;
        y += vy * dt;
        dest.x = (int)x;
        dest.y = (int)y;
        StompBox.x = dest.x;
        StompBox.y = dest.y;
        a->update(dt);
        if (vx < 0)
        {
            if (dest.y + center.y > 243)
                FlipState = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
            else
                FlipState = SDL_FLIP_HORIZONTAL;
        }
        else if (dest.y + center.y > 243)
            FlipState = SDL_FLIP_VERTICAL;
        else
            FlipState = SDL_FLIP_NONE;

        SDL_RenderCopyEx(ren, a->getTexture(), src, &dest, 0, &center, FlipState);
    }

    double getVelocityX()
    {
        return vx;
    }
    double getVelocityY()
    {
        return vy;
    }
    double getx()
    {
        return x;
    }
    double gety()
    {
        return y;
    }

    bool inside(int x, int y)
    {
        return (dest.x <= x && x <= dest.x + dest.w &&
                dest.y <= y && y <= dest.y + dest.h);
    }

    bool insideStompBox(int x, int y)
    {
        return (StompBox.x <= x && x <= StompBox.x + StompBox.w &&
                StompBox.y <= y && y <= StompBox.y + StompBox.h);
    }

    bool detectCollision(wall *aWall)
    {
        double adx = abs((dest.x + (dest.w / 2)) - (aWall->getx() + aWall->centerx()));
        double ady = abs((dest.y + (dest.h / 2)) - (aWall->gety() + aWall->centery()));
        double shw = (dest.w / 2) + aWall->centerx();
        double shh = (dest.h / 2) + aWall->centery();
        if (adx <= shw && ady <= shh)
            return true;
        return false;
    }

    void handleCollision(vector<wall *> &walls)
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
};