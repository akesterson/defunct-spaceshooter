#ifndef __GAMEDISPLAY_H__
#define __GAMEDISPLAY_H__

#include "Character.h"
#include "HUD.h"

class GameDisplay : public Display2D
{
protected:
    Player *player1;
    Player *player2;
    HUDDisplay *hud1;
    HUDDisplay *hud2;
    GameDisplay();
public:
    static GameDisplay &NewSingleton() {
	static GameDisplay entity;
	return entity;
    }
    std::vector<Actor *> colliders;
    void update(int logicOnly = 0);
    SDL_Surface *nextFrame();
    void setHUDs(HUDDisplay *hud1, HUDDisplay *hud2);
    void setPlayers(Player *player1, Player *player2);
    void clampActor(Actor *actor);
    void runCollisions();
};

#endif // __GAMEDISPLAY_H__
