#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

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
#include "constantes.h"
#include "ennemy.h"

int generMap(Map map[LONGUEUR][LARGEUR], WINDOW *win);
void initMap(Map map[LONGUEUR][LARGEUR], WINDOW *win);
void destroyMap(Map map[LONGUEUR][LARGEUR], Player player);
void destroyWall(Map map[LONGUEUR][LARGEUR], int x, int y, Weapons *weaponHold, int *gem);

#endif