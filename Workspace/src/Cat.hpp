#pragma once
#include <vector>
#include <fstream>
#include <ctime>
#include <cmath>
#include "drawtext.hpp"
#include "Animation.hpp"
#include "Wall.hpp"
#include "Enemy.hpp"
#include "Block.hpp"

//#include "Spike.hpp"

using namespace std;

class Cat
{
	SDL_Renderer *ren;
	vector<Animation> *anims;
	SDL_Rect *src;
	SDL_Rect dest;
	double x, y, vx, vy, ax, ay, gravity, rotate;
	int minx, miny, maxx, maxy;
	SDL_Point center, gcheck1, gcheck2, gcheck3;
	SDL_RendererFlip FlipState = SDL_FLIP_NONE;
	bool grounded, alive;
	bool meow;
	double meowtimer;
	time_t initial, idleTime, newIdleTime;
	int playernumber;
	int idleInSeconds;

public:
	Cat(SDL_Renderer *newRen, vector<Animation> *newAnims, SDL_Rect *newSrc,
		double newx = 0.0, double newy = 0.0, double newvx = 0.0, double newvy = 0.0,
		double newax = 0, double newgravity = 0, int newplayernumber = 0)
	{
		src = newSrc;
		ren = newRen;
		anims = newAnims;
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
		alive = true;
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
		int animNumber = 0;
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
			anims->at(animNumber).update(dt);
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

		if (isIdle(vx, vy))
		{
			if (idleTime == 0)
				idleTime = time(0);
			else
				newIdleTime = time(0);
			idleInSeconds = (int)-difftime(idleTime, newIdleTime); // time spent idle in seconds

			if (idleInSeconds > 5) // IF IDLE FOR LONGER THAN 5 SECONDS
			{
				animNumber = 1;
				anims->at(animNumber).update(dt);
			}
		}
		else
		{
			// reset idle count
			idleTime = 0;
		}

		if (vx < 0)
			FlipState = SDL_FLIP_NONE;
		if (vx > 0)
			FlipState = SDL_FLIP_HORIZONTAL;
		SDL_RenderCopyEx(ren, anims->at(animNumber).getTexture(), src, &dest, rotate, &center, FlipState);
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

	// might improve collision detection scheme later by making it non-point based
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
	void handleCollision(vector<wall *> &walls, double dt)
	{
		for (auto aWall : walls)
		{
			if (detectCollision(aWall))
			{
				double dx = (dest.x + (dest.w / 2)) - (aWall->getx() + aWall->centerx());
				double dy = (dest.y + (dest.h / 2)) - (aWall->gety() + aWall->centery());
				double shw = (dest.w / 2) + (aWall->getw() / 2);
				double shh = (dest.h / 2) + (aWall->geth() / 2);
				// cout << abs(dx) << " " << abs(dy) << endl;
				// cout << dest.y + (dest.h / 2) << " " << aWall->centery() << endl;
				if ((shw - abs(dx)) <= (shh - abs(dy)))
				{
					x -= vx * dt;
					vx = 0;
				}
				else if (vy > 0 && playernumber == 1)
				{
					// cout << shw - abs(vx) << " " << shh - abs(vy) << endl;
					y += (vy * dt) - 1;
					vy = 0;
					grounded = true;
				}
				else if (vy <= 0 && playernumber == 1)
				{
					y += (vy * dt) + 1;
					vy = 0;
				}
				else if (vy >= 0 && playernumber == 2)
				{
					y += (vy * dt) - 1;
					vy = 0;
				}
				else if (vy < 0 && playernumber == 2)
				{
					y += (vy * dt) + 1;
					vy = 0;
					grounded = true;
				}
			}
		}
	}

	//////

	bool detectBlockCollision(block *aBlock)
	{
		return ((inside(aBlock->getx(), aBlock->gety())) ||
				(inside(aBlock->getx() + aBlock->getw(), aBlock->gety() + aBlock->geth())) ||
				(inside(aBlock->getx() + aBlock->getw(), aBlock->gety())) ||
				(inside(aBlock->getx(), aBlock->gety() + aBlock->geth())));
	}
	void handleBlockCollision(vector<block *> &blocks, double dt)
	{
		for (auto aBlock : blocks)
		{
			if (detectBlockCollision(aBlock))
			{
				double dx = (dest.x + (dest.w / 2)) - (aBlock->getx() + aBlock->centerx());
				double dy = (dest.y + (dest.h / 2)) - (aBlock->gety() + aBlock->centery());
				double shw = (dest.w / 2) + (aBlock->getw() / 2);
				double shh = (dest.h / 2) + (aBlock->geth() / 2);
				aBlock->collided(getVelocityX() * .6);
				if ((shw - abs(dx)) <= (shh - abs(dy)))
				{
					x -= vx * dt;
					vx = 0;
				}
				if (playernumber == 1)
				{
					if (aBlock->insideStompBox(x + (dest.w / 2), y + dest.h + 1) || aBlock->insideStompBox(x + (dest.w), y + dest.h + 1) ||
						aBlock->insideStompBox(x, y + dest.h + 1))
						if (vy > 0)
						{
							vy = -480;
							aBlock->sety(244);
						}
				}

				if (playernumber == 2)
				{
					if (aBlock->insideStompBox(x + (dest.w / 2), y - 1) || aBlock->inside(x + (dest.w), y - 1) ||
						aBlock->inside(x, y - 1))
						if (vy < 0)
						{
							vy = 480;
							aBlock->sety(207);
						}
				}
			}
		}
	}

	//////

	void handleEnemyCollision(enemy *aEnemy)
	{
		if (playernumber == 1)
		{
			if (aEnemy->insideStompBox(x + (dest.w / 2), y + dest.h + 1) || aEnemy->insideStompBox(x + (dest.w), y + dest.h + 1) ||
				aEnemy->insideStompBox(x, y + dest.h + 1))
			{
				// if (vy > 0)
				vy = -480;
			}
			else if (aEnemy->inside(x, y) || aEnemy->inside(x + dest.w, y) ||
					 aEnemy->inside(x, y + dest.h) || aEnemy->inside(x + dest.w, y + dest.h))
			{
				alive = false;
			}
		}
		if (playernumber == 2)
		{
			if (aEnemy->insideStompBox(x + (dest.w / 2), y - 1) || aEnemy->inside(x + (dest.w), y - 1) ||
				aEnemy->inside(x, y - 1))
				// if (vy < 0)
				vy = 480;
			else if (aEnemy->inside(x, y) || aEnemy->inside(x + dest.w, y) ||
					 aEnemy->inside(x, y + dest.h) || aEnemy->inside(x + dest.w, y + dest.h))
			{
				alive = false;
			}
		}
	}

	bool checkGrounded(vector<wall *> &walls)
	{
		if (playernumber == 1)
		{
			for (auto aWall : walls)
			{
				if (aWall->inside(x + (dest.w / 2), y + dest.h + 1) || aWall->inside(x + (dest.w), y + dest.h + 1) ||
					aWall->inside(x, y + dest.h + 1))
					return true;

				else if (y + dest.h + 1 >= 243)
					return true;
			}
			return false;
		}
		else if (playernumber == 2)
		{
			for (auto aWall : walls)
			{
				if (aWall->inside(x + (dest.w / 2), y - 1) || aWall->inside(x + (dest.w), y - 1) ||
					aWall->inside(x, y - 1))
					return true;

				else if (y - 1 <= 243)
					return true;
			}
			return false;
		}
		else
			return false;
	}

	bool isIdle(int vx, int vy)
	{
		if (vx == 0 && vy == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool isAlive()
	{
		return alive;
	}

	void revive(int rx, int ry)
	{
		x = rx;
		y = ry;
		alive = true;
	}

	int getw()
	{
		return dest.w;
	}
	int geth()
	{
		return dest.h;
	}

	// void CheckSpikeCollision(spike *aSpike) // switch to bool
	// {
	// 	// if (aSpike->inside(x, y) || aSpike->inside(x, y + dest.h) ||
	// 	// 	aSpike->inside(x + dest.w, y) || aSpike->inside(x + dest.w, y + dest.h))
	// 	// cout << "collision" << endl;
	// 	// cout << aSpike->inside(140, 240);
	// }

	// bool alive, isalive, kill, revive
};
