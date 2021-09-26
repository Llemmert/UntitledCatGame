#pragma once

#include <vector>
#include <fstream>
using namespace std;

class AnimationFrame{
	SDL_Texture *frame;
	int millis;
	public:
	void init(SDL_Texture *newFrame,int newMillis=100) {
		frame=newFrame;
		millis=newMillis;
	}
	void read(MediaManager *media,ifstream &in) {
		string fname;
		in >> millis >> fname;
		frame=media->read("Workspace/media/"+fname+".bmp");
	}
	int getMillis() { return millis; }
	SDL_Texture *getTexture() { return frame; }
};

class Animation{
	vector<AnimationFrame*> frames;
	int totalTime;
	int currentTime;
	public:
	Animation() { 
	  totalTime=0;
	  currentTime=0;
	}
	void read(MediaManager *media,string filename) {
		int max;
		ifstream in;
		in.open(filename);
		in >> max;
		#if DEBUG
		cout << "Max " <<max << endl;
		#endif
		for (int i=0;i<max;i++) {
			AnimationFrame *af=new AnimationFrame();
			af->read(media,in);
			add(af);
		}
		in.close();
	}
	void add(AnimationFrame *af) {
		frames.push_back(af);
		totalTime+=af->getMillis(); 
	}
	void update(double dt) {
		currentTime+=(int)(dt*1000.0);
		cout << "Total Time " << totalTime << endl;
		currentTime%=totalTime;
	}
	SDL_Texture *getTexture() {
		int checkTime=0;
		int t=0;
		for (t=0;t<frames.size();t++) {
			if (checkTime+frames[t]->getMillis()>currentTime) 
			  break;
			checkTime+=frames[t]->getMillis();
		}
		if (t==frames.size()) t=0;
		return frames[t]->getTexture();
	}
	~Animation() {
		for (auto f:frames) 
		  delete f;
	}
};

class Particle {
  SDL_Renderer *ren;
  Animation  *a;
  SDL_Rect *src;
  SDL_Rect dest;
  double x,y,vx,vy,ax,ay;
  int minx,miny,maxx,maxy;
  public:
  Particle(SDL_Renderer *newRen,Animation *newA, 
           SDL_Rect *newSrc,
           double newx=0.0,double newy=0.0,
           double newvx=0.0,double newvy=0.0,
           double newax=0.0,double neway=0.0) {
	  src=newSrc;
	  ren=newRen;
	  a=newA;
	  dest.w=src->w;
      dest.h=src->h;
      dest.x=newx;
      dest.y=newy;
      x=newx;
      y=newy;
      vx=newvx; // px/s
      vy=newvy; // px/s
      ax=newax;
      ay=neway;  // px/s/s
      setBound();
  }
  void setBound(int newMinX=0,int newMinY=0,int newMaxX=0,int newMaxY=0) {
      minx=newMinX;
      maxx=newMaxX;
      miny=newMinY;
      maxy=newMaxY;
  }
  void update(double dt) {
	  if (maxx!=minx) {
		  if (x<=minx) { vx=-vx; x=minx; }
		  if (x>=maxx) { vx=-vx; x=maxx; }
	  }
	  if (maxy!=miny) {
		  if (y<=miny) { vy=-vy; y=miny; }
		  if (y>=maxy) { vy=-vy; y=maxy; }
      }
	  vx+=ax*dt; vy+=ay*dt; 
	  x+=vx*dt;  y+=vy*dt;
	  dest.x=(int)x;
      dest.y=(int)y;
      a->update(dt);
      SDL_RenderCopy(ren, a->getTexture(), src, &dest);
  }
};
