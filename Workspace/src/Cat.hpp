#pragma once

#include <vector>
#include <fstream>
using namespace std;

class AnimationFrame
{
	SDL_Texture *frame;
	int millis;
	public:
	void init(SDL_Texture *newFrame,int newMillis=100) 
	{
		frame=newFrame;
		millis=newMillis;
	}
	void read(MediaManager *media,ifstream &in) 
	{
		string fname;
		in >> millis >> fname;
		frame=media->read("media/"+fname+".bmp");
		//cout << "reading " << fname << ".bmp" << endl;
	}
	int getMillis() { return millis; }
	SDL_Texture *getTexture() { return frame; }
};

class Animation
{
	vector<AnimationFrame*> frames;
	int totalTime;
	int currentTime;
	public:
	Animation() 
	{ 
		totalTime=0;
		currentTime=0;
	}
	void read(MediaManager *media,string filename) 
	{
		int max;
		ifstream in;
		in.open(filename);
		in >> max;
		for (int i=0;i<max;i++) 
		{
			AnimationFrame *af=new AnimationFrame();
			af->read(media,in);
			add(af);
		}
		in.close();
	}
	void add(AnimationFrame *af) 
	{
		frames.push_back(af);
		totalTime+=af->getMillis(); 
	}
	void update(double dt) 
	{
		currentTime+=(int)(dt*1000.0);
		//cout << "Total Time " << totalTime << endl;
		currentTime%=totalTime;
	}
	SDL_Texture *getTexture() 
	{
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

class Cat 
{
	SDL_Renderer *ren;
	Animation  *a;
	SDL_Rect *src;
	SDL_Rect dest;
	double x,y,vx,vy;
	int minx,miny,maxx,maxy;
	public:
	Cat(SDL_Renderer *newRen, Animation *newA, SDL_Rect *newSrc,
        				double newx=0.0, double newy=0.0,
        				double newvx=0.0, double newvy=0.0,) 
	{
		//FYI i'm commenting out the y stuff since we're not dealing with jumping yet, so for now the y value can stay static
		src=newSrc;
		ren=newRen;
		a=newA;
		dest.w=src->w; //img width
    		dest.h=src->h; //img height
    		dest.x=newx; //starting x pos
    		dest.y=newy; //starting y pos
    		x=newx;
    		y=newy;
    		vx=newvx; //x velocity
    		//vy=newvy; //y velocity
    		setBound();
  	}
	void setBound(int newMinX=0, int newMinY=0, int newMaxX=0, int newMaxY=0) //bounds based on window size
	{
    	minx=newMinX; maxx=newMaxX;
    	miny=newMinY; maxy=newMaxY;
  	}
	void update(double dt) 
  	{
		if (maxx!=minx) //causes the cats to loop/reappear on the other side when they reach an edge
		{
			if (x<=minx) { x=maxx; }
			else if (x>=maxx) { x=minx; }
		}	
		x+=vx; 	//y+=vy*dt; 	update x and y velocities of cat
		dest.x=int(x);	//dest.y=(int)y; 	sets new destination of x and y
    		a->update(dt);	//animate! animate! animate! make leg move
    		SDL_RenderCopy(ren, a->getTexture(), src, &dest); //cat 
   	}
	void setVelocityX(double velocity)
  	{
		vx = velocity;
  	}
  	double getVelocityX()
  	{
		return vx;
  	}
};
