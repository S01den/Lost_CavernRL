#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

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
#include "item.h"

int craftMenu(Inventory inventory[], Drug listDrug[], Player player, int *component, int *exoticBlood, int *gem);
int takeDrug(Map map[LONGUEUR][LARGEUR],Inventory inventory[], Drug drugs[], int indice, char messagePrint[], int *colorPrint, Player *player, double *diseaseLevel);
int pauseMenu();
void objectMenu(Weapons *weapon, Weapons listWeapons[], Armor *armor, Armor listArmors[], Inventory inventory[], Player *player, int *component, int caseInventory, int *colorPrint, char messagePrint[], int type);
void questsMenu(Quest actQuest[], int nbrQuestAct, Weapons listWeapons[], Armor listArmor[], int nbrTurretKilled, int nbrInfectedKilled, int nbrDroidKilled);
void gameOver(int source, int score);

#endif