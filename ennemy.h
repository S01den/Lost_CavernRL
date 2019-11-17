#ifndef ENNEMY_H_INCLUDED
#define ENNEMY_H_INCLUDED

#include "common.h"

// http://www.roguebasin.com/index.php?title=Monster_attacks_in_a_structured_list very interesting

void moveRand(Ennemy ennemies[], Map map[LONGUEUR][LARGEUR], int indice);
void iaCore(Ennemy ennemies[], Player *player, Map map[LONGUEUR][LARGEUR], int *nbrEnnemies, char messagePrint[], int *colorPrint, Armor *armor, WINDOW *win_msg, int *source);
void repair(Ennemy ennemies[], int indice, int *colorPrint, char messagePrint[]);
void setEnnemies(Map map[LONGUEUR][LARGEUR], Ennemy ennemies[], int nbrEnnemies, int *nbrEnnemyInFloor, int floor);
int testEnnemyOnPos(Ennemy ennemies[], int x, int y, int nbrEnnemies);
int blastHit(Ennemy enemy, Player *player, Armor *armor, Map map[LONGUEUR][LARGEUR]);

#endif
