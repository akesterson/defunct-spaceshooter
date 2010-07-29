#include "HUD.h"
#include "Character.h"
#include <SDL_gfxPrimitives.h>

HUDDisplay::HUDDisplay()
{
    this->target = NULL;
    this->livesActor = NULL;
    this->text = "";
}

void HUDDisplay::setText(std::string text)
{
    this->text = text;
}

void HUDDisplay::setLivesActor(Actor *actor)
{
    this->livesActor = actor;
}

void HUDDisplay::setTarget(Player *tgt)
{
    this->target = tgt;
}

SDL_Surface *HUDDisplay::nextFrame()
{
    if ( this->active == 1 ) {
	return this->canvas;
    } else {
	return NULL;
    }
}

void HUDDisplay::update(int logicOnly)
{
    SDL_Surface *curFrame;
    char scoreVal[64];
    
    if ( this->active == 0 || logicOnly == 1 ) {
	return;
    }

    memset((char *)&scoreVal, 0, 64);

    sprintf((char *)&scoreVal, "%d", this->target->score);

    SDL_SetColorKey(this->canvas, SDL_SRCCOLORKEY, SDL_MapRGB(this->canvas->format, 0, 0, 0));
    SDL_FillRect(this->canvas, NULL, SDL_MapRGB(this->canvas->format, 0, 0, 0));
    //stringColor(this->canvas, 10, 10, (char *)&scoreVal, SDL_MapRGB(this->canvas->format, 0xB8, 0x13, 0x26));
    //std::cerr << "Player score is " << scoreVal << " (" << this->target->score << ")\n";
    stringRGBA(this->canvas, 0, 0, this->text.c_str(), 255, 255, 255, 255);
    stringRGBA(this->canvas, 0, 15, (char *)&scoreVal, 255, 0, 0, 255);
    rectangleRGBA(this->canvas, 0, 29, 101, 51, 255, 0, 0, 255);
    if ( target->health > 0 ) {
	boxRGBA(this->canvas, 1, 30, 0 + (target->health), 50, 0x68, 0x10, 0x20, 255);
    } else 
	boxRGBA(this->canvas, 1, 30, 0, 50, 0x68, 0x10, 0x20, 255);


    if ( this->livesActor == NULL ) {
	return;
    }
    curFrame = this->livesActor->nextFrame();
    if ( curFrame == NULL ) {
	return;
    }
    SDL_Rect destRect;
    destRect.x = 0;
    destRect.y = 60;
    destRect.w = curFrame->w;
    destRect.h = curFrame->h;
    for ( int i = 0; i < this->target->lives ; i++ ) {
	destRect.x = i*curFrame->w;
	SDL_BlitSurface(curFrame, NULL, this->canvas, &destRect);
    }
}
