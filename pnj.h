#ifndef PNJ_H_INCLUDED
#define PNJ_H_INCLUDED

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

void setPnj(Map map[LONGUEUR][LARGEUR], Pnj pnj[], int *nbrPnjOnFloor);
void initQuest(Quest listQuests[]);
void clearQuest(Quest quest[]);
int testPnjOnPos(Pnj pnj[], int x, int y, int nbrPnj);
int sayQuest(Pnj pnj, Quest listQuests[], Quest actQuests[], int *nbrQuestAct, Weapons listWeapons[], Armor listArmor[], double *disease, int *questDrug, int *drugUsed);

#endif
