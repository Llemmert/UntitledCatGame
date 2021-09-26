#pragma once

using namespace std;

class MediaManager{
	map<string,SDL_Texture *> images;
	SDL_Renderer *ren;
	public:
	MediaManager(SDL_Renderer *newRen) {
		ren=newRen;
	}
	SDL_Texture *read(string filename) {
	  SDL_Texture *bitmapTex;
	  if (images.find(filename)==images.end()) {
		cout << "Read " << filename << endl;
	    SDL_Surface *ob;
	    ob=SDL_LoadBMP(filename.c_str());
	    SDL_SetColorKey(ob,SDL_TRUE,0x0000ff);
        if (ob==NULL) throw Exception("Could not load "+filename);
	    bitmapTex = SDL_CreateTextureFromSurface(ren, ob);
        if (bitmapTex==NULL) throw Exception ("Could not create texture");
        SDL_FreeSurface(ob);
        images[filename]=bitmapTex;
	  }
	  return images[filename];
	}
	~MediaManager(){
	  for(auto i:images) SDL_DestroyTexture(i.second);
	}
};
