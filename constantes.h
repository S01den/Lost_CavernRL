#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

#define LARGEUR 130
#define LONGUEUR 39
#define NBR_TILES 5 // number of tiles which aren't items
#define NBR_TYPES_TRAPS 3

#define PNJ 11
#define BOMB 10
#define DIST_WEAPON 9
#define CONTACT_WEAPON 8
#define ARMOR 7
#define DOOR 6
#define DRUG 5
#define MINES 4
#define STAIRS 3
#define WALL 2
#define FLOOR 1
#define HOLE 0

#define COLOR_DRUGS 50
#define COLOR_HOLE 234
#define COLOR_LAVA 245
#define COLOR_RESIST_1 19
#define COLOR_RESIST_2 24
#define COLOR_RESIST_3 239
#define COLOR_BH 250
#define COLOR_MEDIC 251
#define COLOR_TRACER 252
#define COLOR_PACIFICATOR 253
#define STAIRS_COLOR 254
#define ENEMY_COLORS 240
#define BURN_EFFECT 246

#define NBR_WEAPONS 15
#define NBR_ARMORS 10
#define NBR_DRUGS 7
#define NBR_QUESTS 11
#define NBR_QUESTS_MAX 3

#define ARMORLESS 9

// maximal number of things on a floor
#define WEAPON_MAX 8
#define ARMOR_MAX 6
#define DRUGS_MAX 14
#define ENEMIES_MAX 35	
#define NBR_PNJ_MAX 4
#define INVENTORY_MAX 15

typedef struct Pnj
{
	char *name;
	int type; // 0 = cleric (from the stone order) 1 = worker 2 = ermit 3 = scientific
	int quest; 
	int questDone;
	int x;
	int y;
} Pnj;

typedef struct Quest
{
	char *name;
	char *description;
	int typeReward; // 0 = heal from disease, 1 = heal life points, 2 = components, 3 = exotic blood, 4 = weapon, 5 = armor, 6 = drug
	int reward; 
	int indice;
	int isSet;
} Quest;

typedef struct Maps
{
	int content;
	int effect; // 0 = nothing, 1 = on fire, 2 = radioactive, 3 = lava, 4 = gem
	int seen;
	int typeEnnemy;
	int typePnj;
	int resistance;
	int trap; // type of trap: 0 = none ; 1 = electric mine ; 2 = paralysant mine ; 3 = explosive trap
} Map;

typedef struct Ennemies
{
	int x;
	int y; // light, on fire etc...
	int life;
	int seen;
	int strength;
	int agility;
	int haveBlaster;
	int efficiency;
	int type; // 1 = droid, 2 = sentry, 3 = guardian etc...
	int effect; // 0 = nothing, 1 = on fire etc...
	int boss;
	char *name;
} Ennemy;

typedef struct Weapons
{
	char *name;
	char *description;
	int degat;
	int efficiency; // probability of hit an ennemy
	int strengthRequired;
	int state; // state or munitions (depend of the range, if it's a contact or a distance weapon)
	int range; 
	int type; // Distance(1) or Contact(0)
	int cost; // cost in components when state = 0; the player can destroy himself his weapons in order to get more components (cost+state/2)
	int indice; // 0 = fist, 1 = basic spear etc...
	int rarity; // 1 = common, 2 = epic, 3 = legendary
	int x;
	int y;
} Weapons;

typedef struct Armor
{
	char *name;
	char *description;
	int cost;
	int resistance;
	int state;
	int strengthRequired;
	int indice;
	int rarity;
	int x;
	int y;
} Armor;

typedef struct Drug
{
	int type; // vaccin = 0, steroid = 1, antidote = 2, stimulants = 3, hallucinogen = 4, medipac = 5, neuroparalysant = 6
	int poisoned;
	int color;
	int indice;
	int discovered;
	int x;
	int y;
} Drug;

typedef struct Inventory
{
	int type; // type of object: -1 = nothing, 0 = weapon, 1 = armor, 2 = drugs, 3 = other
	int object; // indice of object
	//int number; // the number of items in the slot
} Inventory;

typedef struct Player
{
	char name[15];
	int life;
	int x;
	int y;
	int power;
	int strength;
	int agility;
	int hate;
	int view;
	int state; // normal, burned, paralysed, high,
	int state2; // same
	int colorBlind;
	int inventorySize;

} Player;

#endif

