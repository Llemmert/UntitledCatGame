#pragma once
using namespace std;

class Game 
{
	protected:
	MediaManager *media;
	SDL_Window *window;
    	SDL_Renderer *ren;
    	int ticks; // ms ticks since start
    	int w,h;
	public:
	Game(string title,int newW=640,int newH=480) 
	{
		w=newW;
		h=newH;
		SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow(
          	title.c_str(),                  // window title
          	SDL_WINDOWPOS_UNDEFINED,           // initial x position
          	SDL_WINDOWPOS_UNDEFINED,           // initial y position
          	w,                               // width, in pixels
          	h,                               // height, in pixels
          	SDL_WINDOW_OPENGL                  // flags - see below
        	);
      		if (window==NULL) throw Exception("Could not create window: ");
      		ren=SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);
      		if (ren==NULL) throw Exception("Could not create renderer ");
      		media=new MediaManager(ren);
      		ticks=SDL_GetTicks();
    		}
	  void run() 
	  {
	  	int newTicks;
	  	while (1) //i know this is ugly im sorry it's like 3 am
    		{	
			newTicks=SDL_GetTicks();
        		SDL_Event e;
			//read event from user
        		if (SDL_PollEvent(&e))
			{
			//if red x was hit, close game
			if (e.type == SDL_QUIT)
			{
				break;
			}
			else
			{
				if (e.type == SDL_KEYDOWN) //key handle events
				{
					handleKeyDown(e);
				}
				if (e.type == SDL_KEYUP)
				{
					handleKeyUp(e);
				}
				update(double(newTicks-ticks)/1000.0);  // seconds of elapsed time
				ticks=newTicks;
			}
		}
      		//  SDL_Delay(100);
    		}
	}
	virtual void update(double dt/*s of elapsed time*/)=0;
	virtual void handleKeyDown(SDL_Event key)=0;
	virtual void handleKeyUp(SDL_Event key)=0;
	~Game() 
	{
		SDL_DestroyRenderer(ren);
      		SDL_DestroyWindow(window);
      		SDL_Quit();
	}
};

