#ifndef __STARFIELDBACKGROUND_H__
#define __STARFIELDBACKGROUND_H__

#include <libgame/libgame.h>

class StarfieldBackground : public Actor
{
protected:
    SDL_Surface *canvas;
    unsigned int lockedFPS;
    unsigned int lastTime;
    std::vector<Vector> starpoints;
	std::vector<Vector> starvelocities;
    std::vector<SDL_Color> starcolors;
public:
    StarfieldBackground();
    ~StarfieldBackground();
    SDL_Surface *nextFrame();
    void lockFPS(unsigned int fps);
    void initStarfield(int w, int h);
    void update(); 
};


#endif // __STARFIELDBACKGROUND_H__
