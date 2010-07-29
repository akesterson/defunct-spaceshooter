#ifndef __MAIN_H__
#define __MAIN_H__

#include "Common.h"

#define MAXSCREENS	6
#define MAXSPRITES	21

const char *screenBackgrounds[] = {
    BACKGROUNDFILE_CREDITS,
    BACKGROUNDFILE_FAIL,
    BACKGROUNDFILE_HELP,
    BACKGROUNDFILE_PLAIN,
    BACKGROUNDFILE_MENU,
    BACKGROUNDFILE_OPTIONS };

int spriteStats[][2] = {
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{0,0},
	{110,110},  
	{0,0},
	{0,0}, 
	{0,0},
	{0,0} };

const char *spriteStrips[] = { 
	SPRITEFILE_ENEMYFIGHTER1,
	SPRITEFILE_ENEMYFIGHTER2,
	SPRITEFILE_ENEMYFIGHTER3,
	SPRITEFILE_ENEMYFIGHTER4,
	SPRITEFILE_ENEMYLASER1,
	SPRITEFILE_ENEMYLASER2,
	SPRITEFILE_PLAYERSHIP1,
	SPRITEFILE_PLAYERSHIP2,
	SPRITEFILE_PLAYERSHIP3,
	SPRITEFILE_PLAYERSHIELDHIT,
	SPRITEFILE_POWERUP_SHIELD,
	SPRITEFILE_POWERUP_SPREAD,
	SPRITEFILE_POWERUP_SHIP,
	SPRITEFILE_POWERUP_ONEUP,
	SPRITEFILE_ENEMYDESTROYER1,
	SPRITEFILE_ENEMYDESTROYER2,
	SPRITEFILE_EXPLOSION,
	SPRITEFILE_PLAYERLASER1,
	SPRITEFILE_PLAYERLASER2,
	SPRITEFILE_PLAYERSPREAD1,
	SPRITEFILE_PLAYERSPREAD3 };

#define MAX_OPTS_TITLEMENU 6

const char *titleMenuOpts[] = {
    "1P START",
    "MP START",
    "OPTIONS",
    "CREDITS",
    "HELP",
    "QUIT"
};

#include <libgame/libgame.h>
#include "Character.h"
#include "HUD.h"
#include "StarfieldBackground.h"
#include "GameLogic.h"
#include "GameDisplay.h"

#endif

