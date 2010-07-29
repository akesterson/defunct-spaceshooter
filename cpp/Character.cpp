#include <libgame/libgame.h>
#include "Character.h"
#include "GameLogic.h"
#include "Common.h"
#include "GameDisplay.h"

Character::Character()
{
    this->lastDirChangeTime = SDL_GetTicks();
}

void Character::update()
{
    GameStatus &status = GameStatus::NewSingleton();
    //GameOptions &opts = GameOptions::NewSingleton();
    GameDisplay &display = GameDisplay::NewSingleton();
    Game &game = Game::NewSingleton();
    //std::cerr << "Updating character with userType " << this->userType << "\n";
    if ( !this->hasState(STATE_DYING) && !this->hasState(STATE_DEAD) ) {
	if ( (this->userType & ENEMYTYPE_ENEMY2) == ENEMYTYPE_ENEMY2 ) {
	    //std::cerr << "WAVY ENEMY IS CHECKING TIMES\n";
	    if (SDL_GetTicks() > (this->lastDirChangeTime + 1000)) {
		if ( this->hasState(STATE_MOVERIGHT) ) {
		    this->removeState(STATE_MOVERIGHT);
		    this->addState(STATE_MOVELEFT);
		} else {
		    this->removeState(STATE_MOVELEFT);
		    this->addState(STATE_MOVERIGHT);
		}
		this->lastDirChangeTime = SDL_GetTicks();
	    } else {
		if ( this->velocity.x == 0 ) {
		    this->velocity.x = 3;
		    this->addState(STATE_MOVERIGHT);
		}
	    }
	} else if ((this->userType & ENEMYTYPE_ENEMY3) == ENEMYTYPE_ENEMY3 ) {
	    if ( this->velocity.x == 0 ) {
		this->velocity.x = this->velocity.y;
	    }
	    if ( this->target != NULL ) {
		if ( this->target->getPosition().x - this->position.x > -(this->velocity.x) || 
		     this->target->getPosition().x - this->position.x < this->velocity.x ) {
		    this->position.x = this->target->getPosition().x;
		} else if ( this->target->getPosition().x < this->position.x ) {
		    this->removeState(STATE_MOVERIGHT);
		    this->addState(STATE_MOVELEFT);
		} else if ( this->target->getPosition().x > this->position.x ) {
		    this->removeState(STATE_MOVELEFT);
		    this->addState(STATE_MOVERIGHT);
		}
		if ( this->target->getPosition().x == this->position.x ) {
		    this->velocity.y = status.enemyVelocity.y;
		} else {
		    this->velocity.y = status.enemyVelocity.y / 2;
		}
		if ( this->target->hasState(STATE_DEAD) || this->target->hasState(STATE_DYING) ) {
		    this->target = NULL;
		}
	    }
	} else if ( (this->userType & ENEMYTYPE_ENEMY4) == ENEMYTYPE_ENEMY4 ) {
	    if ( this->target != NULL ) {
		if ( this->position.x - this->velocity.x <= 0 ) {
		    this->removeState(STATE_MOVELEFT);
		    this->addState(STATE_MOVERIGHT);
		} else if ( this->position.x + this->velocity.x > 800 ) {
		    this->removeState(STATE_MOVERIGHT);
		    this->addState(STATE_MOVELEFT);
		}
		if ( this->position.y + this->velocity.y > 600 ) {
		    this->removeState(STATE_MOVEDOWN);
		    this->addState(STATE_MOVEUP);
		} else if ( this->position.y - this->velocity.y <= 0 ) {
		    this->removeState(STATE_MOVEUP);
		    this->addState(STATE_MOVEDOWN);
		}
	    }
	}
    }
    if ( this->health <= 0 && (!this->hasState(STATE_DYING))) {
	std::cerr << "Character " << this << " is now dying\n";
	game.playSound(this->deathSound);
	this->setState(STATE_DYING);
	if ( ((rand()%100)+1) <= status.powerDropChance ) {
	    Actor *pwup = game.newActor();
	    if ( pwup != NULL ) {
		int pwuptype = (rand()%3)+1;
		std::cerr << "Generating powerup type " << pwuptype << "\n";
		switch ( pwuptype ) {
		case 1:
		    pwup->setUserType(ACTOR_TEAM_POWERUP | POWERUP_SPREADSHOT);
		    pwup->addAnimation(game.getAnimation(ANIM_PWUP_SPREADSHOT), STATE_DEFAULT);
		    break;
		case 2:
		    pwup->setUserType(ACTOR_TEAM_POWERUP | POWERUP_ONEUP);
		    pwup->addAnimation(game.getAnimation(ANIM_PWUP_ONEUP), STATE_DEFAULT);
		    break;
		case 3:
		    pwup->setUserType(ACTOR_TEAM_POWERUP | POWERUP_SHIELDRECHARGE);
		    pwup->addAnimation(game.getAnimation(ANIM_PWUP_SHIELD), STATE_DEFAULT);
		    break;
		case 4:
		    pwup->setUserType(ACTOR_TEAM_POWERUP | POWERUP_BIGSHIP);
		    pwup->addAnimation(game.getAnimation(ANIM_PWUP_BIGSHIP), STATE_DEFAULT);
		    break;
		}
		pwup->setPosition((Vector){this->position.x, this->position.y, 0});
		pwup->setState(STATE_NONE);
		display.addActor(pwup, LAYER_SPRITE3);
		display.colliders.push_back(pwup);
	    }
        }
    }
    if ( this->state != STATE_DYING && ((this->userType & ACTOR_TEAM_ENEMY) == ACTOR_TEAM_ENEMY) ) {
	if ( ((rand()%100)+1) < status.enemyShootChance ) {
	    this->fireProjectile();
	}
    }
    Actor::update();
}

void Character::fireProjectile(Vector velocity, int playSound)
{
  // FIXME : Causes segfault on win32, modification of memory after it's freed
    // TODO : get a generator for the Projectile class
    SDL_Surface *laserFrame = NULL;
    SDL_Surface *playerFrame = NULL;
    Game &myGame = Game::NewSingleton();
    GameDisplay &gameDisplay = GameDisplay::NewSingleton();
    GameStatus &status = GameStatus::NewSingleton();
    Projectile *shotActor = NULL;
    if ( this->hasState(STATE_DEAD) || this->hasState(STATE_DYING) ) {
	return;
    }
    if ( this->hasState(STATE_SPREADSHOT) && (!this->hasState(STATE_FIRING))) {
	this->addState(STATE_FIRING);
	this->projectileAnim = myGame.getAnimation(ANIM_PLAYERSPREAD1);
	this->fireProjectile((Vector){-5, 10, 0}, 0);
	this->lastFireTime = 0;
	this->projectileAnim = myGame.getAnimation(ANIM_PLAYERSPREAD3);
	this->fireProjectile((Vector){5, 10, 0}, 0);
	this->lastFireTime = 0;
	this->projectileAnim = myGame.getAnimation(ANIM_PLAYERSPREAD2);
	this->fireProjectile((Vector){0, 10, 0}, 0);
	this->lastFireTime = SDL_GetTicks();
	myGame.playSound(this->projectileSound);
	this->removeState(STATE_FIRING);
    }
    if ( SDL_GetTicks() - this->lastFireTime > status.weaponSpeed && (this->projectileAnim != NULL) ) {
      //std::cerr << "TRYING TO FIRE\n";
	shotActor = new Projectile();
	if ( shotActor == NULL ) {
	    std::cerr << "firePlayerLaser BAILING OUT for NULL Projectile!\n";
	    return;
	}
	//std::cerr << "Got new projectile\n";
	this->lastFireTime = SDL_GetTicks();
	shotActor->addAnimation(this->projectileAnim, STATE_DEFAULT);
	shotActor->addAnimation(NULL, STATE_DYING);
	shotActor->addAnimation(NULL, STATE_DEAD);
	Vector playerPos = this->getPosition();
	laserFrame = shotActor->nextFrame();
	playerFrame = this->nextFrame();
	if ( laserFrame == NULL || playerFrame == NULL ) {
	    return;
	}
	shotActor->setPosition((Vector){(playerPos.x + ((playerFrame->w/2) - (laserFrame->w/2))), 
	    playerPos.y, playerPos.z});
	if ( velocity.x < 0 ) {
	    shotActor->addState(STATE_MOVELEFT);
	    velocity.x = -(velocity.x);
	} else if ( velocity.x > 0 )
	    shotActor->addState(STATE_MOVERIGHT);
	if ( (this->userType & ACTOR_TEAM_PLAYER) == ACTOR_TEAM_PLAYER )
	    shotActor->addState(STATE_MOVEUP);
	else if ( ((this->userType & ENEMYTYPE_ENEMY4 ) == ENEMYTYPE_ENEMY4) &&
		  (this->target != NULL) && (this->target->getPosition().y < this->position.y) ) {
	    shotActor->addState(STATE_MOVEUP);
	} else 
	    shotActor->addState(STATE_MOVEDOWN);
	shotActor->setVelocity((Vector){velocity.x, velocity.y, velocity.z});
	shotActor->setUserType((this->userType & ACTOR_TEAMMASK) | ACTORTYPE_PROJECTILE);
	shotActor->damage = this->weaponDamage;
	shotActor->parent = this;
	//std::cerr << "Setting player's laser bolt to do " << shotActor->damage << " points of damage\n";
	gameDisplay.addActor(shotActor, LAYER_SPRITE2);
	if ( playSound == 1) 
	  myGame.playSound(this->projectileSound);
	gameDisplay.colliders.push_back(shotActor);
	std::cerr << "Added new laser beam " << shotActor << " to colliders list.\n";
    }
}	

void Character::collide(Character *ptr)
{
    std::cerr << "CHARACTER [" << this << "] I AM COLLIDING WITH " << ptr << "\n";
    if ( ptr != NULL && 
	 (this->state != STATE_DYING && this->state != STATE_DEAD)  && 
	 (!ptr->hasState(STATE_DYING) && !ptr->hasState(STATE_DEAD))) {
	std::cerr << "I DID " << this->damage << " POINTS OF DAMAGE TO " << ptr << "\n";
	ptr->health -= this->damage;
    } else if ( ptr == NULL ) {
	std::cerr << " I CANNOT COLLIDE WITH NULL\n";
    } else {
	std::cerr << "I AM DEAD, I CAN'T COLLIDE WITH SHIT\n";
    }
}

void Projectile::collide(Character *ptr)
{
    //std::cerr << "PROJECTILE [" << this << "] I AM COLLIDING WITH " << ptr << "\n";
    if ( ptr != NULL && 
	 (this->state != STATE_DYING && this->state != STATE_DEAD) && 
	 (!ptr->hasState(STATE_DYING) && !ptr->hasState(STATE_DEAD)) &&
	 ((ptr->getUserType() & ACTOR_TEAM_POWERUP) != ACTOR_TEAM_POWERUP)  ) {
	//std::cerr << "I DID " << this->damage  << " POINTS OF DAMAGE TO " << ptr << "\n";
	ptr->health = ptr->health - this->damage;
	if ( (ptr->getUserType() & ACTOR_TEAMMASK) != (this->getUserType() & ACTOR_TEAMMASK) ) {
	    //std::cerr << "Projectile " << this << " with userType " << this->getUserType() << " is killing itself against an object with userType " << ptr->getUserType() << "!\n";
	    this->state = STATE_DEAD;
	}
	if ( this->parent != NULL && ((ptr->getUserType() & ACTORTYPE_PROJECTILE) != ACTORTYPE_PROJECTILE) ) {
	    ((Character *)this->parent)->score += ptr->score;
	}
    }
}

void Player::collide(Character *ptr)
{
    Game &game = Game::NewSingleton();
    Character::collide(ptr);
    if ( (ptr->getUserType() & ACTOR_TEAM_POWERUP) == ACTOR_TEAM_POWERUP ) {
	if ( ((ptr->getUserType() & POWERUP_ONEUP) == POWERUP_ONEUP) && this->lives < 4 ) {
	    this->lives += 1;
	} else if ( (ptr->getUserType() & POWERUP_BIGSHIP) == POWERUP_BIGSHIP ) {
	    this->addAnimation(game.getAnimation(ANIM_PLAYERDOUBLE), STATE_DEFAULT);
	    this->projectileAnim = game.getAnimation(ANIM_PLAYERLASERDOUBLE);
	} else if ( (ptr->getUserType() & POWERUP_SHIELDRECHARGE) == POWERUP_SHIELDRECHARGE ) {
	    this->health = 100;
	} else if ( (ptr->getUserType() & POWERUP_SPREADSHOT) == POWERUP_SPREADSHOT ) {
	    this->addState(STATE_SPREADSHOT);
	}
	ptr->setState(STATE_DEAD);
    }
}

void Player::update()
{
    GameOptions &opts = GameOptions::NewSingleton();
    //std::cerr << "During update player health is " << this->health << "\n";
    if ( this->playerDieTimer != 0 && this->state != STATE_DYING ) {
      // without this, input from the player will make us reappear on the screen
      this->state = STATE_DYING;
    }
    if ( this->health <= 0 && (this->state != STATE_DYING && this->state != STATE_DEAD)) {
	this->setState(STATE_DYING);
	this->playerDieTimer = SDL_GetTicks();
    } else if ( this->state == STATE_DYING && (SDL_GetTicks() - this->playerDieTimer < 5000) ) {
	return;
    } else if ( (this->state == STATE_DYING || this->state == STATE_DEAD)&& (SDL_GetTicks() - this->playerDieTimer > 5000) && (this->lives > -1) ) {
	this->setState(STATE_NONE);
	this->lives -= 1;
	this->playerDieTimer = 0;
	this->health = opts.playerStartHealth;
    }  
    Actor::update();
}

