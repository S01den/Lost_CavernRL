#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "common.h"

int generMap(Map map[LONGUEUR][LARGEUR], WINDOW *win);
void initMap(Map map[LONGUEUR][LARGEUR], WINDOW *win);
void destroyMap(Map map[LONGUEUR][LARGEUR], Player player);
void destroyWall(Map map[LONGUEUR][LARGEUR], int x, int y, Weapons *weaponHold, int *gem);

#endif
