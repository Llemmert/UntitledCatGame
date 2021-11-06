#pragma once
#include <vector>
#include <fstream>
#include <ctime>
#include "drawtext.hpp"
#include "Animation.hpp"
#include "Wall.hpp"

using namespace std;

class Cat
{
	SDL_Renderer *ren;
	Animation *a;
	SDL_Rect *src;
	SDL_Rect dest;
	double x, y, vx, vy, ax, ay, gravity, rotate;
	int minx, miny, maxx, maxy;
	SDL_Point center;
	SDL_RendererFlip FlipState = SDL_FLIP_NONE;
	bool grounded;
	bool meow;
	double meowtimer;
	time_t initial;
	int playernumber;

public:
	Cat(SDL_Renderer *newRen, Animation *newA, SDL_Rect *newSrc,
		double newx = 0.0, double newy = 0.0, double newvx = 0.0, double newvy = 0.0,
		double newax = 0, double newgravity = 0, int newplayernumber = 0)
	{
		src = newSrc;
		ren = newRen;
		a = newA;
		dest.w = src->w; // img width
		dest.h = src->h; // img height
		dest.x = newx;	 // starting x pos
		dest.y = newy;	 // starting y pos
		playernumber = newplayernumber;
		x = newx;
		y = newy;
		vx = newvx; // x velocity
		vy = newvy; // y velocity
		ax = newax;
		ay = 0;
		gravity = newgravity;
		center = {30, 20};
		grounded = true;
		meow = false;
		meowtimer = 0;
		initial = -1;
	}
	void setBound(int newMinX, int newMinY, int newMaxX, int newMaxY) // bounds based on window size
	{
		minx = newMinX;
		maxx = newMaxX;
		miny = newMinY;
		maxy = newMaxY;
	}
	void update(double dt)
	{
		if (maxx != minx) // causes the cats to loop/reappear on the other side when they reach an edge
		{
			if (x <= minx)
			{
				x = maxx;
			}
			else if (x >= maxx)
			{
				x = minx;
			}
		}
		if (maxy != miny)
		{
			if (y < miny)
			{
				y = miny;
				vy = 0;
				grounded = true;
			}
			if (y > maxy)
			{
				y = maxy;
				vy = 0;
				grounded = true;
				// cout << ay << endl;
			}
		}

		if (vx != 0)
		{
			a->update(dt);
		}

		if (vy > 0)
		{
			if (FlipState == SDL_FLIP_NONE)
				rotate = -30;
			else
				rotate = 30;
		}
		if (vy < 0)
		{
			if (FlipState == SDL_FLIP_NONE)
				rotate = 25;
			else
				rotate = -25;
		}

		if (vy == 0)
			rotate = 0;
		if (!grounded)
			ay = gravity;
		else
			ay = 0;
		vx += ax * dt;
		vy += ay * dt;
		x += vx * dt;
		y += vy * dt;	 // update x and y velocities of cat
		dest.x = (int)x; // sets new destination of x and y
		dest.y = (int)y;
		// a->update(dt);	//animate! animate! animate! make leg move
		if (vx < 0)
			FlipState = SDL_FLIP_NONE;
		if (vx > 0)
			FlipState = SDL_FLIP_HORIZONTAL;
		SDL_RenderCopyEx(ren, a->getTexture(), src, &dest, rotate, &center, FlipState);
		if (meow)
		{
			if (initial == -1)
				initial = time(0);
			if (playernumber == 1)
				drawText(src, ren, "*meow*", x - 25, y - 40, 0, {255, 255, 255});
			else
				drawText(src, ren, "F*ckoff m8", x - 25, y + 40, 0, {255, 255, 255});
			if (difftime(time(0), initial) > .5)
			{
				meow = false;
				initial = -1;
			}
		}
	}
	// void animateWalk(double dt){
	//	a->update(dt);
	//	SDL_RenderCopy(ren,a->getTexture(), src, &dest);
	// }

	void setVelocityX(double velocity)
	{
		vx = velocity;
	}
	void setVelocityY(double velocity)
	{
		vy = velocity;
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
	bool isGrounded()
	{
		return (grounded);
	}
	void setGrounded(bool groundvar)
	{
		grounded = groundvar;
	}
	void setMeow(bool newmeow)
	{
		meow = newmeow;
	}
	bool inside(int x, int y)
	{
		return (dest.x <= x && x <= dest.x + dest.w &&
				dest.y <= y && y <= dest.y + dest.h);
	}
	bool detectCollision(wall *aWall)
	{
		return ((inside(aWall->getx(), aWall->gety())) ||
				(inside(aWall->getx() + aWall->getw(), aWall->gety() + aWall->geth())) ||
				(inside(aWall->getx() + aWall->getw(), aWall->gety())) ||
				(inside(aWall->getx(), aWall->gety() + aWall->geth())));
	}
	void handleCollision(wall *aWall)
	{
		if (detectCollision(aWall))
		{
			double xcomp = (x + (dest.w / 2)) - aWall->centerx();
			double ycomp = (y + (dest.h / 2)) - aWall->centery();
			double tempx = dest.w / 2 + aWall->getw() / 2;
			// double movcompx = xcomp - (dest.w / 2);
			// double movcompy = ycomp - (dest.h / 2);
			x -= 3;
			// y -= vy;
			vx = 0;
			// dy = 0;
		}
	}
	// return (dest.x <= x && x <= dest.x + dest.w &&
	// 		dest.y <= y && y <= dest.y + dest.h)
};
