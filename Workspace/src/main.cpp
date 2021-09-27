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
    Animation a, a2, b;
	public:
	MyGame(int w=640,int h=480):Game("UntitledCat",w,h) 
	{
		vector <string> animations;
		int yaxis = 0;
    	for (int i=0;i<2;i++) //loops twice, once for each cat
		{ 
			if (i == 0) //top cat y position; i=0 in loop so this is the flag for cat1 initiation
			{
				animations.push_back("media/animB.txt");
				yaxis = 210;
				a.read(media, animations[i]);
				src.x=0; src.y=0;
				SDL_QueryTexture(a.getTexture(), NULL, NULL, &src.w, &src.h);
				//cat is 40 by 32 pixels 
        		cats.push_back(new Cat(ren, &a, &src, w/2, yaxis, 0, 0, 0, 0));
        		cats[i]->setBound(0, 0, w, h);
			}
			else //bottom cat y position; i!=0 in loop so this is the flag for cat2 initiation
			{
				animations.push_back("media/animW.txt");
				yaxis = 243;
				a2.read(media, animations[i]);
				src.x=0; src.y=0;
				SDL_QueryTexture(a.getTexture(), NULL, NULL, &src.w, &src.h);
				//cat is 40 by 32 pixels 
        		cats.push_back(new Cat(ren, &a2, &src, w/2, yaxis, 0, 0, 0, 0));
        		cats[i]->setBound(0, 0, w, h);
			}
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
