#include <libgame/libgame.h>
#include "GameLogic.h"
#include "GameDisplay.h"
#include "Character.h"
#include "HUD.h"
#include "Common.h"

GameDisplay::GameDisplay()
{
	//Character::Character();
	this->player1 = player1;
	this->player2 = player2;
	this->hud1 = NULL;
	this->hud2 = NULL;
}

SDL_Surface *GameDisplay::nextFrame()
{
    	Game &theGame = Game::NewSingleton();
	SDL_Surface *frame = NULL;
	SDL_Rect destRect;
	Player *player = NULL;
	GameStatus &gamestatus = GameStatus::NewSingleton();
	Vector position;
	char fpsString[32];

	if ( this->active == 0 ) {
		return NULL;
	}

	Display2D::nextFrame();

	player = this->player1;
	for ( int i = 0 ; i < 2 ; i++ ) {
		//std::cerr << "Player is " << player << "\n";
		if ( player == NULL )
			continue;
		frame = player->nextFrame();
		if ( frame == NULL ) {
			continue;
		}
		position = player->getPosition();
		destRect.x = (Sint16)position.x;
		destRect.y = (Sint16)position.y;
		destRect.w = frame->w;
		destRect.h = frame->h;
		//std::cerr << "Rendering player\n";
		SDL_BlitSurface(frame, NULL, this->canvas, &destRect);
		frame = NULL;
		if ( gamestatus.status == GAMESTATE_PLAYMULTI) {
			//std::cerr << "Playing multiplayer..\n";
			player = this->player2;
		} else {
			//std::cerr << "Playing single player..\n";
			player = NULL;
		}
	}
	//std::cerr << "Done rendering players\n";
	if ( this->hud1 != NULL ) {
		this->hud1->update();
		frame = this->hud1->nextFrame();
		if ( frame != NULL ) {
			position = this->hud1->getPosition();
			destRect.x = (Sint16)position.x;
			destRect.y = (Sint16)position.y;
			destRect.w = frame->w;
			destRect.h = frame->h;
			SDL_BlitSurface(frame, NULL, this->canvas, &destRect);
		}
	}
	if ( this->hud2 != NULL ) {
		this->hud2->update();
		frame = this->hud2->nextFrame();
		if ( frame != NULL ) {
			position = this->hud2->getPosition();
			destRect.x = (Sint16)position.x;
			destRect.y = (Sint16)position.y;
			destRect.w = frame->w;
			destRect.h = frame->h;
			SDL_BlitSurface(frame, NULL, this->canvas, &destRect);
		}
	}


	FontRenderer &textEngine = FontRenderer::NewSingleton();
	memset((char *)&fpsString, 0x00, 32);
	sprintf((char *)&fpsString, "%d FPS", theGame.realfps);
	textEngine.setColor((SDL_Color){0x2E,0x8F,0x2C,0}, (SDL_Color){0,0,0,0}, 1);
	textEngine.renderString(std::string((char *)&fpsString), this->canvas, "arial", (Vector){150,20,0}, 20); 

	return this->canvas;
}

void GameDisplay::update(int logicOnly)
{
	ActorList::iterator actor;
	ActorList::iterator oldActor; 
	//GameOptions &opts = GameOptions::NewSingleton();
	Actor *realActor;
	std::vector<Actor *>::iterator actorIter;
	Vector position;
	
	if ( this->active == 0 ) {
		return;
	}

	
	Display2D::update(logicOnly);

	if ( this->player1 != NULL )
		this->player1->update();
	if ( this->player2 != NULL )
		this->player2->update();

	oldActor = this->colliders.begin();
	while ( oldActor != this->colliders.end() ) {
		//std::cerr << "COLLIDER " << (*oldActor) << " has state " << (*oldActor)->getState() << " and current animation " << (*oldActor)->getAnimation((*oldActor)->getState()) << "\n";
		position = (*oldActor)->getPosition();
		if ( (*oldActor)->hasState(STATE_DYING) || 
			 (*oldActor)->hasState(STATE_DEAD) ||
			 ((position.x > this->canvas->w ||
			   position.y > this->canvas->h ||
			   position.x < 0 ||
			   position.y < 0 )) ) {
			if ( ((*oldActor)->getUserType() & ACTORTYPE_PLAYER) == ACTORTYPE_PLAYER ) {
				oldActor++;
				continue;
			}
			std::cerr << "ERASING FROM COLLIDERS " << (*oldActor) << " userType " << (*oldActor)->getUserType() << " state " << (*oldActor)->getState() << " position (" << position.x << "x" << position.y << ")\n";
			actor = oldActor;
			//while ( oldActor == actor ) {
			  // I've seen this a few times ... don't ask me how ...
			  //std::cerr << "actor " << (*oldActor) << " is still trying to delete from colliders list\n";
			  oldActor = this->colliders.erase(oldActor);
			  //}
			continue;
		}
		oldActor++;
	}
	// remove actors that are out of screen bounds
	for ( int i = 0; i < MAX_LAYERS ; i++ ) {
		actor = this->layers[i].begin();
		while ( actor != this->layers[i].end() ) {
			position = (*actor)->getPosition();
			realActor = (*actor);
			if ( realActor->hasState(STATE_DEAD) ||
				 (position.x > this->canvas->w ||
				  position.y > this->canvas->h ||
				  position.x < 0 ||
				  position.y < 0 )) {
				if ( realActor == this->player1 || realActor == this->player2 ) {
					actor++;
					continue;
				}
				actor = this->layers[i].erase(actor);
				if ( this->removeActor(realActor) != 0) {
				    std::cerr << "Couldn't remove actor from actor list\n";
				}
				std::cerr << "Trying to delete object " << realActor << " type " << realActor->getUserType() << " from memory\n";
				if ( (realActor->getUserType() & ACTORTYPE_CHARACTER) == ACTORTYPE_CHARACTER ) {
					std::cerr << "DELETING CHARACTER " << realActor << " ON LAYER " << i << " has userType " << realActor->getUserType() << " and state " << realActor->getState() << " and current animation " << realActor->getAnimation(realActor->getState()) << "\n";
					Character *tmp = (Character *)realActor;
					delete tmp;
				} else if ( (realActor->getUserType() & ACTORTYPE_PROJECTILE) == ACTORTYPE_PROJECTILE ) {
					std::cerr << "DELETING PROJECTILE " << realActor << " ON LAYER " << i << " has userType " << realActor->getUserType() << " and state " << realActor->getState() << " and current animation " << realActor->getAnimation(realActor->getState()) << "\n";
					Projectile *tmp = (Projectile *)realActor;
					delete tmp;
				} else if ( (realActor->getUserType() & ACTORTYPE_NORMAL) == ACTORTYPE_NORMAL ) {
				  std::cerr << "DELETING ACTOR " << realActor << " ON LAYER " << i << " has userType " << realActor->getUserType() << " and state " << realActor->getState() << " and current animation " << realActor->getAnimation(realActor->getState()) << "\n";
					delete realActor;
				}
				continue;
			}
			actor++;
		}
	}
}

void GameDisplay::clampActor(Actor *actor)
{
	Vector position;
	SDL_Surface *curFrame;
	SDL_Rect frameSize;

	if ( actor == NULL ) {
		return;
	}
	position = actor->getPosition();
	curFrame = actor->nextFrame();
	frameSize.x = 0;
	frameSize.y = 0;
	if ( curFrame ) {
		frameSize.w = curFrame->w;
		frameSize.h = curFrame->h;
	} else {
		frameSize.w = frameSize.h = 0;
	}

	if ( (position.x + frameSize.w) > this->canvas->w ) {
		position.x = (this->canvas->w - frameSize.w);
	} else if ( position.x < 0 ) {
		position.x = 1;
	}
	if ( (position.y + frameSize.h) > this->canvas->h ) {
		position.y = (this->canvas->h - frameSize.h);
	} else if ( position.y < 0 ) {
		position.y = 1;
	}

	actor->setPosition(position);
}

void GameDisplay::setPlayers(Player *player1, Player *player2)
{
	this->player1 = player1;
	this->player2 = player2;
}

void GameDisplay::setHUDs(HUDDisplay *hud1, HUDDisplay *hud2)
{
	this->hud1 = hud1;
	this->hud2 = hud2;
}

void GameDisplay::runCollisions()
{
	std::vector<Actor *>::iterator iter;
	std::vector<Actor *>::iterator iter2;
	Actor *actor1;
	Actor *actor2;
	iter = this->colliders.begin();
	iter2 = iter + 1;
	SDL_Surface *tmpFrame;
	SDL_Rect r1;
	SDL_Rect r2;
	//std::cerr << "Starting collisions...\n";
	while ( iter != this->colliders.end() ) {
		if ( (*iter) == NULL ) {
			iter++;
			continue;
		}
		tmpFrame = (*iter)->nextFrame();
		r1.x = (Sint16)(*iter)->getPosition().x;
		r1.y = (Sint16)(*iter)->getPosition().y;
		if ( tmpFrame == NULL ) {
			r1.w = 0;
			r1.h = 0;
		} else {
			r1.w = tmpFrame->w;
			r1.h = tmpFrame->h;
		}
		while ( iter2 != this->colliders.end()) {
			if ( (*iter2) == NULL ) {
				iter2++;
				continue;
			}
			std::cerr << "TRYING TO COLLIDE " << (*iter) << " AGAINST " << (*iter2) << "\n";
			tmpFrame = (*iter2)->nextFrame();
			if ( tmpFrame == NULL ) {
				r2.w = 0;
				r2.h = 0;
			} else {
				r2.w = tmpFrame->w;
				r2.h = tmpFrame->h;
			}
			r2.x = (Sint16)(*iter2)->getPosition().x;
			r2.y = (Sint16)(*iter2)->getPosition().y;

			if ( (((r1.x > r2.x && r1.x < r2.x + r2.w) || (r1.x + r1.w > r2.x && r1.x + r1.w < r2.x + r2.w)) && ((r1.y > r2.y && r1.y < r2.y + r2.h) || (r1.y + r1.h > r2.y && r1.y + r1.h < r2.y + r2.h))) || 
				 (((r2.x > r1.x && r2.x < r1.x + r1.w) || (r2.x + r2.w > r1.x && r2.x + r2.w < r1.x + r1.w)) && ((r2.y > r1.y && r2.y < r1.y + r1.h) || (r2.y + r2.h > r1.y && r2.y + r2.h < r1.y + r1.h))) ) {
				// collision
				//if ( (*iter)->userType & ACTOR_TEAMMASK != (*iter2)->userType & ACTOR_TEAMMASK )  {
				if ( ((*iter)->getUserType() & ACTOR_TEAMMASK) != ((*iter2)->getUserType() & ACTOR_TEAMMASK)) {
					actor1 = (*iter);
					actor2 = (*iter2);
					// for now just change every collided object's state to dying to show it's working
					while ( actor1 != NULL ) {
						std::cerr << "COLLIDING " << actor1 << " WITH " << actor2 << "\n";
						if ( (actor1->getUserType() & ACTORTYPE_PLAYER) == ACTORTYPE_PLAYER ) {
							((Character *)actor1)->collide((Character *)actor2);
						} else if ( (actor1->getUserType() & ACTORTYPE_CHARACTER) == ACTORTYPE_CHARACTER ) {
							((Character *)actor1)->collide((Character *)actor2);
						} else if ( (actor1->getUserType() & ACTORTYPE_PROJECTILE) == ACTORTYPE_PROJECTILE ) {
							((Projectile *)actor1)->collide((Character *)actor2);
						} else if ( (actor1->getUserType() & ACTORTYPE_NORMAL) == ACTORTYPE_NORMAL ) {
							actor1->collide(actor2);
						}
						if ( actor1 == (*iter2) ) {
							actor1 = NULL;
						} else {
							std::cerr << "Running second round of reverse collisions\n";
							actor1 = (*iter2);
							actor2 = (*iter);
						}
					}
				}
			}
			iter2++;
		}
		iter++;
		iter2 = iter + 1;
	}
}
