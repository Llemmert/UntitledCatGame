#pragma once

#include <vector>
#include <fstream>
#include <math.h>
using namespace std;

class AnimationFrame
{
    SDL_Texture *frame;
    int millis;

public:
    void init(SDL_Texture *newFrame, int newMillis = 100)
    {
        frame = newFrame;
        millis = newMillis;
    }
    void read(MediaManager *media, ifstream &in)
    {
        string fname;
        in >> millis >> fname;
        frame = media->read("Workspace/media/" + fname + ".bmp");
        // cout << "reading " << fname << ".bmp" << endl;
    }
    int getMillis() { return millis; }
    SDL_Texture *getTexture() { return frame; }
};

class Animation
{
    vector<AnimationFrame *> frames;
    int totalTime;
    int currentTime;

public:
    Animation()
    {
        totalTime = 0;
        currentTime = 0;
    }
    void read(MediaManager *media, string filename)
    {
        int max;
        ifstream in;
        in.open(filename);
        in >> max;
        for (int i = 0; i < max; i++)
        {
            AnimationFrame *af = new AnimationFrame();
            af->read(media, in);
            add(af);
        }
        in.close();
    }
    void add(AnimationFrame *af)
    {
        frames.push_back(af);
        totalTime += af->getMillis();
    }
    void update(double dt)
    {
        currentTime += (int)(dt * 1000);
        // cout << "Total Time " << totalTime << endl;
        currentTime %= totalTime;
    }
    SDL_Texture *getTexture()
    {
        int checkTime = 0;
        int t = 0;
        for (t = 0; t < frames.size(); t++)
        {
            if (checkTime + frames[t]->getMillis() > currentTime)
                break;
            checkTime += frames[t]->getMillis();
        }
        if (t == frames.size())
            t = 0;
        return frames[t]->getTexture();
    }
    ~Animation()
    {
        for (auto f : frames)
            delete f;
    }
};