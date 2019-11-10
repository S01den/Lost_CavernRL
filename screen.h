#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <curses.h>
#include <termios.h>
#include <termio.h>
#include <sgtty.h> 
#include <signal.h>
#include "ennemy.h"
#include "map.h"

void calcFOV(Map map[LONGUEUR][LARGEUR], int view, int posX, int posY, WINDOW *win_map, Ennemy ennemies[], int nbrEnnemies, int *ennemyPresent, int colorBlind, int stone, Drug drugList[], int nbrDrugs, Weapons weaponOnFloor[], int nbrWeapons, Armor armorOnFloor[], int nbrArmor, Pnj pnjOnFloor[], int nbrPnjOnFloor);
void recalcScreen(WINDOW* win_map, Map map[LONGUEUR][LARGEUR], int nbrEnnemies, Ennemy ennemies[]);
void delay();
void flashBomb(WINDOW *win_map);

#endif