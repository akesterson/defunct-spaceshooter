#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <libgame/libgame.h>

#define STATE_SPREADSHOT	0x00200000
#define STATE_FIRING		0x00400000

class Character : public Actor
{
    unsigned int lastDirChangeTime; // used by the Wavy Enemy subtype (ACTORTYPE_ENEMY3)
    unsigned int lastFireTime;
public:
    Character();
    virtual void collide(Character *ptr);
    // ... don't move the order of the 'damage' element. It will break the Projectile class. 
    // (It makes senseif you look at it long enough. No, I didn't design it that way. Yes, I'm ashamed of it.
    // Yes, I'll fix it later.)
    int damage; // the damage dealt when something runs in to us
    
    Actor *target;
    Animation *projectileAnim;
    std::string projectileSound;
    std::string deathSound;

    int health;
    int weaponDamage; // the damage dealt when we use our weapon on something
    int score;
    virtual void fireProjectile(Vector velocity = (Vector){0,10,0}, int playSound = 1);
    virtual void update();
};

class Projectile : public Actor
{
public:
    // we don't use the health or weaponDamage items in the Character class,
    // but because of the way I handled characters damaging each other, this was
    // a simpler hack than redoing the classes properly.
    void collide(Character *ptr);
    int damage;
};

class Player : public Character
{
    Uint32 playerShieldTimer; // used to track the amount of time the player's shield has been fading
    Uint32 playerFailTimer; // timer for player fail
    Uint32 playerDieTimer; // timer for when the player dies
public:
    int lives;
    void collide(Character *ptr);
    void update();
};

#endif // __CHARACTER_H__
