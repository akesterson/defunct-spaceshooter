#include "StarfieldBackground.h"
#include <SDL_gfxPrimitives.h>
#include <libgame/libgame.h>

StarfieldBackground::StarfieldBackground()
{
	Actor::Actor();
	this->lastTime = 0;
	this->lockedFPS = 24;
}

StarfieldBackground::~StarfieldBackground()
{
	if ( this->canvas != NULL ) {
		delete this->canvas;
	}
}

void StarfieldBackground::lockFPS(unsigned int fps)
{
	this->lockedFPS = fps;
}

void StarfieldBackground::initStarfield(int w, int h)
{
    Game &myGame = Game::NewSingleton();
	// the entire purpose of this function is to ensure our temporary copy surface will
	// have the same bit value and flags as the original image
	Vector position;
	Vector velocity;
	SDL_Color color;
/*
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	int rmask = 0xff000000;
	int gmask = 0x00ff0000;
	int bmask = 0x0000ff00;
	//int amask = 0x000000ff;
#else
	int rmask = 0x000000ff;
	int gmask = 0x0000ff00;
	int bmask = 0x00ff0000;
	//int amask = 0xff000000;
#endif
*/
	//this->canvas = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, rmask, gmask, bmask, 0); 
	
	this->canvas = myGame.getCanvas();
	this->canvas->refcount += 1;
	
	if ( this->canvas == NULL ) {
		return;
	}
	for ( int i = 0; i < this->canvas->w/2 ; i++ ) {
		position.x = (rand()%this->canvas->w)+1;
		position.y = (rand()%this->canvas->h)+1;
		position.z = 0;
		velocity.x = 0;
		velocity.y = 0;
		if ( this->velocity.x > 0 ) {
			velocity.x = (rand()%(int)this->velocity.x)+1;
		}
		if ( this->velocity.y > 0 ) {
			velocity.y = (rand()%(int)this->velocity.y)+1;
		}
		velocity.z = 0;
		color.r = (rand()%128)+127;
		color.g = (rand()%128)+127;
		color.b = (rand()%128)+127;
		this->starpoints.push_back(position);
		this->starcolors.push_back(color);
		this->starvelocities.push_back(velocity);
	}
}
 
SDL_Surface *StarfieldBackground::nextFrame()
{
	std::vector<Vector>::iterator fieldIter;
	std::vector<SDL_Color>::iterator colorIter;
	//std::vector<Vector>::iterator velocityIter;
	
	/*if ( (SDL_GetTicks() - this->lastTime) < (unsigned int) (1000 / this->lockedFPS) ) {
		return;
	} else {
		this->lastTime = SDL_GetTicks();
	}*/

	//SDL_FillRect(this->canvas, NULL, 0x00000000);
	fieldIter = this->starpoints.begin();
	colorIter = this->starcolors.begin();
	//velocityIter = this->starvelocities.begin();
	while ( fieldIter != this->starpoints.end() ) {
		pixelRGBA(this->canvas, (Sint16)(*fieldIter).x, (Sint16)(*fieldIter).y, (*colorIter).r, (*colorIter).g, (*colorIter).b, 255);
		fieldIter++;
		colorIter++;

	}
	
	return this->canvas;
}

void StarfieldBackground::update()
{
	std::vector<Vector>::iterator fieldIter;
	//std::vector<SDL_Color>::iterator colorIter;
	std::vector<Vector>::iterator velocityIter;

	if ( (SDL_GetTicks() - this->lastTime) < (unsigned int) (1000 / this->lockedFPS) ) {
		return;
	} else {
		this->lastTime = SDL_GetTicks();
	}
	//SDL_FillRect(this->canvas, NULL, 0x00000000);
	fieldIter = this->starpoints.begin();
	//colorIter = this->starcolors.begin();
	velocityIter = this->starvelocities.begin();
	while ( fieldIter != this->starpoints.end() ) {
		(*fieldIter).x += (*velocityIter).x;
		(*fieldIter).y += (*velocityIter).y;
		(*fieldIter).z += (*velocityIter).z; 
		if ( (*fieldIter).x > this->canvas->w ) {
			(*fieldIter).x = 0;
		}
		if ( (*fieldIter).y > this->canvas->h ) {
			(*fieldIter).y = 0;
		}
		//pixelRGBA(this->canvas, (*fieldIter).x, (*fieldIter).y, (*colorIter).r, (*colorIter).g, (*colorIter).b, 255);
		fieldIter++;
		//colorIter++;
		velocityIter++;
	}
	
	return;
}
