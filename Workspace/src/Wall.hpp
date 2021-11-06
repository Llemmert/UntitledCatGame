#include <SDL.h>
#include <iostream>
using namespace std;

class wall
{
private:
	SDL_Renderer *ren;
	Animation *a;
	SDL_Rect *src;
	SDL_Rect dest;
	double x, y, w, h;
	SDL_Point center;

public:
	wall(SDL_Renderer *newRen, Animation *newA, SDL_Rect *newSrc,
		 double newx = 0.0, double newy = 0.0 /*, double newW = 0, double newH = 0*/)
	{
		src = newSrc;
		ren = newRen;
		a = newA;
		dest.w = src->w; // img width
		dest.h = src->h; // img height
		dest.x = newx;	 // starting x pos
		dest.y = newy;	 // starting y pos
		// border.w = newW - 2;
		// border.h = newH - 2;
		// border.x = newx + 2;
		// border.y = newy;
		x = newx;
		y = newy;
		center = {dest.w / 2, dest.h / 2};
		w = dest.w;
		h = dest.h;
		// SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	}
	// collision(int Obx, int Oby)
	// {
	// 	if (Obx < x && Oby < y)
	// 		cout << " test" << endl;
	// }
	void update(double dt)
	{
		SDL_RenderCopy(ren, a->getTexture(), src, &dest);
		// SDL_RenderDrawRect(ren, &dest);
		// SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		// SDL_RenderFillRect(ren, &border);
		//  SDL_RenderDrawRect(ren, &dest);
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
	bool inside(int x, int y)
	{
		return (dest.x <= x && x <= dest.x + dest.w &&
				dest.y <= y && y <= dest.y + dest.h);
	}
};
