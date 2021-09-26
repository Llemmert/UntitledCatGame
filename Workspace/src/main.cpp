// Example program:
// Using SDL2 to create an application window

#include <iostream>
#include <SDL.h>
#include <vector>
#include <map>
#include "Exception.hpp"
#include "MediaManager.hpp"
#include "Game.hpp"
#include "Particle.hpp"

using namespace std;

class MyGame:public Game{	  
    SDL_Rect src;
    vector<Particle *> particles;
    Animation a,b;
	public:
	MyGame(int w=640,int h=480):Game("Karl was here",w,h) {
      for (int i=0;i<100;i++) { 
		 int vx=rand()%500 - 250;
		 int vy=rand()%500 - 250;
		 a.read(media,"Workspace/media/anim1.txt");
	//	 SDL_Texture *bitmapTex=media->read("media/obsticle.bmp");
		 src.x=0; src.y=0;
		 SDL_QueryTexture(a.getTexture(), NULL, NULL, &src.w, &src.h);
         particles.push_back(new Particle(ren,&a,&src,w/2,h/2,vx,vy,0,50));
         particles[i]->setBound(0,0,w,h);
       }
       b.read(media,"Workspace/media/background.txt");
       src.x=0; src.y=0; src.w=640; src.h=480;
	}
	void update(double dt) {
      SDL_RenderClear(ren);
      b.update(dt);
      
      SDL_RenderCopy(ren, b.getTexture(), &src, &src);
      for (unsigned i=0;i<particles.size();i++) 
        particles[i]->update(dt);
      SDL_RenderPresent(ren);
	}
	~MyGame() {
	}
};

int main(int argc, char* argv[]) {
    try {
		MyGame g;
		g.run();
	} catch (Exception e) {
		cerr << e ;
	}
	return 0;
}
