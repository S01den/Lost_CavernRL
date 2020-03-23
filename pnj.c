#include "pnj.h"

int testPnjOnPos(Pnj pnj[], int x, int y, int nbrPnj)
{
	int ret = -1;
	int i = 0;

	for(i = 0; i < nbrPnj; i++)
	{
		if(pnj[i].x == x && pnj[i].y == y)
		{
			ret = i;
			break;
		}
	}

	return ret;
}


int sayQuest(Pnj pnj, Quest listQuests[], Quest actQuests[], int *nbrQuestAct, Weapons listWeapons[], Armor listArmor[], double *disease, int *questDrug, int *drugUsed)
{
	char *classNames[] = {"CLERIC","WORKER","ERMIT","SCIENTIST"};
	char *typeRewards[] = {"VAX","LIFE POINTS","COMPONENTS","EXOTIC BLOOD"};
	int key = 0;
	int i = 0;
	int select = 0;
	int choice = -1;

	if(listQuests[pnj.quest].isSet == 0)
	{
		WINDOW *win;
		win = newwin(13,75,LONGUEUR/2,LARGEUR/2+10);
		box(win,0,0);
		wattron(win, A_UNDERLINE);
		mvwprintw(win,2,22,"%s (%s)",pnj.name, classNames[pnj.type]);
		wattroff(win, A_UNDERLINE);
		mvwprintw(win,4,2,"%s",listQuests[pnj.quest].description);
		keypad(win, TRUE);
		if(listQuests[pnj.quest].typeReward <= 3)
		{
			mvwprintw(win,7,2,"Reward: %d (%s)", listQuests[pnj.quest].reward, typeRewards[listQuests[pnj.quest].typeReward]);
		}
		else if(listQuests[pnj.quest].typeReward == 4)
		{
			mvwprintw(win,7,2,"Reward: %s", listWeapons[listQuests[pnj.quest].reward].name);
		}
		else if(listQuests[pnj.quest].typeReward == 5)
		{
			mvwprintw(win,7,2,"Reward: %s", listArmor[listQuests[pnj.quest].reward].name);
		}
		else if(listQuests[pnj.quest].typeReward == 6)
		{
			mvwprintw(win,7,2,"Reward: a drug");
		}

		char *menus[] = {"ACCEPT","REJECT"};

		while(1)
		{
			for(i = 0; i < 2; i++)
			{
				if(i == select)
				{
					wattron(win, COLOR_PAIR(STAIRS_COLOR));
					mvwprintw(win, i+9, 75/2-10, menus[i]);
					wattroff(win,COLOR_PAIR(STAIRS_COLOR));
				}
				else
				{
					mvwprintw(win, i+9, 75/2-10, menus[i]);
				}
			}

			wrefresh(win);

			key = wgetch(win);
			switch(key)
			{	
				case KEY_DOWN:
					if(select+1<=1)
						select++;
					break;

				case KEY_UP:
					if(select-1 >= 0)
						select--;
					break;

				case '\n':
					choice = select;
					break;
			}
			if(choice != -1)
				break;
		}

		if(choice == 0 && *nbrQuestAct+1 <= NBR_QUESTS_MAX)
		{
			if(listQuests[pnj.quest].indice == 2)
			{
				*disease = 0;
			}
			int indiceQuest = 0;
			for(indiceQuest = 0; indiceQuest < NBR_QUESTS_MAX; indiceQuest++)
			{
				if(actQuests[indiceQuest].isSet == 0)
				{
					actQuests[indiceQuest].name = listQuests[pnj.quest].name;
					actQuests[indiceQuest].description = listQuests[pnj.quest].description;
					actQuests[indiceQuest].indice = listQuests[pnj.quest].indice;
					actQuests[indiceQuest].isSet = 1;
					listQuests[pnj.quest].isSet = 1;
					actQuests[indiceQuest].typeReward = listQuests[pnj.quest].typeReward;
					actQuests[indiceQuest].reward = listQuests[pnj.quest].reward;
					if(actQuests[indiceQuest].indice == 0)
					{
						*drugUsed = 0;
						*questDrug = 1;
					}
				}
			}
			(*nbrQuestAct)++;
		}

		box(win, ' ', ' ');
		wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
		wclear(win);
		wrefresh(win);
		delwin(win);
	}

	return choice;
}

void clearQuest(Quest quest[])
{
	int i = 0;
	for(i = 0; i < NBR_QUESTS_MAX; i++)
	{
		quest[i].isSet = 0;
		quest[i].name = "";
		quest[i].description = "";
		quest[i].typeReward = -1;
		quest[i].reward = -1;
		quest[i].indice = -1;
	}
}

void setPnj(Map map[LONGUEUR][LARGEUR], Pnj pnj[], int *nbrPnjOnFloor)
{
	*nbrPnjOnFloor = rand()%NBR_PNJ_MAX+1;
	char *nameClerc[] = {"Sister Maria", "Brother Mikael","Mother Cynthia", "Father Lyris"};
	char *nameWorker[] = {"Swann","Nicolas","Thomas","Erica"};
	char *nameErmit[] = {"Michel","Wallace","Francis","Jannice"};
	char *nameScientific[] = {"Dr Jane","Dr Cooper","Dr Jones","Dr Marlene"}; 
	
	int i = 0;

	for(i = 0; i < *nbrPnjOnFloor; i++) // let's spawn the NPC
	{
		pnj[i].x = rand()%LONGUEUR;
		pnj[i].y = rand()%LARGEUR;
		pnj[i].questDone = 0;

		if(pnj[i].x == 0)
			pnj[i].x++;
		if(pnj[i].x == LONGUEUR)
			pnj[i].x--;
		if(pnj[i].y == 0)
			pnj[i].y++;
		if(pnj[i].y == LARGEUR)
			pnj[i].y--;

		if(map[pnj[i].x][pnj[i].y].content != FLOOR)
		{
			if(LARGEUR-pnj[i].y > pnj[i].y)
			{
				while(map[pnj[i].x][pnj[i].y].content != FLOOR && pnj[i].y < LARGEUR)
				{
					pnj[i].y++;
				}
			}
			else
			{
				while(map[pnj[i].x][pnj[i].y].content != FLOOR && pnj[i].y > 0)
				{
					pnj[i].y--;
				}
			}
		}

		int rollType = rand()%4;
		pnj[i].type = rollType;

		switch(pnj[i].type)
		{
			case 0: // cleric
				pnj[i].name = nameClerc[rand()%4];
				pnj[i].quest = rand()%3;
				break;
			case 1: // worker
				pnj[i].name = nameWorker[rand()%4];
				pnj[i].quest = rand()%3+3;
				break;
			case 2: // ermit
				pnj[i].name = nameErmit[rand()%4];
				pnj[i].quest = rand()%3+6;
				break;
			case 3: // scientific
				pnj[i].name = nameScientific[rand()%4];
				pnj[i].quest = rand()%2+9;
				break;
		}
			map[pnj[i].x][pnj[i].y].content = PNJ;
			map[pnj[i].x][pnj[i].y].typePnj = pnj[i].type;

	}
}

void initQuest(Quest listQuests[])
{
	int i = 0;
	for(i = 0; i < NBR_QUESTS; i++)
	{
		listQuests[i].isSet = 0;
		listQuests[i].indice = i;
	}

	listQuests[0].name = "EXPIATION !"; // done
	listQuests[0].description = "Our god said that drugs are bad. I bet you can't stop it during the 3 next floors. And don't forget: GEMS ARE THE TRUTH";
	listQuests[0].typeReward = 0;  // 0 = heal from disease, 1 = heal life points, 2 = components, 3 = exotic blood, 4 = weapon, 5 = armor, 6 = drug
	listQuests[0].reward = 20; 

	listQuests[1].name = "EXHIBITIONIST !"; // done
	listQuests[1].description = "Our god said that wearing armors is sin. Please take of it until the next floor. And don't forget: GEMS ARE THE TRUTH";
	listQuests[1].typeReward = 0;
	listQuests[1].reward = 15;

	listQuests[2].name = "ERADICATOR !"; // done
	listQuests[2].description = "Our god said that technology is sin. Give up distance weapons now and forever and your soul will be treated. But don't abuse of our church, you wouldn't unleach the divine wrath... And don't forget: GEMS ARE THE TRUTH";
	listQuests[2].typeReward = 0;
	listQuests[2].reward = 100;

	listQuests[3].name = "THE LOST COMPUTER"; // done
	listQuests[3].description = "I've lost my laptop, it contains some... important data..., can you please bring me it ?";
	listQuests[3].typeReward = 2;
	listQuests[3].reward = 1000;

	listQuests[4].name = "KILL THE TURRETS, KILL THE TURRETS !"; //done
	listQuests[4].description = "The turrets are revolting against us ! Please help us to kill 5 of them !";
	listQuests[4].typeReward = 5;
	listQuests[4].reward = rand()%NBR_ARMORS;

	listQuests[5].name = "FIXING"; // done
	listQuests[5].description = "I need 50 components to fix my heal-machine.";
	listQuests[5].typeReward = 1;
	listQuests[5].reward = 20;

	listQuests[6].name = "THERE ARE MONSTERS HERE..."; //done
	listQuests[6].description = "Infected guardians terrify me, please kill 5 of them !";
	listQuests[6].typeReward = 6;
	listQuests[6].reward = rand()%NBR_DRUGS;
	
	listQuests[7].name = "THE LOST BONE"; // done
	listQuests[7].description = "I've lost my bone spear, can you bring me another one please ?";
	listQuests[7].typeReward = 6;
	listQuests[7].reward = rand()%NBR_DRUGS;

	listQuests[8].name = "BORN OF A BROKEN MAN"; //done
	listQuests[8].description = "Destroy 10 of theses damn droids; I can't stand this fucking corporation.";
	listQuests[8].typeReward = 4;
	listQuests[8].reward = rand()%NBR_WEAPONS;

	listQuests[9].name = "ANOTHER UNIVERSE"; // done
	listQuests[9].description = "I have to study the effects of hallucinogens drugs. Please kill someone under the effects of psychedelics.";
	listQuests[9].typeReward = 3;
	listQuests[9].reward = 40;

	listQuests[10].name = "GEMS ARE BLUE"; // done
	listQuests[10].description = "Thoses gems are a great source of power, we need to study it. Can you bring us 5 gems ?";
	listQuests[10].typeReward = 4;
	listQuests[10].reward = rand()%NBR_WEAPONS;
}
