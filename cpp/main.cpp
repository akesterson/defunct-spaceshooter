#include <libgame/libgame.h>
#include "main.h"
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <fstream>

// TODO:
//
//   - add level clear screens
//   - handle appearance of the boss dependent upon player score
//	 - get the game to start in fullscreen 
//
// THE PROJECTILE/CHARACTER SUBCLASSING IS FUCKED. THE ORDER OF THE 'damage' ELEMENT AND OTHER 
// ITEMS CAUSES UNEXPECTED RESULTS AS ITEMS SUCH AS PROJECTILES ARE TYPECAST AS A (Character *)
// AND PASSED AROUND THRU COLLISION FUNCTIONS. CHANGE THE ORDER THAT THE 'damage' ELEMENT IS DEFINED
// IN THE CHARACTER CLASS AND YOU'LL SEE THE LASERS START DOING WIERD SHIT AS A RESULT.

// keyed by difficulty level
// see generateEnemies() for how this is used
const int enemyWeights[][10] = {
    {1, 1, 1, 1, 1, 2, 2, 2, 3, 3},
    {1, 1, 1, 1, 2, 2, 2, 3, 3, 4},
    {1, 1, 1, 2, 2, 2, 3, 3, 3, 4},
    {1, 1, 2, 2, 2, 3, 3, 3, 4, 4} };

// keyed by difficulty level
// see generateEnemies() for how this is used
const int maxVisibleEnemies[][5] = {
    // {TYPE1, TYPE2, TYPE3, TYPE4, BOSS}
    {2, 1, 1, 1, 1},
    {2, 2, 1, 1, 1},
    {3, 2, 2, 1, 1},
    {3, 3, 2, 2, 1} };

// keyed by difficulty level
int curVisibleEnemies[][5] = {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0} };


int loadSprites()
{
	SpriteStrip *strip = NULL;
	Game &myGame = Game::NewSingleton();
	int failed = 0;

	//std::cerr  << "Looping from 0 to " << MAXSPRITES << " sprites...\n";
	for ( int i = 0; i < MAXSPRITES ; i++ ) {
		std::cerr .flush();
		//std::cerr  << "Trying to load sprite strip from (index " << i << ") file " << spriteStrips[i] << "\n";
		strip = myGame.newSpriteStrip(spriteStrips[i]);
		if ( strip == NULL ) {
			//std::cerr  << "Failed to allocate new sprite strip in memory.\n";   
			failed++;
			continue ;
		}
		if ( strip->loadFromFile(spriteStrips[i], spriteStats[i][0], spriteStats[i][1], (Vector){0, 0, 0}) != 0 ) {
			//std::cerr  << "Failed to load sprite strip from file " << spriteStrips[i] << "\n";
			delete strip;
			failed++;
			continue ;
		}
		//std::cerr  << "Added sprite strip " << spriteStrips[i] << "(" << spriteStats[i][0] << "x" << spriteStats[i][1] << ") to memory\n";
	}
	// now do the same for the backgrounds
	for ( int i = 0; i < MAXSCREENS ; i++ ) {
		std::cerr .flush();
		//std::cerr  << "Trying to load sprite strip from (index " << i << ") file " << screenBackgrounds[i] << "\n";
		strip = myGame.newSpriteStrip(screenBackgrounds[i]);
		if ( strip == NULL ) {
			//std::cerr  << "Failed to allocate new sprite strip in memory.\n";   
			failed++;
			continue ;
		}
		if ( strip->loadFromFile(screenBackgrounds[i], 0, 0, (Vector){0, 0, 0}) != 0 ) {
			//std::cerr  << "Failed to load sprite strip from file " << spriteStrips[i] << "\n";
			failed++;
			delete strip;
			continue ;
		}
		//std::cerr  << "Added sprite strip " << screenBackgrounds[i] << " (0x0) to memory\n";
	}
	//std::cerr << "returning with " << failed << " sprites failed to load.\n";

	// init the enemy animations
	myGame.newAnimation(ANIM_ENEMYFIGHTER1)->setStrip(myGame.getSpriteStrip(SPRITEFILE_ENEMYFIGHTER1), 0, 0, (Vector){0,0,0});
	myGame.newAnimation(ANIM_ENEMYFIGHTER2)->setStrip(myGame.getSpriteStrip(SPRITEFILE_ENEMYFIGHTER2), 0, 0, (Vector){0,0,0});
	myGame.newAnimation(ANIM_ENEMYFIGHTER3)->setStrip(myGame.getSpriteStrip(SPRITEFILE_ENEMYFIGHTER3), 0, 0, (Vector){0,0,0});
	myGame.newAnimation(ANIM_ENEMYFIGHTER4)->setStrip(myGame.getSpriteStrip(SPRITEFILE_ENEMYFIGHTER4), 0, 0, (Vector){0,0,0});
	myGame.newAnimation(ANIM_ENEMYDESTROYER1)->setStrip(myGame.getSpriteStrip(SPRITEFILE_ENEMYDESTROYER1), 0, 0, (Vector){0,0,0});
	myGame.newAnimation(ANIM_ENEMYDESTROYER2)->setStrip(myGame.getSpriteStrip(SPRITEFILE_ENEMYDESTROYER2), 0, 0, (Vector){0,0,0});
	myGame.newAnimation(ANIM_ENEMYLASERSINGLE)->setStrip(myGame.getSpriteStrip(SPRITEFILE_ENEMYLASER1), 0, 0, (Vector){0, 0, 0});
	myGame.newAnimation(ANIM_ENEMYLASERDOUBLE)->setStrip(myGame.getSpriteStrip(SPRITEFILE_ENEMYLASER2), 0, 0, (Vector){0, 0, 0});
	// init misc animations
	myGame.newAnimation(ANIM_EXPLOSION)->setStrip(myGame.getSpriteStrip(SPRITEFILE_EXPLOSION), 12, 1, (Vector){0,0,0});
	myGame.newAnimation(ANIM_PWUP_BIGSHIP)->setStrip(myGame.getSpriteStrip(SPRITEFILE_POWERUP_SHIP), 0, 0, (Vector){0,0,0});
	myGame.newAnimation(ANIM_PWUP_ONEUP)->setStrip(myGame.getSpriteStrip(SPRITEFILE_POWERUP_ONEUP), 0, 0, (Vector){0,0,0});
    myGame.newAnimation(ANIM_PWUP_SHIELD)->setStrip(myGame.getSpriteStrip(SPRITEFILE_POWERUP_SHIELD), 0, 0, (Vector){0,0,0});
	myGame.newAnimation(ANIM_PWUP_SPREADSHOT)->setStrip(myGame.getSpriteStrip(SPRITEFILE_POWERUP_SPREAD), 0, 0, (Vector){0,0,0});
	myGame.newAnimation(ANIM_PLAYERSPREAD1)->setStrip(myGame.getSpriteStrip(SPRITEFILE_PLAYERSPREAD1), 0, 0, (Vector){0,0,0});
	myGame.newAnimation(ANIM_PLAYERSPREAD3)->setStrip(myGame.getSpriteStrip(SPRITEFILE_PLAYERSPREAD3), 0, 0, (Vector){0,0,0});
	return failed;
}

int initPlayers(Player *player1, Player *player2)
{
	Animation *shipAnim = NULL;
	Player *player;
	Game &game = Game::NewSingleton();
	GameOptions &opts = GameOptions::NewSingleton();
	if ( player1 == NULL || player2 == NULL ) {
		//std::cerr << "Player1 : " << player1 << " Player2 : " << player2 << "\n";
		delete shipAnim;
		return 1;
	}
	//std::cerr << "Setting sprite strip for player actors...\n";
	if ( (shipAnim = game.newAnimation(ANIM_PLAYERDOUBLE)) == NULL ) {
		return 1;
	}
	if ( shipAnim->setStrip(game.getSpriteStrip(SPRITEFILE_PLAYERSHIP2), 1) != 0 ) {
		//std::cerr << "Couldn't set sprite strip for player animations\n";
		return 1;
	}

	if ( (shipAnim = game.newAnimation(ANIM_PLAYERSINGLE)) == NULL ) {
		return 1;
	}
	if ( shipAnim->setStrip(game.getSpriteStrip(SPRITEFILE_PLAYERSHIP1), 1) != 0 ) {
		//std::cerr << "Couldn't set sprite strip for player animations\n";
		return 1;
	}

	player = player1;
	while ( player != NULL ) {
		player->addAnimation(game.getAnimation(ANIM_PLAYERSINGLE), STATE_DEFAULT);
		player->addState(STATE_NONE);
		player->lives = 0;
		player->score = 0;
		player->setUserType(ACTOR_TEAM_PLAYER | ACTORTYPE_PLAYER | ACTORTYPE_NODELETE);
		player->damage = 5;
		player->weaponDamage = opts.playerBaseDamage;
		player->projectileAnim = game.getAnimation(ANIM_PLAYERLASERSINGLE);
		player->projectileSound = SOUNDFILE_PLAYERLASERSHOT;
		player->deathSound = SOUNDFILE_EXPLOSION;
		if ( player == player2 )
			break;
		player = player2;
	}
	player1->addAnimation(game.getAnimation(ANIM_EXPLOSION), STATE_DYING);
	player2->addAnimation(game.getAnimation(ANIM_EXPLOSION), STATE_DYING);
	player1->addAnimation(NULL, STATE_DEAD);
	player2->addAnimation(NULL, STATE_DEAD);
	player1->setPosition((Vector){250, 500, 0});
	player2->setPosition((Vector){550, 500, 0});
	player1->setState(STATE_NONE);
	player2->setState(STATE_NONE);
	return 0;
}

int setupStaticDisplay(Display2D *staticDisplay, std::string bgfile)
{
	Actor *staticBackActor;
	Animation *staticBackAnim;
	Game &game = Game::NewSingleton();

	if ( staticDisplay == NULL ) {
		return 1;
	}
	staticBackActor = game.newActor(bgfile);
	staticBackAnim = game.newAnimation(bgfile);
	if ( staticBackActor == NULL ) {
		return 1;
	}
	if ( staticBackAnim == NULL ) {
		game.freeActor(bgfile);
		return 1;
	}

	//staticDisplay->initVideo((Vector){0,0,0}, 800, 600, 32, SDL_SWSURFACE); 
	staticDisplay->shareCanvas(&(Game::NewSingleton()));
	staticBackAnim->setStrip(game.getSpriteStrip(bgfile), 0);
	staticBackActor->addAnimation(staticBackAnim, STATE_NONE);
	staticBackActor->addState(STATE_NONE);
	staticDisplay->addActor(staticBackActor, 0);
	game.windows.push_back(staticDisplay);
	return 0;   
}

int setupGameDisplay(GameDisplay *gameDisplay)
{
	StarfieldBackground *gameBackActor;
	Game &game = Game::NewSingleton();

	if ( gameDisplay == NULL ) {
		return 1;
	}
	gameBackActor = new StarfieldBackground();
	if ( gameBackActor == NULL ) {
		return 1;
	}

	//std::cerr << "Scrolling background actor is at " << gameBackActor << "\n";

	//gameDisplay->initVideo((Vector){0,0,0}, 800, 600, 32, SDL_SWSURFACE); 
	gameDisplay->shareCanvas(&(Game::NewSingleton()));
	gameBackActor->setVelocity((Vector){0, 5, 0});
	gameBackActor->initStarfield(800, 600);
	gameBackActor->addState(STATE_NONE);
	gameBackActor->lockFPS(24);
	gameDisplay->addActor(gameBackActor, 0);
	game.windows.push_back(gameDisplay);
	game.addActor(gameBackActor, ACTOR_STARBACKGROUND);
	return 0;
}

int configureOptionsMenu(MenuDisplay *menuDisplay)
{
	Animation *menuAnimation;
	Animation *pointerAnim;
	Actor *menuBackground;
	char joyName[128];
	Game &myGame = Game::NewSingleton();
	GameOptions &opts = GameOptions::NewSingleton();
	int i = 0;

	if ( pointerAnim == NULL ) {
		return 1;
	}

	menuAnimation = myGame.newAnimation(ANIM_OPTIONMENU);
	pointerAnim = myGame.newAnimation(ANIM_OPTIONPTR);
	menuBackground = myGame.newActor(ACTOR_OPTMENUBACKGROUND);

	if ( menuBackground == NULL ) {
		myGame.freeAnimation(ANIM_OPTIONPTR);
		return 1;
	}
	if ( menuAnimation == NULL ) {
		myGame.freeAnimation(ANIM_OPTIONPTR);
		myGame.freeActor(ACTOR_OPTMENUBACKGROUND);
		return 1;
	}

	menuDisplay->initVideo((Vector){200,100,0}, 400, 400, 32, SDL_SWSURFACE); 
	menuAnimation->setStrip(myGame.getSpriteStrip(BACKGROUNDFILE_OPTIONS), 0);

	pointerAnim->setStrip(myGame.getSpriteStrip(SPRITEFILE_PLAYERSHIP3), 0);

	menuBackground->addAnimation(menuAnimation, STATE_NONE);
	menuBackground->setPosition((Vector){0, 0, 0});

	menuDisplay->addActor(menuBackground);
	menuDisplay->setPointer(pointerAnim);
	menuDisplay->setMenuOrigin((Vector){20, 20, 0} );
	menuDisplay->setFont("arial", 20, (SDL_Color){0x2E,0x8F,0x2C,0}, (SDL_Color){0,0,0,0});
	menuDisplay->setSpacing(20);

	menuDisplay->addOption(GAMEOPT_DIFFICULTY, 0, 0, 0, 0, NULL);
	menuDisplay->addSubOption(GAMEOPT_DIFFICULTY, DIFFICULTY_EASY);
	menuDisplay->addSubOption(GAMEOPT_DIFFICULTY, DIFFICULTY_NORMAL);
	menuDisplay->addSubOption(GAMEOPT_DIFFICULTY, DIFFICULTY_HARD);
	menuDisplay->addSubOption(GAMEOPT_DIFFICULTY, DIFFICULTY_IMMORTAL);
	menuDisplay->addOption(GAMEOPT_MUSIC, 0, 0, 0, 0, NULL);
	menuDisplay->addSubOption(GAMEOPT_MUSIC, "ON");
	menuDisplay->addSubOption(GAMEOPT_MUSIC, "OFF");
	menuDisplay->addOption(GAMEOPT_SOUNDEFFECTS, 0, 0, 0, 0, NULL);
	menuDisplay->addSubOption(GAMEOPT_SOUNDEFFECTS, "ON");
	menuDisplay->addSubOption(GAMEOPT_SOUNDEFFECTS, "OFF");
	menuDisplay->addOption(GAMEOPT_MUSICVOL, 0, 128, opts.musicVolume, 1, NULL);
	menuDisplay->addOption(GAMEOPT_SOUNDVOL, 0, 128, opts.soundVolume, 1, NULL);

	menuDisplay->addOption(GAMEOPT_P1CONTROL, 0, 0, 0, 0, NULL); 
	menuDisplay->addSubOption(GAMEOPT_P1CONTROL, CONTROLOPT_KEYBOARD);
	for (i = 0; i < SDL_NumJoysticks() ; i++ ) {
		memset((char *)&joyName, 128, 0);
		sprintf((char *)&joyName, "J%d %s", i, SDL_JoystickName(i));
		menuDisplay->addSubOption(GAMEOPT_P1CONTROL, std::string(joyName));
	}
	menuDisplay->addSubOption(GAMEOPT_P1CONTROL, CONTROLOPT_NONE);

	menuDisplay->addOption(GAMEOPT_P2CONTROL, 0, 0, 0, 0, NULL); 
	menuDisplay->addSubOption(GAMEOPT_P2CONTROL, CONTROLOPT_NONE);
	menuDisplay->addSubOption(GAMEOPT_P2CONTROL, CONTROLOPT_KEYBOARD);
	for (i = 0; i < SDL_NumJoysticks() ; i++ ) {
		memset((char *)&joyName, 128, 0);
		sprintf((char *)&joyName, "J%d %s", i, SDL_JoystickName(i));
		menuDisplay->addSubOption(GAMEOPT_P2CONTROL, std::string(joyName));
	}

	menuDisplay->setRelation(GAMEOPT_P2CONTROL, GAMEOPT_P1CONTROL);
	menuDisplay->setRelation(GAMEOPT_P1CONTROL, GAMEOPT_P2CONTROL);
	myGame.windows.push_back(menuDisplay);
	return 0;
}

int configureTitleMenu(MenuDisplay *menuDisplay)
{
	Animation *menuAnimation;
	Animation *pointerAnim;
	Actor *menuBackground;
	Game &myGame = Game::NewSingleton();

	if ( pointerAnim == NULL ) {
		return 1;
	}
	menuAnimation = myGame.newAnimation(ANIM_TITLEMENU);
	pointerAnim = myGame.newAnimation(ANIM_TITLEPTR);
	menuBackground = myGame.newActor(ACTOR_TITLEBACKGROUND);

	if ( menuBackground == NULL ) {
		delete pointerAnim;
		return 1;
	}
	if ( menuAnimation == NULL ) {
		delete pointerAnim;
		delete menuBackground;
		return 1;
	}

	//menuDisplay->initVideo((Vector){0,0,0}, 800, 600, 32, SDL_SWSURFACE); 
	menuDisplay->shareCanvas(&(Game::NewSingleton()));
	menuAnimation->setStrip(myGame.getSpriteStrip(BACKGROUNDFILE_MENU), 0);

	pointerAnim->setStrip(myGame.getSpriteStrip(SPRITEFILE_PLAYERSHIP3), 0);

	menuBackground->addAnimation(menuAnimation, STATE_NONE);
	menuBackground->setPosition((Vector){0, 0, 0});

	menuDisplay->addActor(menuBackground);
	menuDisplay->setPointer(pointerAnim);
	menuDisplay->setMenuOrigin((Vector){ 380, 150, 0} );
	menuDisplay->setFont("arial", 30, (SDL_Color){0x2E,0x8F,0x2C,0}, (SDL_Color){0,0,0,0});

	for (int i = 0; i < MAX_OPTS_TITLEMENU ; i++ ) {
		//std::cerr << "Adding title menu option " << titleMenuOpts[i] << "\n"; 
		menuDisplay->addOption(titleMenuOpts[i], 0, 0, 0, 0, NULL);
	}
	myGame.windows.push_back(menuDisplay);
	return 0;
}

void checkPlayerInput(SDL_Event *event, int playerNum, Player *player, GameDisplay *gameDisplay)
{
	GameOptions &opts = GameOptions::NewSingleton();
	// FIXME: The static allocation of Animation objects is fucked. Have to dynamic EVERY SINGLE ONE.
	// this one is GUARANTEED to be a loss at application exit.
	ControlMap *controlMap = &opts.controlMaps[playerNum-1];
	int joyNum = 0;
	char tmpBuf[512];

	memset((char *)&tmpBuf, 0x00, 512);

	SDL_JoystickUpdate();

	if ( controlMap == NULL || event == NULL ) {
		//std::cerr << "NULL CONTROL MAP!\n";
		return;
	}

	joyNum = SDL_JoystickIndex(controlMap->joystick);
	//std::cerr << "Player " << playerNum << " has control type " << controlMap->controlType << "\n";

	//sprintf((char *)&tmpBuf, "Current player state (pre): %d", player->getState() );
	//std::cerr << tmpBuf << "\n";

	if ( controlMap->controlType == CONTROL_TYPE_KEYBOARD ) {
		if ( event->type == SDL_KEYUP ) {
			if ( event->key.keysym.sym == controlMap->moveLeft ) {
				player->removeState(STATE_MOVELEFT);
			} else if ( event->key.keysym.sym == controlMap->moveRight ) {
				player->removeState(STATE_MOVERIGHT);
			} else if ( event->key.keysym.sym == controlMap->moveUp ) {
				player->removeState(STATE_MOVEUP);
			} else if ( event->key.keysym.sym == controlMap->moveDown ) {
				player->removeState(STATE_MOVEDOWN);
			} else if ( event->key.keysym.sym == controlMap->fire ) {
				player->fireProjectile();
			}
		} else if ( event->type == SDL_KEYDOWN ) {
			if ( event->key.keysym.sym == controlMap->moveLeft ) {
				player->addState(STATE_MOVELEFT);
			} else if ( event->key.keysym.sym == controlMap->moveRight ) {
				player->addState(STATE_MOVERIGHT);
			} else if ( event->key.keysym.sym == controlMap->moveUp ) {
				player->addState(STATE_MOVEUP);
			} else if ( event->key.keysym.sym == controlMap->moveDown ) {
				player->addState(STATE_MOVEDOWN);
			}
		}
	} else if ( controlMap->controlType == CONTROL_TYPE_JOYSTICK ) {
	  std::cerr << "Joystick event is " << (int)event->type << " (hat motion up is " << SDL_JOYHATMOTION << ")" << std::endl;
		if ( event->type == SDL_JOYHATMOTION ) {
			// OKAY you know what SCREW axis movements
			// They suck suck SUCK
			// just use the hat for this game
		  std::cerr << "Joystick event is for stick " << event->jhat.which << " but we're monitoring " << joyNum << std::endl;
			if ( (event->type == SDL_JOYHATMOTION && event->jhat.which != joyNum) ) {
				return;
			}
			std::cerr << "Joy hat value is : " << event->jhat.value << std::endl;
			// the reason we also check axes 5 and 6 here is because some gamepads (like the
			// logitech wingman this was developed with) map the joystick hat events to axes
			// 5 and 6
			if ( (event->jhat.value & controlMap->moveLeft) == controlMap->moveLeft) {
				std::cerr << "Player is moving left\n";
				player->addState(STATE_MOVELEFT);
			} else {
				player->removeState(STATE_MOVELEFT);
			} 
			if ( (event->jhat.value & controlMap->moveRight) == controlMap->moveRight) {
				std::cerr << "Player is moving right\n";
				player->addState(STATE_MOVERIGHT);
			} else {
				player->removeState(STATE_MOVERIGHT);
			}
			if ( (event->jhat.value & controlMap->moveDown) == controlMap->moveDown) {
				//std::cerr << "Player is moving up\n";
				player->addState(STATE_MOVEDOWN);
			} else {
				player->removeState(STATE_MOVEDOWN);
			}
			if ( (event->jhat.value & controlMap->moveUp) == controlMap->moveUp) {
				//std::cerr << "Player is moving down\n";
				player->addState(STATE_MOVEUP);
			} else {
				player->removeState(STATE_MOVEUP);

			}
		} else if ( event->type == SDL_JOYBUTTONUP ) {
			if ( event->jbutton.which == joyNum ) {
				//std::cerr << "FIRING LASER\n";
				player->fireProjectile();
			}
		}
	}
	//sprintf((char *)&tmpBuf, "Current player state (post): %d", player->getState() );
	//std::cerr << tmpBuf << "\n";

}

void processInput(MenuDisplay *menuDisplay, 
				  GameDisplay *gameDisplay, 
				  Display2D *creditDisplay,
				  Display2D *helpDisplay,
				  Display2D *failDisplay,
				  MenuDisplay *optionDisplay,
				  Player *player1,
				  Player *player2,
				  HUDDisplay *hudDisplay1,
				  HUDDisplay *hudDisplay2)
{
	Game &myGame = Game::NewSingleton();
	GameStatus &gamestatus = GameStatus::NewSingleton();
	SDL_Event nextEvent;

	if ( gamestatus.status == GAMESTATE_OPTIONSCREEN ) {
		if ( myGame.keyHeldDown(SDLK_LEFT) ) {
			nextEvent.type = SDL_KEYDOWN;
			nextEvent.key.keysym.sym = SDLK_LEFT;
			nextEvent.key.state = SDL_PRESSED;
			optionDisplay->handleEvent(&nextEvent);
		}
		if ( myGame.keyHeldDown(SDLK_RIGHT) ) {
			nextEvent.type = SDL_KEYDOWN;
			nextEvent.key.keysym.sym = SDLK_RIGHT;
			nextEvent.key.state = SDL_PRESSED;
			optionDisplay->handleEvent(&nextEvent);
		}
	}
	while ( SDL_PollEvent(&nextEvent) != 0 ) {
		//std::cerr << "Polling event from SDL queue ...\n";
		switch (nextEvent.type) {
		case SDL_KEYUP:
			//std::cerr << "Keyup event received\n";
			if ( nextEvent.key.state != SDL_RELEASED ) {
				break;
			}
			if ( nextEvent.key.keysym.sym == SDLK_ESCAPE ) {
				//std::cerr << gamestatus.status << "\n";
				if ( gamestatus.status == GAMESTATE_CREDITSCREEN || 
				     gamestatus.status == GAMESTATE_HELPSCREEN || 
				     gamestatus.status == GAMESTATE_FAILSCREEN ) {
					creditDisplay->setActive(0);
					helpDisplay->setActive(0);
					failDisplay->setActive(0);
					gamestatus.status = GAMESTATE_TITLEMENU;
					menuDisplay->setActive(1);
					myGame.haltMusic();
					myGame.playMusic(MUSICFILE_TITLESCREEN);
				} else if ( gamestatus.status == GAMESTATE_OPTIONSCREEN ) {
					optionDisplay->setActive(0);
					//gamestatus.status = GAMESTATE_TITLEMENU;
				} else if ( gamestatus.status == GAMESTATE_PLAYSINGLE || gamestatus.status == GAMESTATE_PLAYMULTI) {
					gamestatus.status = GAMESTATE_TITLEMENU;
					gameDisplay->setActive(0);
					menuDisplay->setActive(1);
					//std::cerr << "Clearing sounds...\n";
					myGame.cleanSounds(1);
					myGame.haltMusic();
					myGame.playMusic(MUSICFILE_TITLESCREEN);
				} else {
					gamestatus.running = 0;
				}
			} else {
				// processing keys per-gamestatus.status
				if ( gamestatus.status == GAMESTATE_TITLEMENU ) {
					menuDisplay->handleEvent(&nextEvent);
				} else if ( gamestatus.status == GAMESTATE_OPTIONSCREEN) {
					optionDisplay->handleEvent(&nextEvent);
				}
			}
		case SDL_KEYDOWN:
			if ( nextEvent.key.state != SDL_PRESSED ) {
				break;
			}
		}
		if ( gamestatus.status == GAMESTATE_PLAYSINGLE || gamestatus.status == GAMESTATE_PLAYMULTI ) {
			checkPlayerInput(&nextEvent, 1, player1, gameDisplay);
			checkPlayerInput(&nextEvent, 2, player2, gameDisplay);
		}
	}
}

void setControlMap(MenuOption *opt, Player *player, int playerNum)
{
	GameOptions &opts = GameOptions::NewSingleton();
	int joyNum; 
	char joyStr[2] = {'\0', '\0'};
	ControlMap *controlMap = &opts.controlMaps[playerNum-1];

	if ( opt == NULL ) {
		return;
	}
	//std::cerr << "Setting player " << playerNum << "control scheme to " << opt->getStringValue() << "\n";
	if ( opt->getStringValue() == CONTROLOPT_KEYBOARD ) {
		//std::cerr << "Setting up keyboard control for player " << playerNum << "\n";
		controlMap->controlType = CONTROL_TYPE_KEYBOARD;
		controlMap->joystick = NULL;
		controlMap->moveUp = CONTROL_KEY_MOVEUP;
		controlMap->moveDown = CONTROL_KEY_MOVEDOWN;
		controlMap->moveLeft = CONTROL_KEY_MOVELEFT;
		controlMap->moveRight = CONTROL_KEY_MOVERIGHT;
		controlMap->fire = CONTROL_KEY_FIRE;
		//std::cerr << "Control is ready\n";
	} else if ( opt->getStringValue() == CONTROLOPT_NONE ) {
		controlMap->controlType = CONTROL_TYPE_NONE;
		controlMap->joystick = NULL;
		controlMap->moveUp = 0;
		controlMap->moveDown = 0;
		controlMap->moveLeft = 0;
		controlMap->moveRight = 0;
		controlMap->fire = 0;
	} else if ( opt->getStringValue()[0] == 'J' ) {
	  //std::cerr << "Setting up joystick control for player " << playerNum << "\n";
		joyStr[0] = opt->getStringValue()[1];
		joyNum = atoi((char *)&joyStr);
		if ( joyNum < SDL_NumJoysticks() ) {
			if ( controlMap->joystick == NULL ) {
				controlMap->joystick = SDL_JoystickOpen(joyNum);
				if ( controlMap->joystick == NULL ) {
					// shit..
					std::cerr << "Failed opening joystick; re-setting this control to keyboard and hoping for the best.\n";
					opt->name == CONTROLOPT_KEYBOARD;
					setControlMap(opt, player, playerNum);
					return;
				}
				std::cerr << "Successfully opened joystick " << joyNum << " " << opt->getStringValue() << "\n";
				std::cerr << "Number of buttons " << SDL_JoystickNumButtons(controlMap->joystick) << "\n";
				std::cerr << "Number of axes " << SDL_JoystickNumAxes(controlMap->joystick) << "\n";
			}
			controlMap->controlType = CONTROL_TYPE_JOYSTICK;
			controlMap->moveUp = CONTROL_JS_MOVEUP;
			controlMap->moveDown = CONTROL_JS_MOVEDOWN;
			controlMap->moveLeft = CONTROL_JS_MOVELEFT;
			controlMap->moveRight = CONTROL_JS_MOVERIGHT;
			controlMap->fire = CONTROL_JS_FIRE;
			controlMap->lastFireButtonState = 0;
			//std::cerr << "Control is ready\n";
		}
	}
}

void processMenus(MenuDisplay *menuDisplay, 
				  GameDisplay *gameDisplay, 
				  Display2D *creditDisplay,
				  Display2D *helpDisplay,
				  Display2D *failDisplay,
				  MenuDisplay *optionDisplay,
				  Player *player1,
				  Player *player2,
				  HUDDisplay *hudDisplay1,
				  HUDDisplay *hudDisplay2)
{
	Game &myGame = Game::NewSingleton();
	GameStatus &gamestatus = GameStatus::NewSingleton();
	GameOptions &opts = GameOptions::NewSingleton();
	MenuOption *opt;

	// this is mostly used to handle title menu option handling, not much else goes on here
	if ( gamestatus.status == GAMESTATE_TITLEMENU ) {
		// --------------- title screen menu processing ------------------------
		if ( menuDisplay->isActive() == 0 ) {
			opt = menuDisplay->getOption();   
			//std::cerr << opt->getStringValue() << "\n";
			if ( opt != NULL && opt->getStringValue() == "1P START" ) {
				menuDisplay->setActive(0);
				gamestatus.status = GAMESTATE_PLAYSINGLE;
				gameDisplay->setActive(1);   
				if ( opts.playMusic == 1 ) {
				  myGame.haltMusic();
				  myGame.playMusic(MUSICFILE_GAMESCREEN);
				}
				gamestatus.nextLevel(player1, NULL, 0);
				player1->setPosition((Vector){350, 500, 0});
				player1->setState(STATE_NONE);
				gameDisplay->setHUDs(hudDisplay1, NULL);
				if ( opts.playSound == 1 )
				  myGame.playSound(SOUNDFILE_PLAYERSTART);
				player2->lives = -1;
			} else if ( opt != NULL && opt->getStringValue() == "MP START" ) {
				menuDisplay->setActive(0);
				gamestatus.status = GAMESTATE_PLAYMULTI;
				gameDisplay->setActive(1);   
				if ( opts.playMusic == 1 ) {
				  myGame.haltMusic();
				  myGame.playMusic(MUSICFILE_GAMESCREEN);
				} 
				if ( opts.playSound == 1 )
				  myGame.playSound(SOUNDFILE_PLAYERSTART);
				gameDisplay->setHUDs(hudDisplay1, hudDisplay2);
				gamestatus.nextLevel(player1, player2, 0);
			} else if ( opt != NULL && opt->getStringValue() == "CREDITS" ) {
				menuDisplay->setActive(0);
				gamestatus.status = GAMESTATE_CREDITSCREEN;
				creditDisplay->setActive(1);
				if ( opts.playMusic == 1 ) {
				  myGame.haltMusic();
				  myGame.playMusic(MUSICFILE_CREDITSCREEN);
				}
			} else if ( opt != NULL && opt->getStringValue() == "HELP" ) {
				menuDisplay->setActive(0);
				gamestatus.status = GAMESTATE_HELPSCREEN;
				helpDisplay->setActive(1);
			} else if ( opt != NULL && opt->getStringValue() == "OPTIONS" ) {
				menuDisplay->setActive(1);
				gamestatus.status = GAMESTATE_OPTIONSCREEN;
				optionDisplay->setActive(1);
			} else if ( opt != NULL && opt->getStringValue() == "QUIT" ) {
				//std::cerr << "Quitting\n";
				exit(0);
			}
		}
		// we do this here instead of below, in the post-opt screen processing,
		// so the default control maps are enforced even if they never enter the option screen
		setControlMap(optionDisplay->getOption(GAMEOPT_P1CONTROL), player1, 1);
		setControlMap(optionDisplay->getOption(GAMEOPT_P2CONTROL), player2, 2);
		// ------------- OPTION SCREEN MENU PROCESSING -------------------------
	} else if ( gamestatus.status == GAMESTATE_OPTIONSCREEN ) {
		if ( optionDisplay->isActive() == 0 ) {
			//std::cerr << "Trying to update options\n";
			gamestatus.status = GAMESTATE_TITLEMENU;
			// set music/sound/difficulty options
			opt = optionDisplay->getOption(GAMEOPT_MUSIC);
			if ( opt->getStringValue() == "OFF" ) {
				myGame.haltMusic();
				opts.playMusic = 0;
			} else {
				if ( opts.playMusic == 0 ) {
				  myGame.playMusic(MUSICFILE_TITLESCREEN);
				}
				opts.playMusic = 1;
			}
			opt = optionDisplay->getOption(GAMEOPT_SOUNDEFFECTS);
			if ( opt->getStringValue() == "Off" ) {
				opts.playSound = 0;
			} else {
				opts.playSound = 1;
			}
			opt = optionDisplay->getOption(GAMEOPT_DIFFICULTY);
			if ( opt->getStringValue() == DIFFICULTY_EASY ) {
				opts.difficultyLevel = 1;
			} else if ( opt->getStringValue() == DIFFICULTY_NORMAL ) {
				opts.difficultyLevel = 2;
			} else if ( opt->getStringValue() == DIFFICULTY_HARD ) {
				opts.difficultyLevel = 3;
			} else if ( opt->getStringValue() == DIFFICULTY_IMMORTAL ) {
				opts.difficultyLevel = 4;
			}
			opt = optionDisplay->getOption(GAMEOPT_MUSICVOL);
			myGame.setMusicVolume(opt->getIntValue());
			opt = optionDisplay->getOption(GAMEOPT_SOUNDVOL);
			myGame.setSoundVolume(opt->getIntValue());
		}
	}
}

void generateEnemies(GameDisplay *display, Player *player1, Player *player2)
{
	GameOptions &opts = GameOptions::NewSingleton();
	GameStatus &status = GameStatus::NewSingleton();
	Character *enemy = NULL;
	Game &game = Game::NewSingleton();
	char enemyName[64];
	int enemyType = enemyWeights[opts.difficultyLevel-1][(rand()%10)+1];
	//static Animation *enemyAnim = NULL;

	if ( status.status != GAMESTATE_PLAYSINGLE && status.status != GAMESTATE_PLAYMULTI )
		return;
	if ( ((rand()%100)+1 < status.enemySpawnChance) && 
		 ( SDL_GetTicks() - status.enemySpawnInterval) > status.enemyLastSpawned ) {
		status.enemyLastSpawned = SDL_GetTicks();
		enemy = new Character();
		sprintf((char *)&enemyName, "ENEMY FIGHTER %d", enemyType);
		enemy->addAnimation(game.getAnimation(std::string((char *)&enemyName)), STATE_DEFAULT);
		enemy->addAnimation(game.getAnimation(ANIM_EXPLOSION), STATE_DYING);
		enemy->addAnimation(NULL, STATE_DEAD);
		enemy->setPosition((Vector){100+(rand()%600), 0, 0});
		if ( enemyType == 4 ) {
			enemy->setVelocity((Vector){status.enemyVelocity.y, status.enemyVelocity.y, 0});
			enemy->addState(STATE_MOVERIGHT);
		} else if ( enemyType == 3 ) {
			enemy->setVelocity((Vector){status.enemyVelocity.y, status.enemyVelocity.y/2, 0});
		} else
			enemy->setVelocity((Vector){0, status.enemyVelocity.y, 0});
		enemy->addState(STATE_MOVEDOWN);
		// the math below yields us one of the four ACTORTYPE_ENEMYx defines.
		enemy->setUserType(ACTOR_TEAM_ENEMY | ACTORTYPE_CHARACTER | (ENEMYTYPE_ENEMY1 << (enemyType-1)));
		std::cerr << "Creating new enemy with userType " << enemy->getUserType() << " enemy type " << (ENEMYTYPE_ENEMY1 << (enemyType - 1)) << " and animation \"" << (char *)&enemyName << "\"\n";
		enemy->damage = 50;
		enemy->weaponDamage = 20;
		enemy->health = 1;
		std::cerr << "Getting enemy animation...\n";
		if ( enemyType > 1 )
		  enemy->projectileAnim = game.getAnimation(ANIM_ENEMYLASERDOUBLE);
		else
		  enemy->projectileAnim = game.getAnimation(ANIM_ENEMYLASERSINGLE);
		enemy->projectileSound = SOUNDFILE_ENEMYLASERSHOT;
		enemy->deathSound = SOUNDFILE_EXPLOSION;
		enemy->score = 100 * enemyType;
		enemy->target = player1;
		std::cerr << "(SPAWN) curVisibleEnemies[" << opts.difficultyLevel-1 << "][" << enemyType-1 << "] = " << curVisibleEnemies[opts.difficultyLevel-1][enemyType-1] << "\n";
		display->addActor(enemy, LAYER_SPRITE1);
		display->colliders.push_back(enemy);
	}
	return;
}

int main(int argc, char *argv[])
{
	// -- menu stuff
	MenuDisplay menuDisplay;
	GameDisplay &gameDisplay = GameDisplay::NewSingleton();
	Display2D creditDisplay;
	Display2D helpDisplay;
	Display2D failDisplay;
	MenuDisplay optionDisplay;
	HUDDisplay hudDisplay1;
	HUDDisplay hudDisplay2;
	Actor *hudLivesActor; 
	Animation *hudLivesAnimation;
	//unsigned int lastTimer = SDL_GetTicks();

	// -- control stuff
	//SDL_Event nextEvent;

	// --- gamestate stuff
	GameOptions &opts = GameOptions::NewSingleton();
	GameStatus &gamestatus = GameStatus::NewSingleton();
	Game &myGame = Game::NewSingleton();
	std::vector<Character *> collidables;
	Player player1;
	Player player2;

	std::ofstream logfile;
	std::map<int, Display2D *> screenList;

	myGame.initSDL();
	myGame.initVideo(800, 600, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	srand(time(0));
	TTF_Init();

	FontRenderer &textengine = FontRenderer::NewSingleton();
	textengine.loadFont("fonts/arialbd.ttf", "arial", 30);
	textengine.loadFont("fonts/arial.ttf", "arial", 20);

	if ( loadSprites() > 0 ) {
		exit(1);
	}

	myGame.newAnimation(ANIM_PLAYERLASERSINGLE)->setStrip(myGame.getSpriteStrip(SPRITEFILE_PLAYERLASER1), 0, 0, (Vector){0,0,0}); 
	myGame.newAnimation(ANIM_PLAYERLASERDOUBLE)->setStrip(myGame.getSpriteStrip(SPRITEFILE_PLAYERLASER2), 0, 0, (Vector){0,0,0});

	if ( configureTitleMenu(&menuDisplay) == 1 )
		exit(1);
	if ( setupGameDisplay(&gameDisplay) == 1 )
		exit(1);
	if ( setupStaticDisplay(&creditDisplay, BACKGROUNDFILE_CREDITS) == 1 )
		exit(1);
	if ( setupStaticDisplay(&helpDisplay, BACKGROUNDFILE_HELP) == 1 )
		exit(1);
	if ( setupStaticDisplay(&failDisplay, BACKGROUNDFILE_FAIL) == 1 )
		exit(1);
	if ( configureOptionsMenu(&optionDisplay) == 1)
		exit(1);

	//gameDisplay.addActor(&player1, LAYER_SPRITE3);
	//gameDisplay.addActor(&player2, LAYER_SPRITE3);
	gameDisplay.setPlayers(&player1, &player2) ;
	gameDisplay.colliders.push_back(&player1);
	gameDisplay.colliders.push_back(&player2);
    
	hudLivesActor = myGame.newActor(ACTOR_HUDLIVES);
	hudLivesAnimation = myGame.newAnimation(ANIM_HUDLIVES);
	hudLivesAnimation->setStrip(myGame.getSpriteStrip(SPRITEFILE_PLAYERSHIP3), 0, 0, (Vector){0,0,0});
	hudLivesActor->addAnimation(hudLivesAnimation, STATE_NONE);
	hudLivesActor->addState(STATE_NONE);
	hudDisplay1.initVideo((Vector){0,0,0}, 120, 120, 32, SDL_SWSURFACE);
	hudDisplay1.setLivesActor(hudLivesActor);
	hudDisplay2.initVideo((Vector){800-120, 0, 0}, 120, 120, 32, SDL_SWSURFACE);
	hudDisplay2.setLivesActor(hudLivesActor);
	hudDisplay1.setTarget(&player1);
	hudDisplay2.setTarget(&player2);
	hudDisplay1.setText("PLAYER ONE");
	hudDisplay2.setText("PLAYER TWO");

	gamestatus.status = GAMESTATE_TITLEMENU;
	menuDisplay.setActive(1);
	gameDisplay.setActive(0);
	creditDisplay.setActive(0);
	failDisplay.setActive(0);
	helpDisplay.setActive(0);
	hudDisplay1.setActive(1);
	hudDisplay2.setActive(1);
	//myGame.playMusic(MUSICFILE_TITLESCREEN);

	if ( initPlayers(&player1, &player2) != 0 ) {
		//std::cerr << "I was unable to initialize the players. WTF?\n";
	}

	opts.musicVolume = 0;
	opts.soundVolume = 0;
	myGame.setMusicVolume(opts.musicVolume);
	myGame.setSoundVolume(opts.soundVolume);
	myGame.lockFPS(60);

	//SDL_WM_GrabInput(SDL_GRAB_ON);
	while ( gamestatus.running == 1 ) {
	  //opts.playSound = 0;
		SDL_PumpEvents();
		//std::cerr << "Menu display at " << &menuDisplay << " is active? " << menuDisplay.isActive() << "\n";
		//std::cerr << "Game display at " << &gameDisplay << " is active? " << gameDisplay.isActive() << "\n";
		//std::cerr << "Gamestate = " << GAMESTATE_TITLEMENU << " menuDisplay.isActive() " << menuDisplay.isActive() << "\n";
		//std::cerr << "Game time : " << SDL_GetTicks() << "\n";

		if ( player1.lives < 0 && player2.lives < 0 && gamestatus.status != GAMESTATE_FAILSCREEN) {
		  gamestatus.status = GAMESTATE_FAILSCREEN;
		  gameDisplay.setActive(0);
		  failDisplay.setActive(1);
		  //std::cerr << "Clearing sounds...\n";
		  myGame.cleanSounds(1);
		  myGame.haltMusic();
		  myGame.playMusic(MUSICFILE_FAILSCREEN);
		  player1.lives = player2.lives = 0;
		}	

		processInput(&menuDisplay, &gameDisplay, &creditDisplay, &helpDisplay, &failDisplay,
					 &optionDisplay, &player1, &player2, &hudDisplay1, &hudDisplay2);
		processMenus(&menuDisplay, &gameDisplay, &creditDisplay, &helpDisplay, &failDisplay,
					 &optionDisplay, &player1, &player2, &hudDisplay1, &hudDisplay2);

		// bottom of event loop
		gameDisplay.clampActor(&player1);
		gameDisplay.clampActor(&player2);

		generateEnemies(&gameDisplay, &player1, &player2);
		gameDisplay.runCollisions();
		myGame.update();
		myGame.finishFrame();
	}
	exit(0);
}
