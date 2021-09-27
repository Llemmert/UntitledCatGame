#include <iostream>
#include <SDL.h>
#include <vector>
#include <map>
#include "Exception.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"
#include "Cat.hpp"

using namespace std;

class MyGame:public Game
{	  
    SDL_Rect src;
    vector<Cat *> cats;
    Animation a,b;
	public:
	MyGame(int w=640,int h=480):Game("UntitledCat",w,h) 
	{
		vector <string> animations;
		int yaxis = 0;
		animations.push_back("media/animB.txt");
		animations.push_back("media/animW.txt");
    	for (int i=0;i<2;i++) 
		{ 
			if (i == 0) //top cat y position
			{
				yaxis = 210;
			}
			else //bottom cat y position
			{
				yaxis = 243;
			}
			a.read(media,animations[i]);
			src.x=0; src.y=0;
			SDL_QueryTexture(a.getTexture(), NULL, NULL, &src.w, &src.h);
			//cat is 40 by 32 pixels 
        	cats.push_back(new Cat(ren,&a,&src,w/2,yaxis,0,0,0,0));
        	cats[i]->setBound(0,0,w,h);
    	}
    	b.read(media,"media/background.txt");
    	src.x=0; src.y=0; src.w=640; src.h=480;
	}
	void update(double dt) 
	{
    	SDL_RenderClear(ren);
    	b.update(dt);
      
    	SDL_RenderCopy(ren, b.getTexture(), &src, &src);
    	for (unsigned i=0;i<cats.size();i++) 
        	cats[i]->update(dt);
    	SDL_RenderPresent(ren);
	}
	~MyGame() 
	{
	}
	void handleKeyDown(SDL_Event aKey) //hold down key handle
	{
		//cat one left and right (arrow keys)
		if (aKey.key.keysym.sym == SDLK_LEFT)
		{
			cats[0]->setVelocityX(-5);
		}
		else if (aKey.key.keysym.sym == SDLK_RIGHT)
		{
			cats[0]->setVelocityX(5);
		}
		//cat two left and right (a and d keys)
		else if (aKey.key.keysym.sym == SDLK_a)
		{
			cats[1]->setVelocityX(-5);
		}
		else if (aKey.key.keysym.sym == SDLK_d)
		{
			
			cats[1]->setVelocityX(5);
		}
	}
	
	void handleKeyUp (SDL_Event aKey) //release key handle
	{
		//cat one left and right (arrow keys)
		if (aKey.key.keysym.sym == SDLK_LEFT)
		{
			if (cats[0]->getVelocityX() < 0)
			{
				cats[0]->setVelocityX(0);
			}
		}
		else if (aKey.key.keysym.sym == SDLK_RIGHT)
		{
			if (cats[0]->getVelocityX() > 0)
			{
				cats[0]->setVelocityX(0);
			}
		}
		//cat two left and right (a and d keys)
		else if (aKey.key.keysym.sym == SDLK_a)
		{
			if (cats[1]->getVelocityX() < 0)
			{
				cats[1]->setVelocityX(0);
			}
		}
		else if (aKey.key.keysym.sym == SDLK_d)
		{
			if (cats[1]->getVelocityX() > 0)
			{
				cats[1]->setVelocityX(0);
			}
		}
	}
};

int main(int argc, char* argv[]) 
{
    try {
		MyGame g;
		g.run();
	} catch (Exception e) {
		cerr << e ;
	}
	return 0;
}
