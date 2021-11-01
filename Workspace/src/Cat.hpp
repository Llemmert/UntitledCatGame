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
		frame=media->read("Workspace/media/"+fname+".bmp");
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
		currentTime+=(int)(dt*1000);
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
        				double newvx=0.0, double newvy=0.0) 
	{

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
    		vy=newvy; //y velocity
  	}
	void setBound(int newMinX, int newMinY, int newMaxX, int newMaxY) //bounds based on window size
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
		if(maxy!=miny){
			if(y<=miny) { y=miny;}
			if(y>=maxy) { y=maxy;}
		}
		if(vx!=0){ a->update(dt); }
		x+=vx*dt; 	
		y+=vy*dt; 	//update x and y velocities of cat
		dest.x=(int)x;	//sets new destination of x and y
		dest.y=(int)y; 	
    		//a->update(dt);	//animate! animate! animate! make leg move
    		SDL_RenderCopy(ren, a->getTexture(), src, &dest); //cat 
   	}
	//void animateWalk(double dt){
	//	a->update(dt);
	//	SDL_RenderCopy(ren,a->getTexture(), src, &dest);
	//}

	void setVelocityX(double velocity)
  	{
		vx = velocity;
  	}	
	void setVelocityY(double velocity){
		vy = velocity;
	}
  	double getVelocityX()
  	{
		return vx;
  	}
	double getVelocityY(){
		return vy;
	}
};
