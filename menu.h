#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "common.h"

int craftMenu(Inventory inventory[], Drug listDrug[], Player player, int *component, int *exoticBlood, int *gem, int minimal);
int takeDrug(Map map[LONGUEUR][LARGEUR],Inventory inventory[], Drug drugs[], int indice, char messagePrint[], int *colorPrint, Player *player, double *diseaseLevel);
int pauseMenu(int y);
int passwordComputer();
void objectMenu(Weapons *weapon, Weapons listWeapons[], Armor *armor, Armor listArmors[], Inventory inventory[], Player *player, int *component, int caseInventory, int *colorPrint, char messagePrint[], int type);
void questsMenu(Quest actQuest[], int nbrQuestAct, Weapons listWeapons[], Armor listArmor[], int nbrTurretKilled, int nbrInfectedKilled, int nbrDroidKilled);
void gameOver(int source, int score, int minimal);
void popUpArchitect();

#endif
