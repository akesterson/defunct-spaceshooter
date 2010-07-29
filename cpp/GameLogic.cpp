#include "GameLogic.h"

ControlMap::ControlMap()
{
    this->controlType = CONTROL_TYPE_KEYBOARD;
    this->joystick = NULL;
    this->moveUp = CONTROL_KEY_MOVEUP;
    this->moveDown = CONTROL_KEY_MOVEDOWN;
    this->moveLeft = CONTROL_KEY_MOVELEFT;
    this->moveRight = CONTROL_KEY_MOVERIGHT;
    this->fire = CONTROL_KEY_FIRE;
    this->lastFireButtonState = 0;
}

GameOptions::GameOptions()
{
    this->enemyBaseShootChance = 2;
    this->difficultyLevel = 1;
    this->bossExplodeLength = 5000;
    this->playerBaseSpeed.x = 3;
    this->playerBaseSpeed.y = 3;
    this->playerBaseSpeed.z = 0;
    this->enemyBaseSpeed.x = 1;
    this->enemyBaseSpeed.y = 1;
    this->enemyBaseSpeed.z = 0;
    this->playerStartLives = 3;
    this->playerStartHealth = 100;
    this->playerBaseDamage = 5;
    this->playMusic = 0;
    this->playSound = 0;
    this->playerShieldTime = 5000;
    this->bossBaseTrip = 10000;
    this->playerFailTime = 5000;
    this->musicVolume = 80;
    this->soundVolume = 60;
    this->enemyBaseSpawnInterval = 1000;
    this->enemyBaseSpawnChance = 5;
}

GameStatus::GameStatus()
{
    this->enemyVelocity.x = 0;
    this->enemyVelocity.y = 0;
    this->enemyVelocity.z = 0;
    this->status = GAMESTATE_TITLEMENU;
    this->curLevel = 0;
    this->levelTimer = 0;
    this->levelScore = 0;
    this->levelPowerups = 0;
    this->levelLives = 0;
    this->bossOnScreen = 0;
    this->enemyShootChance = 2;
    this->weaponSpeed = 100;
    this->enemySpawnChance = 5;
    this->enemySpawnInterval = 1000;
    this->enemyLastSpawned = 0;
    this->powerDropChance = 100;
    this->bossExplodeTimer = 0;
    this->running = 1;
}

void GameStatus::nextLevel(Player *player1, Player *player2, int curLevel)
{
    GameOptions &opts = GameOptions::NewSingleton();
    Player *player = player1;
    if ( curLevel != -1 ) {
	this->curLevel = curLevel;
    }
    this->enemyShootChance = (opts.enemyBaseShootChance * (curLevel+1));
    this->enemyWeaponSpeed = (opts.enemyBaseWeaponSpeed * (curLevel+1)) + opts.difficultyLevel;
    this->enemySpawnChance = (opts.enemyBaseSpawnChance + (curLevel+1)) + opts.difficultyLevel;
    this->enemySpawnInterval = opts.enemyBaseSpawnInterval - (curLevel * -50);
    this->enemyVelocity.x = opts.enemyBaseSpeed.x + ((curLevel+1)/3) + opts.difficultyLevel; 
    this->enemyVelocity.y = opts.enemyBaseSpeed.y + ((curLevel+1)/3) + opts.difficultyLevel;
    this->enemyVelocity.z = opts.enemyBaseSpeed.z + ((curLevel+1)/3) + opts.difficultyLevel;
    this->powerDropChance = (curLevel+1);
    if ( player2 == NULL ) {
	this->levelScore = player1->score;
    } else {
	this->levelScore = (player1->score + player2->score)/2;
    }
    while ( player != NULL ) {
	if ( curLevel == 0 ) {
	    player->score = 0;
	    player->setVelocity((Vector){opts.playerBaseSpeed.x + opts.difficultyLevel, opts.playerBaseSpeed.y + opts.difficultyLevel, 0} );
	    player->health = opts.playerStartHealth;
	    player->lives = opts.playerStartLives;
	    this->enemySpawnChance *= opts.difficultyLevel;
	    this->enemyWeaponSpeed *= opts.difficultyLevel;
	    this->enemySpawnInterval /= opts.difficultyLevel;
	    this->enemyVelocity.y +=  opts.difficultyLevel;
	} else {
	    player->score += 100 * this->levelPowerups;
	    player->score -= ( 200 * this->levelLives);
	    player->score -= (unsigned int)(1000 * ((float)(SDL_GetTicks() - this->levelTimer) / 60000));
	    player->score += 10000 * this->curLevel;
	}
	if ( player == player2 ) {
	    break;
	}
	player = player2;
    }
    this->levelTimer = SDL_GetTicks();
    this->levelLives = 0;
    this->levelPowerups = 0;
    if ( curLevel != 0 )
      this->curLevel += 1;
    else
      this->curLevel = curLevel;
}
