#ifndef __HSA_COMMON_H__
#define __HSA_COMMON_H__

extern const int enemyWeights[][10];
extern const int maxVisibleEnemies[][5];
extern int curVisibleEnemies[][5];

// high nibble of user type is the "team" the actor is on
#define ACTOR_TEAMMASK		0xF0000000
#define ACTOR_TEAMTYPEMASK	0x0FF00000
#define ACTOR_PARENTMASK	0x000F0000
#define ACTOR_TYPEMASK		0x0000FFFF

#define ACTOR_TEAM_PLAYER	0x20000000
#define ACTOR_TEAM_POWERUP	0x40000000
#define ACTOR_TEAM_ENEMY	0x80000000

#define ACTORTYPE_NORMAL       	0x00000000
#define ACTORTYPE_NODELETE     	0x00000002
#define ACTORTYPE_PLAYER       	0x00000004
#define ACTORTYPE_CHARACTER    	0x00000008
#define ACTORTYPE_PROJECTILE   	0x00000010
#define ACTORTYPE_UNDEFINED    	0x00000020

#define POWERUP_SPREADSHOT	0x00000200
#define POWERUP_ONEUP		0x00000400
#define POWERUP_BIGSHIP		0x00000800
#define POWERUP_SHIELDRECHARGE	0x00001000

#define ENEMYTYPE_ENEMY1	0x00200000
#define ENEMYTYPE_ENEMY2	0x00400000
#define ENEMYTYPE_ENEMY3	0x00800000
#define ENEMYTYPE_ENEMY4	0x01000000
#define ENEMYTYPE_DESTROYER1	0x04000000
#define ENEMYTYPE_DESTROYER2	0x08000000

// ------- KEY CONSTANTS --------------------

#define ANIM_PLAYERSINGLE	"PLAYER SMALL SHIP"
#define ANIM_PLAYERDOUBLE	"PLAYER BIG SHIP"
#define ANIM_EXPLOSION		"EXPLOSION" 
#define ANIM_OPTIONMENU		"OPTION MENU"
#define ANIM_OPTIONPTR		"OPTION POINTER"
#define ANIM_TITLEMENU		"TITLE MENU"
#define ANIM_TITLEPTR		"TITLE POINTER"
#define ANIM_PLAYERLASERSINGLE	"SINGLE PLAYER LASER"
#define ANIM_PLAYERLASERDOUBLE  "DOUBLE PLAYER LASER"
#define ANIM_PLAYERSPREAD1	"PLAYER SPREAD LEFT"
#define ANIM_PLAYERSPREAD2	ANIM_PLAYERLASERSINGLE
#define ANIM_PLAYERSPREAD3	"PLAYER SPREAD RIGHT" 
#define ANIM_HUDLIVES		"HUD LIVES ANIM" 
#define ANIM_ENEMYFIGHTER1      "ENEMY FIGHTER 1"
#define ANIM_ENEMYFIGHTER2      "ENEMY FIGHTER 2"
#define ANIM_ENEMYFIGHTER3      "ENEMY FIGHTER 3"
#define ANIM_ENEMYFIGHTER4      "ENEMY FIGHTER 4"
#define ANIM_ENEMYDESTROYER1    "ENEMY DESTROYER 1"
#define ANIM_ENEMYDESTROYER2    "ENEMY DESTROYER 2"
#define ANIM_ENEMYLASERSINGLE	"ENEMY LASER SINGLE"
#define ANIM_ENEMYLASERDOUBLE   "ENEMY LASER DOUBLE"
#define ANIM_PWUP_BIGSHIP	"POWERUP BIG SHIP"
#define ANIM_PWUP_ONEUP		"POWERUP ONE UP"
#define ANIM_PWUP_SHIELD	"POWERUP SHIELD RECHARGE"
#define ANIM_PWUP_SPREADSHOT	"POWERUP SPREAD SHOT"

#define ACTOR_STARBACKGROUND	"STARFIELD BACKGROUND"
#define ACTOR_OPTMENUBACKGROUND	"MENU BACKGROUND"
#define ACTOR_TITLEBACKGROUND	"TITLE BACKGROUND"
#define ACTOR_HUDLIVES		"HUD LIVES ACTOR"

#define MUSICFILE_TITLESCREEN		"music/title theme - gradius 3 fire scramble.ogg"
#define MUSICFILE_GAMESCREEN		"music/background theme - carrieraces.ogg"
#define MUSICFILE_CREDITSCREEN		"music/credits - mobile suit gundam formula wars eternal wind.ogg"
#define MUSICFILE_FAILSCREEN		"music/game over - gundam wing endless duel game over.ogg"

#define SPRITEFILE_ENEMYFIGHTER1 	"sprites/Enemy Fighter 1.png"
#define SPRITEFILE_ENEMYFIGHTER2 	"sprites/Enemy Fighter 2.png"
#define SPRITEFILE_ENEMYFIGHTER3 	"sprites/Enemy Fighter 3.png"
#define SPRITEFILE_ENEMYFIGHTER4 	"sprites/Enemy Fighter 4.png"
#define SPRITEFILE_ENEMYLASER1		"sprites/Enemy Laser Single.png"
#define SPRITEFILE_ENEMYLASER2		"sprites/Enemy Laser Double.png"
#define SPRITEFILE_PLAYERSHIP1		"sprites/Player Ship 1.png"
#define SPRITEFILE_PLAYERSHIP2		"sprites/Player Ship 2.png"
#define SPRITEFILE_PLAYERSHIP3		"sprites/Player Ship Menu.png"
#define SPRITEFILE_PLAYERSHIELDHIT	"sprites/Player Shield Hit.png"
#define SPRITEFILE_POWERUP_SHIELD	"sprites/Powerup - Shield Recharge.png"
#define SPRITEFILE_POWERUP_SPREAD	"sprites/Powerup - Spreadshot.png"
#define SPRITEFILE_POWERUP_SHIP		"sprites/Powerup - Bigger Ship.png"
#define SPRITEFILE_POWERUP_ONEUP	"sprites/Powerup - One Up.png"
#define SPRITEFILE_ENEMYDESTROYER1	"sprites/Star Destroyer 1.png"
#define SPRITEFILE_ENEMYDESTROYER2	"sprites/Star Destroyer 2.png"
#define SPRITEFILE_EXPLOSION		"sprites/explosion.png"
#define SPRITEFILE_PLAYERLASER1		"sprites/Player Laser Single.png"
#define SPRITEFILE_PLAYERLASER2		"sprites/Player Laser Double.png"
#define SPRITEFILE_PLAYERSPREAD1	"sprites/Player Laser Spread -30deg.png"
#define SPRITEFILE_PLAYERSPREAD2	SPRITEFILE_PLAYERLASER1	
#define SPRITEFILE_PLAYERSPREAD3	"sprites/Player Laser Spread 30deg.png"

#define SOUNDFILE_PLAYERLASERSHOT 	"sounds/lasershot.wav"
#define SOUNDFILE_ENEMYLASERSHOT	"sounds/enemylasershot.wav"
#define SOUNDFILE_EXPLOSION		"sounds/EXPLODE.WAV"
#define SOUNDFILE_PLAYERSTART		"sounds/sci_fi_takeoff.wav"
#define SOUNDFILE_PLAYERHIT		"sounds/FORCEFI.wav"

#define BACKGROUNDFILE_CREDITS		"backgrounds/credits background - 800x600.png"
#define BACKGROUNDFILE_FAIL		"backgrounds/fail background - 800x600.png"
#define BACKGROUNDFILE_HELP		"backgrounds/help background - 800x600.png"
#define BACKGROUNDFILE_PLAIN		"backgrounds/Background-800x600.png"
#define BACKGROUNDFILE_MENU		"backgrounds/title screen - 800 x 600.png"
#define BACKGROUNDFILE_OPTIONS		"backgrounds/Options Background.png"

#define GAMEOPT_DIFFICULTY	"DIFFICULTY"
#define GAMEOPT_SOUNDEFFECTS	"SOUND FX"
#define GAMEOPT_MUSIC		"MUSIC"
#define GAMEOPT_MUSICVOL	"MUS. VOL."
#define GAMEOPT_SOUNDVOL	"SND. VOL." 
#define GAMEOPT_P1CONTROL	"P1 CONTROL"
#define GAMEOPT_P2CONTROL	"P2 CONTROL"
#define CONTROLOPT_KEYBOARD	"KEYBOARD"
#define CONTROLOPT_NONE		"NONE"
#define DIFFICULTY_EASY		"EASY"
#define DIFFICULTY_NORMAL	"NORMAL"
#define DIFFICULTY_HARD		"HARD"
#define DIFFICULTY_IMMORTAL	"I AM IMMORTAL"

#endif // __COMMON_H__
