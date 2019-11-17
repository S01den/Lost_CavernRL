#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include "common.h"

void printScreenUser(WINDOW *win, Player player, int powerUsed, char diseaseName[20], unsigned int disease, unsigned int nbrEnnemiesKilled);
void printBar(WINDOW *win, char name[15], int attribute,int x, int y, int colorpair);
void printEnnemy(WINDOW *win,Ennemy ennemies[], int nbrEnnemyFloor);
void printWeapon(WINDOW *win, Weapons weaponHold);
void printInventory(WINDOW *win, int component, int exoticBlood, int gem, Player player, Inventory inventory[], Drug drugList[], Weapons listWeapons[], Armor listArmors[]);
void setMessage(WINDOW *win,char message[],int colorPair);
void printArmor(WINDOW *win, Armor armor);

#endif
