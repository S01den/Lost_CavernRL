#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

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
#include "map.h"

void shuffleDrugs(Map map[LONGUEUR][LARGEUR], Drug drugs[], int nbrDrugs, int *nbrDrugsInFloor);
void setObjects(Map map[LONGUEUR][LARGEUR], Weapons listWeapons[], Weapons weponsOnFloor[], int nbrMaxWeapons, int *nbrWeapons, Armor armorList[], Armor armorOnFloor[], int nbrMaxArmor, int *nbrArmor);
int testDrug(Drug drugList[], int x, int y, int nbrDrugsInFloor);
int testWeapon(Weapons weapons[],int x, int y, int nbrWeapons);
int testArmor(Armor armor[],int x, int y, int nbrArmors);

#endif