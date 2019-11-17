#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include "common.h"

void calcFOV(Map map[LONGUEUR][LARGEUR], int view, int posX, int posY, WINDOW *win_map, Ennemy ennemies[], int nbrEnnemies, int *ennemyPresent, int colorBlind, int stone, Drug drugList[], int nbrDrugs, Weapons weaponOnFloor[], int nbrWeapons, Armor armorOnFloor[], int nbrArmor, Pnj pnjOnFloor[], int nbrPnjOnFloor);
void recalcScreen(WINDOW* win_map, Map map[LONGUEUR][LARGEUR], int nbrEnnemies, Ennemy ennemies[]);
void delay();
void flashBomb(WINDOW *win_map);

#endif
