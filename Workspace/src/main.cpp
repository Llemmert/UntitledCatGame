#include <iostream>
#include <SDL.h>
#include <vector>
#include <map>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Exception.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"
#include "Cat.hpp"
#include "Animation.hpp"
#include "Enemy.hpp"
#include "Spike.hpp"

//#include "Wall.hpp"

using namespace std;

class MyGame : public Game
{
	const int levelw = 1280;
	const int levelh = 960;
	SDL_Rect src, dest;
	SDL_Rect camera;
	vector<Cat *> cats;
	Animation a, a2, b, wallanim, e, s;
	vector<wall *> walls;
	enemy *enemy1, *enemy2;
	spike *spike1;

public:
	MyGame(int w = 840, int h = 480) : Game("UntitledCat", w, h)
	{
		Mix_Chunk *sound;
		Mix_Volume(-1, MIX_MAX_VOLUME / 2);
		sound = media->readWav("Workspace/media/Mystical_Fountain.wav");
		Mix_PlayChannel(0, sound, -1);

		vector<string> animations;
		int yaxis = 0;
		for (int i = 0; i < 2; i++) // loops twice, once for each cat
		{
			if (i == 0) // top cat y position; i=0 in loop so this is the flag for cat1 initiation
			{
				animations.push_back("Workspace/media/animB.txt");
				yaxis = 210;
				a.read(media, animations[i]);
				src.x = 0;
				src.y = 0;
				SDL_QueryTexture(a.getTexture(), NULL, NULL, &src.w, &src.h);

				// cat is 40 by 32 pixels
				cats.push_back(new Cat(ren, &a, &src, w / 2, yaxis, 0, 400, 0, 1000, 1));
				cats[i]->setBound(0, 0, w, yaxis + 1);
			}
			else // bottom cat y position; i!=0 in loop so this is the flag for cat2 initiation
			{
				animations.push_back("Workspace/media/animW.txt");

				yaxis = 243;
				a2.read(media, animations[i]);
				src.x = 0;
				src.y = 0;
				SDL_QueryTexture(a.getTexture(), NULL, NULL, &src.w, &src.h);
				// cat is 40 by 32 pixels
				cats.push_back(new Cat(ren, &a2, &src, w / 2, yaxis, 0, -400, 0, -1000, 2));
				cats[i]->setBound(0, yaxis - 1, w, h - 32);
			}
		}

		// src.x = 0;
		// src.y = 0;
		// src.w = 30;
		// src.h = 30;
		wallanim.read(media, "Workspace/media/animWall.txt");
		SDL_QueryTexture(wallanim.getTexture(), NULL, NULL, &src.w, &src.h);
		walls.push_back(new wall(ren, &wallanim, &src, 220, 250));
		walls.push_back(new wall(ren, &wallanim, &src, 170, 284));
		walls.push_back(new wall(ren, &wallanim, &src, 170, 252));
		walls.push_back(new wall(ren, &wallanim, &src, 170, 220));
		walls.push_back(new wall(ren, &wallanim, &src, 170, 188));
		walls.push_back(new wall(ren, &wallanim, &src, 650, 203));
		walls.push_back(new wall(ren, &wallanim, &src, 650, 243));

		e.read(media, "Workspace/media/animE.txt");
		SDL_QueryTexture(e.getTexture(), NULL, NULL, &src.w, &src.h);
		enemy1 = new enemy(ren, &e, &src, 480, 243 - 55, -50);
		enemy2 = new enemy(ren, &e, &src, 480, 243, -30);

		s.read(media, "Workspace/media/animSpike.txt");
		SDL_QueryTexture(s.getTexture(), NULL, NULL, &src.w, &src.h);
		spike1 = new spike(ren, &s, &src, 100, 243 - 32);

		b.read(media, "Workspace/media/background.txt");

		src.x = 0;
		src.y = 0;
		src.w = w;
		src.h = h;
	}
	void update(double dt)
	{
		SDL_RenderClear(ren);
		b.update(dt);
		// src.x = cats[0]->getx() - (w / 2);
		// dest.x = 0;

		// if (src.x < 0)
		// 	src.x = 0;
		// if ((src.x - (w / 2)) > 1800)
		// 	src.x = 1800 - (w / 2);
		SDL_RenderCopy(ren, b.getTexture(), &src, &src);

		spike1->update(dt);

		for (int i = 0; i < walls.size(); i++)
			walls[i]->update(dt);

		for (unsigned i = 0; i < cats.size(); i++)
			cats[i]->update(dt);

		cats[0]->handleCollision(walls, dt);
		cats[0]->setGrounded(cats[0]->checkGrounded(walls));
		cats[0]->handleEnemyCollision(enemy1);
		// cats[0]->CheckSpikeCollision(spike1);
		cats[1]->handleCollision(walls, dt);
		cats[1]->setGrounded(cats[1]->checkGrounded(walls));
		cats[1]->handleEnemyCollision(enemy2);

		enemy1->update(dt);
		enemy1->handleCollision(walls);

		enemy2->update(dt);
		enemy2->handleCollision(walls);

		if (enemy1->inside(cats[0]->getx(), cats[0]->gety()))
		{
			// cout << "hi" << endl;
		}

		//  drawText(&src, ren, "text", 50, 50, 0, {255, 255, 255});
		SDL_RenderPresent(ren);
	}
	~MyGame()
	{
	}
	void handleKeyDown(SDL_Event aKey) // hold down key handle
	{
		int cat0Ticks = SDL_GetTicks(), cat0NewTicks, cat1Ticks = SDL_GetTicks(), cat1NewTicks;
		if (aKey.type == SDL_KEYDOWN)
		{
			// cat one left and right (arrow keys)
			switch (aKey.key.keysym.sym)
			{
			case SDLK_LEFT:
				cats[0]->setVelocityX(-100);
				break;
			case SDLK_RIGHT:
				cats[0]->setVelocityX(100);
				break;
			case SDLK_UP:
				if (cats[0]->isGrounded())
				{
					cats[0]->setVelocityY(-400);
					cats[0]->setGrounded(false);
				}
				break;
			case SDLK_a:
				cats[1]->setVelocityX(-100);
				break;
			case SDLK_d:
				cats[1]->setVelocityX(100);
				break;
			case SDLK_s:
				if (cats[1]->isGrounded())
				{
					cats[1]->setVelocityY(400);
					cats[1]->setGrounded(false);
				}
				break;
			case SDLK_f:
				Mix_Chunk *meowsound;
				meowsound = media->readWav("Workspace/media/sounds/meow_sound.wav");
				cats[0]->setMeow(true);
				Mix_PlayChannel(1, meowsound, 0);
				break;
			case SDLK_KP_0:
				Mix_Chunk *meowsound2;
				meowsound2 = media->readWav("Workspace/media/sounds/meow_sound_angry.wav");
				cats[1]->setMeow(true);
				Mix_PlayChannel(2, meowsound2, 0);
				break;
			default:
				break;
			}
		}
	}

	void handleKeyUp(SDL_Event aKey) // release key handle
	{
		// cat one left and right (arrow keys)
		if (aKey.type == SDL_KEYUP)
		{
			switch (aKey.key.keysym.sym)
			{
			case SDLK_LEFT:
				if (cats[0]->getVelocityX() < 0)
				{
					cats[0]->setVelocityX(0);
				}
				break;
			case SDLK_RIGHT:
				if (cats[0]->getVelocityX() > 0)
				{
					cats[0]->setVelocityX(0);
				}
				break;
			case SDLK_a:
				if (cats[1]->getVelocityX() < 0)
				{
					cats[1]->setVelocityX(0);
				}
			case SDLK_d:
				if (cats[1]->getVelocityX() > 0)
				{
					cats[1]->setVelocityX(0);
				}
			}
		}
	}
};

int main(int argc, char *argv[])
{
	try
	{
		MyGame g;
		g.run();
	}
	catch (Exception e)
	{
		cerr << e;
	}
	return 0;
}
