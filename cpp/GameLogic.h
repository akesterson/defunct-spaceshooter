#ifndef __GAMELOGIC_H__
#define __GAMELOGIC_H__

#include <SDL.h>
#include <libgame/libgame.h>
#include "Character.h"


#define GAMESTATE_TITLEMENU		0
#define GAMESTATE_PLAYSINGLE		1
#define GAMESTATE_PLAYMULTI		2
#define GAMESTATE_FAILSCREEN		3
#define GAMESTATE_HELPSCREEN		4
#define GAMESTATE_CREDITSCREEN		5
#define GAMESTATE_OPTIONSCREEN		6

#define CONTROL_TYPE_KEYBOARD		0
#define CONTROL_TYPE_JOYSTICK		1
#define CONTROL_TYPE_NONE		2

// --------- some defines for default control configurations
#define CONTROL_KEY_MOVELEFT	SDLK_LEFT
#define CONTROL_KEY_MOVERIGHT	SDLK_RIGHT
#define CONTROL_KEY_MOVEDOWN	SDLK_DOWN
#define CONTROL_KEY_MOVEUP	SDLK_UP
#define CONTROL_KEY_FIRE	SDLK_SPACE
// the CONTROL_JS_Px_MOVEx defines refer to state maps for the joystick hats
// ... controlling the spaceships via control stick currently isn't supported
#define CONTROL_JS_MOVELEFT		SDL_HAT_LEFT
#define CONTROL_JS_MOVERIGHT		SDL_HAT_RIGHT
#define CONTROL_JS_MOVEUP		SDL_HAT_UP
#define CONTROL_JS_MOVEDOWN		SDL_HAT_DOWN
// this refers to a button pressed on the joystick for firing
#define CONTROL_JS_FIRE			1

struct ControlMap
{
    int controlType; // one of the CONTROL_TYPE defines
    SDL_Joystick *joystick; // if CONTROL_JOYSTICK, this is the joystick opened for this control map
    int moveUp;
    int moveDown;
    int moveLeft;
    int moveRight;
    int fire;
    // used by the joystick monitoring stuff
    int lastFireButtonState;
    ControlMap();
};

class GameOptions
{
protected:
    GameOptions();
public:
    static GameOptions &NewSingleton()
    {
	static GameOptions singleton;
	return singleton;
    }
    int difficultyLevel; // integer from 1+ specifying the difficulty of the game
    Uint32 bossExplodeLength; // default 5000, specifies how long the boss should explode (and fade) for
    Vector playerBaseSpeed; // base movement speed for the player
    Vector enemyBaseSpeed; // base movement speed for enemies
    int enemyBaseShootChance; // base chance that an enemy will fire
    int enemyBaseWeaponSpeed; // base speed of the enemy's weapons
    int enemyBaseSpawnChance; // base spawn chance of the enemy ships
    int enemyBaseSpawnInterval; // base spawn interval for enemies
    int playerStartLives; // base number of lives for the player
    int playerStartHealth; // base health for the player
    int playerBaseDamage; // base weapon damage for the player's laser bolts
    int playMusic; // 1 = play background music, 0 = stop background music
    int playSound; // 1 = play sound effects, 0 = don't play sound effects
    int playerShieldTime; // defaults to 5000 (5 seconds), fade time for the player shield effect
    int bossBaseTrip; // determines at what interval (default 10,000) that the boss should appear
    int playerFailTime; // amount of time to wait after the player loses all lives (default 5000) before showing the fail screen
    int musicVolume; // 1 - 128
    int soundVolume; // 1 - 128
    ControlMap controlMaps[2]; // idx 0 = player 1, idx 1 = player 2
};

// options and values that can change during gameplay should go here (i.e. things that
// don't get set directly in the Options screen); everything else should go into the 
// GameOptions struct, above
class GameStatus
{
protected:
    GameStatus();
public:
    static GameStatus &NewSingleton()
    {
	static GameStatus singleton;
	return singleton;
    }
    int running; // is the game still running? (are we exiting)
    void nextLevel(Player *player1, Player *player2, int curLevel = -1);
    Vector enemyVelocity; // current speed of enemies on this level
    int enemyWeaponSpeed;
    int status; // game status flags
    int curLevel; // current level (1, 2, 3, etc)
    Uint32 levelTimer; // time currently spent on this level
    int levelScore; // score for the previous level (used when calculating score)
    int levelPowerups; // the number of powerups you got this level (for score)
    int levelLives; // the number of lives you used this level (for score)
    int bossOnScreen; // 0 = boss is not on screen, 1 = boss is on screen
    int enemyShootChance; // chance per update that an enemy will fire (currently stupid)
    Uint32 weaponSpeed; // player weapon shoot speed
    int enemySpawnChance; // chance per update that an enemy will spawn
    Uint32 enemySpawnInterval; // minimum time in milliseconds between enemy spawns
    Uint32 enemyLastSpawned; // last SDL_GetTicks when an enemy was spawned
    int powerDropChance; // integer from 0 to 100 specifying the chance that a powerup will spawn when an enemy dies
    Uint32 bossExplodeTimer; // used to track how long the boss has been exploding
   
};


#endif // __GAMELOGIC_H__
