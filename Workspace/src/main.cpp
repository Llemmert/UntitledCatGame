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

using namespace std;

class MyGame : public Game
{
	const int levelw = 1280;
	const int levelh = 960;
	SDL_Rect src, dest;
	SDL_Rect camera;
	vector<Cat *> cats;
	Animation a, a2, b, wallanim, e, s;
	vector<Animation> blackCatAnim, whiteCatAnim;
	vector<wall *> walls;
	vector<block *> blocks;
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
		for (int i = 0; i < 2; i++)
		{
			blackCatAnim.push_back(a);
			whiteCatAnim.push_back(a);
		}
		cout << blackCatAnim.size() << endl;
		int yaxis = 0;
		// -----------------------create black cat-----------------------------
		animations.push_back("Workspace/media/animB.txt");
		animations.push_back("Workspace/media/idleAnimB.txt");
		yaxis = 210;
		src.x = 0;
		src.y = 0;

		for (int i = 0; i < blackCatAnim.size(); i++)
		{
			cout << animations[i] << endl;
			blackCatAnim.at(i).read(media, animations[i]);
			SDL_QueryTexture(blackCatAnim.at(i).getTexture(), NULL, NULL, &src.w, &src.h);
		}

		// cat is 40 by 32 pixels
		cats.push_back(new Cat(ren, &blackCatAnim, &src, (w / 2) - 300, yaxis, 0, 400, 0, 1000, 1));
		cats[0]->setBound(0, 0, w, yaxis + 1); // index 0 is black cat/cat 1

		// ---------------------create white cat--------------------------
		animations.push_back("Workspace/media/animW.txt");
		animations.push_back("Workspace/media/idleAnimW.txt");
		yaxis = 243;
		src.x = 0;
		src.y = 0;

		for (int i = 0; i < 2; i++) // read animations and get textures
		{
			whiteCatAnim[i].read(media, animations[i + 2]); // index 3 is the white cat running animation
			SDL_QueryTexture(whiteCatAnim.at(i).getTexture(), NULL, NULL, &src.w, &src.h);
		}

		// cat is 40 by 32 pixels
		cats.push_back(new Cat(ren, &whiteCatAnim, &src, (w / 2) - 300, yaxis, 0, -400, 0, -1000, 2));
		cats[1]->setBound(0, yaxis - 1, w, h - 32); // index 1 is white cat/cat 2

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

		///
		blocks.push_back(new block(ren, &wallanim, &src, 50, 208, 0));
		for (int i = 0; i < blocks.size(); i++)
		{
			blocks[i]->setBound(0, w);
		}
		///

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
		SDL_RenderCopy(ren, b.getTexture(), &src, &src);

		spike1->update(dt);
		for (int i = 0; i < blocks.size(); i++)
			blocks[i]->update(dt);

		for (int i = 0; i < walls.size(); i++)
			walls[i]->update(dt);

		for (unsigned i = 0; i < cats.size(); i++)
		{
			if (cats[i]->isAlive())
				cats[i]->update(dt);
		}

		if (cats[0]->isAlive())
		{
			cats[0]->handleCollision(walls, dt);
			cats[0]->setGrounded(cats[0]->checkGrounded(walls));
			cats[0]->handleEnemyCollision(enemy1);
			cats[0]->handleBlockCollision(blocks, dt);
			// cats[0]->CheckSpikeCollision(spike1);
		}

		if (cats[1]->isAlive())
		{
			cats[1]->handleCollision(walls, dt);
			cats[1]->setGrounded(cats[1]->checkGrounded(walls));
			cats[1]->handleEnemyCollision(enemy2);
			cats[1]->handleBlockCollision(blocks, dt);
		}
		blocks[0]->handleCollision(walls, dt);

		enemy1->update(dt);
		enemy1->handleCollision(walls);

		enemy2->update(dt);
		enemy2->handleCollision(walls);

		// if (enemy1->inside(cats[0]->getx() + cats[0]->getw(), cats[0]->gety()) ||
		// 	enemy1->inside(cats[0]->getx() + cats[0]->getw(), cats[0]->gety() + cats[0]->geth()))
		// {
		// 	cout << "hi" << endl;
		// }

		// if (enemy1->inside(cats[0]->getx(), cats[0]->gety()))
		// {
		// 	// cout << "hi" << endl;
		// }

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
			case SDLK_r:
				if (!cats[0]->isAlive())
					cats[0]->revive(40, 40);
				if (!cats[1]->isAlive())
					cats[1]->revive(40, 400);
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
