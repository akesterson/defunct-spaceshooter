#ifndef __HUD_H__
#define __HUD_H__

#include "Character.h"

class HUDDisplay : public Display2D
{
protected:
    Player *target;
    Actor *livesActor;
    std::string text;
public:
    HUDDisplay();
    SDL_Surface *nextFrame();
    void setText(std::string text);
    void setTarget(Player *tgt);
    void setLivesActor(Actor *actor);
    void update(int logicOnly = 0);
};

#endif // __HUD_H__
