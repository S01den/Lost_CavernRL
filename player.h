#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED


#include "common.h"

void initPlayer(Player *player, Inventory inventory[]);
void crystalCanon(Map map[LONGUEUR][LARGEUR], Player player, char dir);
int shotLine(Map map[LONGUEUR][LARGEUR],Weapons *weaponHold,Ennemy ennemies[], char dir, Player player, int nbrEnnemies,int *components);
int burn(Map map[LONGUEUR][LARGEUR],Weapons *weaponHold,Ennemy ennemies[], char dir, Player player, int nbrEnnemies, int *components,WINDOW* screen);
int hitEnnemy(Player player, Ennemy *ennemy, Weapons weaponHold, int *components, int *blood);
int hit(char dirChar, int dirX, int dirY, WINDOW *win_map, Player *player, Weapons *weaponHold, Weapons listWeapons[], Map map[LONGUEUR][LARGEUR], int *gem, Ennemy ennemies[], int nbrEnnemyInFloor, int *component, int *exoticBlood, int *compBefore, int *indiceEnnemy, int *retHit, Pnj pnj[], int nbrPnjInFloor, int *indicePnj);

#endif
