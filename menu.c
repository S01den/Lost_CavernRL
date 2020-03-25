#include "menu.h"


void questsMenu(Quest actQuest[], int nbrQuestAct, Weapons listWeapons[], Armor listArmor[], int nbrTurretKilled, int nbrInfectedKilled, int nbrDroidKilled)
{
	int i = 0;
	int j = 0;
	char *typeRewards[] = {"VAX","LIFE POINTS","COMPONENTS","EXOTIC BLOOD"};

	WINDOW *win;
	win = newwin(20,75,LONGUEUR/2-2,LARGEUR/4-5);
	box(win,0,0);
	for(i = 1; i < 75/2-5; i++)
		mvwprintw(win,1,i,"-");
	for(i = 75/2+4; i < 74; i++)
		mvwprintw(win,1,i,"-");

	wattron(win, A_BOLD);
	mvwprintw(win,1,75/2-4,"QUESTS:");
	wattroff(win, A_BOLD);
	keypad(win, TRUE);

	for(i = 0; i < nbrQuestAct+1; i++)
	{
		for(j = 1; j < 74; j++)
			mvwprintw(win,i*3+6,j,"-");
		if(actQuest[i].isSet == 1)
		{
			if(actQuest[i].typeReward <= 3)
			{
				mvwprintw(win,i*3+2,1,"%s:\n%s\n  Reward: %d (%s)",actQuest[i].name,actQuest[i].description,actQuest[i].reward,typeRewards[actQuest[i].typeReward]);
			}
			else if(actQuest[i].typeReward == 4)
			{
				mvwprintw(win,i*3+2,1,"%s:\n%s\n Reward: %s",actQuest[i].name,actQuest[i].description,listWeapons[actQuest[i].reward].name);
			}
			else if(actQuest[i].typeReward == 5)
			{
				mvwprintw(win,i*3+2,1,"%s:\n%s\n Reward: %s",actQuest[i].name,actQuest[i].description,listArmor[actQuest[i].reward].name);
			}
			else if(actQuest[i].typeReward == 6)
			{
				mvwprintw(win,i*3+2,1,"%s:\n%s\n Reward: a drug",actQuest[i].name,actQuest[i].description);
			}
			if(actQuest[i].indice == 4)
			{
				wattron(win,A_BOLD);
				mvwprintw(win,i*3+5,1,"(%d/5 Killed)",nbrTurretKilled);
				wattroff(win,A_BOLD);
			}
			else if(actQuest[i].indice == 6)
			{
				wattron(win,A_BOLD);
				mvwprintw(win,i*3+5,1,"(%d/5 Killed)",nbrInfectedKilled);
				wattroff(win,A_BOLD);
			}
			else if(actQuest[i].indice == 8)
			{
				wattron(win,A_BOLD);
				mvwprintw(win,i*3+5,1,"(%d/10 Killed)",nbrDroidKilled);
				wattroff(win,A_BOLD);
			}
		}
		for(j = 1; j < 74; j++)
			mvwprintw(win,i*3+6,j,"-");

	}

	wrefresh(win);

	wgetch(win);

	box(win, ' ', ' ');
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(win);
	wrefresh(win);
	delwin(win);	
}

int pauseMenu(int y, int *keyboard)
{
	int ret = 0;
	int select = 2;
	int choice = -1;
	int i = 0;
	int key = 0;

	WINDOW *win;
	win = newwin(14,20,LONGUEUR/2,LARGEUR/2+y);
	box(win,0,0);
	wattron(win, A_UNDERLINE);
	mvwprintw(win,4,8,"MENU");
	wattroff(win, A_UNDERLINE);
	keypad(win, TRUE);

	char *menus[] = {"KEYS LIST","TILES LIST","RETURN TO GAME","CHANGE KEYBOARD","EXIT THE GAME"};

	while(1)
	{
		for(i = 0; i < 5; i++)
		{
			if(i == select)
			{
				wattron(win, COLOR_PAIR(STAIRS_COLOR));
				mvwprintw(win, i+7, 4, menus[i]);
				wattroff(win,COLOR_PAIR(STAIRS_COLOR));
			}
			else
			{
				mvwprintw(win, i+7, 4, menus[i]);
			}
		}

		wrefresh(win);

		key = wgetch(win);
		switch(key)
		{	
			case KEY_DOWN:
				if(select+1<=4)
					select++;
				break;

			case KEY_UP:
				if(select-1 >= 0)
					select--;
				break;

			case'\n':
				choice = select;
				break;
		}

		if(choice != -1)
			break;
	}

	if(choice == 4)
		ret = 1;

	box(win, ' ', ' ');
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(win);
	wrefresh(win);
	delwin(win);

	if(choice == 0)
	{
		WINDOW *winKey;
		winKey = newwin(15,65,LONGUEUR/2-2,LARGEUR/2+5);
		box(winKey,0,0);
		keypad(winKey, TRUE);	

		wattron(winKey,A_UNDERLINE);
		mvwprintw(winKey,1,15,"*** KEY LIST ***");
		wattroff(winKey,A_UNDERLINE);
		mvwprintw(winKey,2,1,"[ESC] : MENU");
		mvwprintw(winKey,8,1,"c : CRAFT MENU");
		mvwprintw(winKey,3,1,"[SPACE] : MISS A TURN");
		mvwprintw(winKey,9,1,", : QUEST JOURNAL");
		mvwprintw(winKey,6,1,"[ARROWS] : MOVE");
		mvwprintw(winKey,7,1,"zsqdaewx : ATTACK ON A DIRECTION (OR TALK TO A PNJ)");
		mvwprintw(winKey,4,1,"A_NUMBER : USE AN OBJECT IN YOUR INVENTORY");
		mvwprintw(winKey,5,1,"[ENTER] : USE YOUR SPECIAL POWER");

		wrefresh(winKey);
		wgetch(winKey);

		box(winKey, ' ', ' ');
		wborder(winKey, ' ', ' ', ' ',' ',' ',' ',' ',' ');
		wclear(winKey);
		wrefresh(winKey);
		delwin(winKey);
	}
	else if(choice == 1)
	{
		WINDOW *winKey;
		winKey = newwin(24,65,LONGUEUR/2-2,LARGEUR/2+5);
		box(winKey,0,0);
		keypad(winKey, TRUE);	

		wattron(winKey,A_UNDERLINE);
		mvwprintw(winKey,1,15,"*** TILES LIST ***");
		wattroff(winKey,A_UNDERLINE);
		mvwprintw(winKey,2,1,"# : WALL");
		wattron(winKey,COLOR_PAIR(124));
		mvwprintw(winKey,3,1,"# : GEM");
		wattroff(winKey,COLOR_PAIR(124));
		mvwprintw(winKey,4,1,"@ : PLAYER");
		mvwprintw(winKey,5,1,"= : TRAP");
		wattron(winKey,COLOR_PAIR(COLOR_LAVA));
		mvwprintw(winKey,6,1,"* : LAVA");
		wattroff(winKey,COLOR_PAIR(COLOR_LAVA));
		mvwprintw(winKey,7,1,"* : BOMB");
		mvwprintw(winKey,8,1,"> : STAIRS");
		mvwprintw(winKey,9,1,"! : DRUG");
		mvwprintw(winKey,10,1,"+ : SHIELD");
		mvwprintw(winKey,11,1,"; : DISTANCE WEAPON");
		mvwprintw(winKey,12,1,"/ : CONTACT WEAPON");
		mvwprintw(winKey,13,1,"[ : ARMOR");
		mvwprintw(winKey,14,1,"d : DROID");
		mvwprintw(winKey,15,1,"t : TURRET");
		mvwprintw(winKey,16,1,"n : INFECTED GUARDIAN");
		mvwprintw(winKey,17,1,"g : GUARDIAN");
		mvwprintw(winKey,18,1,"c : CLERIC");
		mvwprintw(winKey,19,1,"s : SCIENTIST");
		mvwprintw(winKey,20,1,"e : ERMIT");
		mvwprintw(winKey,21,1,"w : WORKER");
		wattron(winKey,COLOR_PAIR(47));
		mvwprintw(winKey,22,1,".: RADIOACTIVITY");
		wattroff(winKey,COLOR_PAIR(47));

		wrefresh(winKey);
		wgetch(winKey);

		box(winKey, ' ', ' ');
		wborder(winKey, ' ', ' ', ' ',' ',' ',' ',' ',' ');
		wclear(winKey);
		wrefresh(winKey);
		delwin(winKey);	
	}
	if(choice == 3)
	{
		(*keyboard) ^= 1;
	}

	return ret;
}

void objectMenu(Weapons *weapon, Weapons listWeapons[], Armor *armor, Armor listArmors[], Inventory inventory[], Player *player, int *component, int caseInventory, int *colorPrint, char messagePrint[], int type)
{
	WINDOW* win;
	win = newwin(9,60,(LONGUEUR-5)/2+7,(LARGEUR-15)/2);
	box(win, 0 , 0);
	
	if(type == 1) // weapon
	{
		wattron(win,A_BOLD);
		mvwprintw(win,1,5,"%s",listWeapons[inventory[caseInventory].object].name);
		wattroff(win,A_BOLD);
		mvwprintw(win,2,1,"Degats: %d     State: %d",listWeapons[inventory[caseInventory].object].degat,listWeapons[inventory[caseInventory].object].state);
		mvwprintw(win,4,1,"%s",listWeapons[inventory[caseInventory].object].description);
		mvwprintw(win,6,1,"1) EQUIP");
		mvwprintw(win,7,1,"2) DESTROY TO COLLECT COMPONENTS");

		wrefresh(win);	

		int charMenu = wgetch(win);
		switch(charMenu)
		{
			case '1':
					if(listWeapons[inventory[caseInventory].object].strengthRequired <= player->strength)
					{
						int indiceWeapChange = weapon->indice;
						listWeapons[indiceWeapChange].state = weapon->state;
						*weapon = listWeapons[inventory[caseInventory].object];
						inventory[caseInventory].type = 0;
						inventory[caseInventory].object = indiceWeapChange;
					}
					else
					{
						*colorPrint = 197; 
						snprintf(messagePrint,100,"YOU MUST HAVE A MINIMUM STRENGTH OF %d !",listWeapons[inventory[caseInventory].object].strengthRequired);

					}
					break;
				
				case '2':
						*component += listWeapons[inventory[caseInventory].object].cost;
						snprintf(messagePrint,100,"BY DESTROYING THIS ITEM, YOU EARNED %d COMPONENTS !",listWeapons[inventory[caseInventory].object].cost);
						inventory[caseInventory].type = -1;
						inventory[caseInventory].object = 0;
						*colorPrint = 47; 
					break;
		}

	}
	else
	{
		wattron(win,A_BOLD);
		mvwprintw(win,1,5,"%s",listArmors[inventory[caseInventory].object].name);
		wattroff(win,A_BOLD);
		mvwprintw(win,2,1,"Resistance: %d    State: %d",listArmors[inventory[caseInventory].object].resistance,listArmors[inventory[caseInventory].object].state);
		mvwprintw(win,4,1,"%s",listArmors[inventory[caseInventory].object].description);
		mvwprintw(win,6,1,"1) EQUIP");
		mvwprintw(win,7,1,"2) DESTROY TO COLLECT COMPONENTS");

		int charMenu = wgetch(win);
		switch(charMenu)
		{
			case '1':

					if(listArmors[inventory[caseInventory].object].strengthRequired <= player->strength)
					{
						int indiceArmorChange = armor->indice;
						listArmors[indiceArmorChange].state = armor->state;
						*armor = listArmors[inventory[caseInventory].object];
						inventory[caseInventory].type = 1;
						inventory[caseInventory].object = indiceArmorChange;
					}
					else
					{
						*colorPrint = 197; 
						snprintf(messagePrint,100,"YOU MUST HAVE A MINIMUM STRENGTH OF %d !",listArmors[inventory[caseInventory].object].strengthRequired);
					}
					break;
				
				case '2':
						*component += listArmors[inventory[caseInventory].object].cost;
						snprintf(messagePrint,100,"BY DESTROYING THIS ITEM, YOU EARNED %d COMPONENTS !",listArmors[inventory[caseInventory].object].cost);
						inventory[caseInventory].type = -1;
						inventory[caseInventory].object = 0;
						*colorPrint = 47; 
					break;
		}
	}

	wrefresh(win);	

	box(win, ' ', ' ');
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(win);
	wrefresh(win);
	delwin(win);
}


int craftMenu(Inventory inventory[], Drug listDrug[], Player player, int *component, int *exoticBlood, int *gem, int minimal)
{
	int ret = 0;
	int s = 0;

	WINDOW* win;
	if(!minimal)
		win = newwin(16,42,LONGUEUR/2-5,LARGEUR-20);
	else
		win = newwin(16,42,LONGUEUR/2,LARGEUR/4);
	box(win, 0 , 0);
	wattron(win,A_UNDERLINE);
	mvwprintw(win,1,1,"COMPONENTS: %d",*component);
	mvwprintw(win,1,16,"EXOTIC BLOOD: %d",*exoticBlood);
	mvwprintw(win,1,33,"GEMS: %d",*gem);
	wattroff(win,A_UNDERLINE);

	wattron(win,COLOR_PAIR(47));
	mvwprintw(win,2,1,"1) * : NUCLEAR CHARGE (30c + 1g)");
	wattroff(win,COLOR_PAIR(47));
	mvwprintw(win,3,1,"2) ! : VACCIN (30eb)");
	wattron(win,COLOR_PAIR(124));
	mvwprintw(win,4,1,"3) ~ : SHIELD (15c)");
	wattroff(win,COLOR_PAIR(124));
	wattron(win,COLOR_PAIR(22));
	mvwprintw(win,5,1,"4) = : ELECTRIC TRAP (20c)");
	wattroff(win,COLOR_PAIR(22));
	wattron(win,COLOR_PAIR(23));
	mvwprintw(win,6,1,"5) = : PARALYSING TRAP (20c)");
	wattroff(win,COLOR_PAIR(23));
	wattron(win,COLOR_PAIR(COLOR_LAVA));
	mvwprintw(win,7,1,"6) * : LAVA BUCKET (40eb + 1g)");
	wattroff(win,COLOR_PAIR(COLOR_LAVA));
	mvwprintw(win,8,1,"7) ! : ANTIDOTE (35eb)");

	wrefresh(win);

	int key = wgetch(win);

	box(win, ' ', ' ');
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(win);
	wrefresh(win);
	delwin(win);	

	switch(key)
	{	
		case '1':
			if(*component >= 20 && *gem >= 1)
			{
				(*gem)--;
				*component -= 20;
				for(s = 0; s < player.inventorySize; s++)
				{
					if(inventory[s].type == -1)
					{
						inventory[s].type = 3;
						inventory[s].object = 1; // nuclear charge
						break;
					}
				}
			}
			break;
		case '2':
			if(*exoticBlood >= 30)
			{
				*exoticBlood -= 30;
				for(s = 0; s < player.inventorySize; s++)
				{
					if(inventory[s].type == -1)
					{
						inventory[s].type = 2;
						int indiceVax = 0;
						for(indiceVax = 0; indiceVax < NBR_DRUGS; indiceVax++)
						{
							if(listDrug[indiceVax].type == 0)
								inventory[s].object = indiceVax; // vaccin
						}
						break;
					}
				}
			}
			break;
		case '3':
			if(*component >= 15)
			{
				*component -= 15;
				for(s = 0; s < player.inventorySize; s++)
				{
					if(inventory[s].type == -1)
					{
						inventory[s].type = 3;
						inventory[s].object = 2; // shield 
						break;
					}
				}
			}
			break;
		case '4':
			if(*component >= 20)
			{
				*component -= 20;
				for(s = 0; s < player.inventorySize; s++)
				{
					if(inventory[s].type == -1)
					{
						inventory[s].type = 3;
						inventory[s].object = 3; 
						break;
					}
				}
			}
			break;
		case '5':
			if(*component >= 20)
			{
				*component -= 20;
				for(s = 0; s < player.inventorySize; s++)
				{
					if(inventory[s].type == -1)
					{
						inventory[s].type = 3;
						inventory[s].object = 4;
						break;
					}
				}
			}
			break;
		case '6':
			if(*exoticBlood >= 40 && *gem >= 1)
			{
				*exoticBlood -= 40;
				(*gem)--;
				for(s = 0; s < player.inventorySize; s++)
				{
					if(inventory[s].type == -1)
					{
						inventory[s].type = 3;
						inventory[s].object = 5;
						break;
					}
				}
			}
			break;
		case '7':
			if(*exoticBlood >= 35)
			{
				*exoticBlood -= 35;
				for(s = 0; s < player.inventorySize; s++)
				{
					if(inventory[s].type == -1)
					{
						inventory[s].type = 2;
						int indiceAntidote = 0;
						for(indiceAntidote = 0; indiceAntidote < NBR_DRUGS; indiceAntidote++)
						{
							if(listDrug[indiceAntidote].type == 2)
								inventory[s].object = indiceAntidote; // vaccin
						}
						break;
					}
				}
			}
			break;
	}

	return ret;
}	

void popUpArchitect()
{
	WINDOW *win;
	win = newwin(5,30,rand()%LONGUEUR,rand()%LARGEUR);
	box(win,0,0);
	int color = rand()%255+1;
	wattron(win, COLOR_PAIR(color));
	mvwprintw(win,1,1,"PFL TRE'K VJTRGV !");
	mvwprintw(win,2,1,"JKFG");
	mvwprintw(win,3,1,"**WE ALREADY WON**");
	if(rand()%100 > 50)
		mvwprintw(win,4,1,"THEY PAID A LOT FOR THIS WEAPON...");
	wattroff(win, COLOR_PAIR(color));
	box(win, ' ', ' ');
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(win);
	wrefresh(win);
	delwin(win);
}

int takeDrug(Map map[LONGUEUR][LARGEUR], Inventory inventory[], Drug drugs[], int indice, char messagePrint[], int *colorPrint, Player *player, double *diseaseLevel)
{
	int ret = 0;
	int i = 0;
	char *typeOfDrugs[] = {"VACCIN","STEROID","ANTIDOTE","STIMULANT","HALLUCINOGEN","MEDIPAC","NEUROPARALYSANT"};

	WINDOW* win_menuInvent;
	win_menuInvent = newwin(8,30,(LONGUEUR-5)/2+7,(LARGEUR-15)/2);
	box(win_menuInvent, 0 , 0);
	
	if(inventory[indice].type == 2)
	{
		Drug drugUse = drugs[inventory[indice].object];
		if(drugUse.discovered == 0)
		{
			wattron(win_menuInvent,COLOR_PAIR(drugUse.color));
			mvwprintw(win_menuInvent,1,3,"! : UNKNOWN DRUG");
			wattroff(win_menuInvent,COLOR_PAIR(drugUse.color));
		}
		else
		{
			wattron(win_menuInvent,COLOR_PAIR(drugUse.color));
			mvwprintw(win_menuInvent,1,3,"! : %s",typeOfDrugs[drugUse.type]);
			wattroff(win_menuInvent,COLOR_PAIR(drugUse.color));
		}

			mvwprintw(win_menuInvent,3,1,"1) INJECT");
			mvwprintw(win_menuInvent,4,1,"2) DROP");

			int charMenu = wgetch(win_menuInvent);
			switch(charMenu)
			{
				case '1':
					drugs[inventory[indice].object].discovered = 1;
					for(i = 0; i < NBR_DRUGS; i++)
					{
						if(drugs[i].type == drugUse.type)
						{
							drugs[i].discovered = 1;

						}
					}
					switch(drugUse.type)
					{
						case 0: // vaccin
							if(drugUse.poisoned == 1)
							{
								*diseaseLevel+=10;
								*colorPrint = 197;
								snprintf(messagePrint,100,"YOU TOOK A POISONNED VAX ! (DISEASE LEVEL +10)");
							}
							else
							{
								*diseaseLevel-=10;
								*colorPrint = 47;
								snprintf(messagePrint,100,"YOU TOOK A VAX ! (DISEASE LEVEL -10)");
							}
							break;
						case 1: // steroid
							if(drugUse.poisoned == 1)
							{
								player->strength--;
								*colorPrint = 197;
								snprintf(messagePrint,100,"YOU TOOK A POISONNED STEROID DOSE, (-1 STR) !");
							}
							else
							{
								player->strength++;
								*colorPrint = 47;
								snprintf(messagePrint,100,"YOU TOOK A STEROID DOSE, (+1 STR) !");
							}
							break;
						case 2: // antidote
							wclear(win_menuInvent);
							wattron(win_menuInvent,A_UNDERLINE);
							mvwprintw(win_menuInvent,1,(LARGEUR-15)/4+25,"ANTIDOTE");
							wattroff(win_menuInvent,A_UNDERLINE);
							mvwprintw(win_menuInvent,2,3,"SELECT A DRUG IN YOUR INVENTORY TO DISCOVER AND CLEAN IT");
							wrefresh(win_menuInvent);

							int keyAntidote = wgetch(win_menuInvent);
							if(keyAntidote >= '1' && keyAntidote <= '9')
							{
								keyAntidote -= ('0'+1);
								if(inventory[keyAntidote].type == 2)
								{
									drugs[inventory[keyAntidote].object].discovered = 1;
									drugs[inventory[keyAntidote].object].poisoned = 0;
								}
							}
							
							*colorPrint = 47;
							snprintf(messagePrint,100,"YOU TOOK AN ANTIDOTE !");
							break;
						case 3: // stimulant
							if(drugUse.poisoned == 1)
							{
								player->agility--;
								*colorPrint = 197;
								snprintf(messagePrint,100,"YOU TOOK A POISONNED STIMULANT DOSE, (-1 AGL) !");
							}
							else
							{
								player->agility++;
								*colorPrint = 47;
								snprintf(messagePrint,100,"YOU TOOK A STIMULANT DOSE, (+1 AGL) !");
							}
							break;
						case 4: // hallucinogen
							if(player->state != 0)
								player->state2 = 3;
							else
								player->state = 3;
							player->view += 2;
							*colorPrint = 47;
							snprintf(messagePrint,120,"YOU TOOK AN HALLUCINOGEN DRUG ! YOUR VISION IS ALTERED BUT YOUR FIELD OF VIEW IS INCREASED");
							break;
						case 5: // medipac
							if(drugUse.poisoned == 1)
							{
								player->life -= 20;
								*colorPrint = 197; 
								snprintf(messagePrint,100,"YOU TOOK A POISONNED MEDIPAC, (-20 LP) !");
							}
							else
							{
								player->life += 20;
								*colorPrint = 47; 
								snprintf(messagePrint,100,"YOU TOOK A MEDIPAC, (+20 LP) !");
							}
							break;
						case 6: // neuroparalysant
								if(player->state != 0)
									player->state2 = 2;
								else
									player->state = 2;
								*colorPrint = 197; 
								snprintf(messagePrint,100,"YOU TOOK A NEUROPARALYSER POISON !");						
							break;
					}
					
					inventory[indice].type = -1;
					inventory[indice].object = 0;
					ret = 1;
					break;

				case '2':
					if(map[player->x+1][player->y].content == FLOOR)
					{
						drugs[inventory[indice].object].x = player->x+1;
						drugs[inventory[indice].object].y = player->y;
						map[player->x+1][player->y].content = DRUG;

						inventory[indice].type = -1;
						inventory[indice].object = 0;
					}
					else if(map[player->x-1][player->y].content == FLOOR)
					{
						drugs[inventory[indice].object].x = player->x-1;
						drugs[inventory[indice].object].y = player->y;
						map[player->x-1][player->y].content = DRUG;

						inventory[indice].type = -1;
						inventory[indice].object = 0;
					}
					else if(map[player->x][player->y+1].content == FLOOR)
					{
						drugs[inventory[indice].object].x = player->x;
						drugs[inventory[indice].object].y = player->y+1;
						map[player->x][player->y+1].content = DRUG;

						inventory[indice].type = -1;
						inventory[indice].object = 0;
					}
					else if(map[player->x][player->y-1].content == FLOOR)
					{
						drugs[inventory[indice].object].x = player->x;
						drugs[inventory[indice].object].y = player->y-1;
						map[player->x][player->y-1].content = DRUG;

						inventory[indice].type = -1;
						inventory[indice].object = 0;
					}	
						break;
			}

			wrefresh(win_menuInvent);
	}
	
	box(win_menuInvent, ' ', ' ');
	wborder(win_menuInvent, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(win_menuInvent);
	wrefresh(win_menuInvent);
	delwin(win_menuInvent);

 return ret;
}

void gameOver(int source, int score, int minimal)
{
	WINDOW *win;
	if(minimal)
		win = newwin(7,50,LONGUEUR/2-2,LARGEUR/2-5);
	else
		win = newwin(7,50,LONGUEUR/2-2,LARGEUR/2+60);
	box(win,0,0);
	wattron(win,COLOR_PAIR(197));
	mvwprintw(win,1,15,"** GAME OVER !**");
	wattroff(win,COLOR_PAIR(197));
	wattron(win,A_BOLD);

	mvwprintw(win,3,1,"You were miserably killed by");
	switch(source)
	{
		case 1:
			mvwprintw(win,3,30,"an electric trap");
			break;
		case 2:
			mvwprintw(win,3,30,"an explosive trap");
			break;
		case 3:
			wattron(win,COLOR_PAIR(47));
			mvwprintw(win,3,30,"radioactivity");
			wattroff(win,COLOR_PAIR(47));
			break;
		case 4:
			mvwprintw(win,3,30,"the divine wrath !");
			break;
		case 5:
			mvwprintw(win,3,30,"your disease");
			break;
		case 6:
			wattron(win,COLOR_PAIR(197));
			mvwprintw(win,3,30,"fire");
			wattroff(win,COLOR_PAIR(197));
			break;
		case 7:
			mvwprintw(win,3,30,"a droid");
			break;
		case 8:
			mvwprintw(win,3,30,"a sentry");
			break;
		case 9:
			mvwprintw(win,3,30,"a guardian");
			break;
		default:
			mvwprintw(win,3,30,"suicide...");
	}
	wattron(win, A_UNDERLINE);
	mvwprintw(win,5,17,"SCORE: %d",score);
	wattroff(win, A_UNDERLINE);
	wrefresh(win);

	int i = 0;
	for(i = 0; i < 1000000000; i++)
	{
		//wait
	}


	wgetch(win);
	wattroff(win,A_BOLD);
	box(win,' ',' ');
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(win);
	wrefresh(win);
	delwin(win);	
}

int passwordComputer()
{
	int ret = 1;
	int k = 0;
	int i = 0;
	char password[11];
	char crypt[] = "\xf4\xb2\xa3\xa8\xf1\xb4\xf3\xa3\xb4\xe1";

	WINDOW *win;
	win = newwin(7,60,LONGUEUR/2-2, LARGEUR/2+60);
	box(win,0,0);
	wattron(win, COLOR_PAIR(47));
	mvwprintw(win,1,30,"*** CORRUPTION ***");
	mvwprintw(win,2,1,"PLEASE ENTER THE PASSWORD TO RECOVER YOUR DATA: ");
	wrefresh(win);

	int keyName = 0;
	int nameLen = 0;
	while(keyName != '\n')
	{
		keyName = wgetch(win);
		if(keyName == KEY_BACKSPACE && nameLen > 0)
		{
			password[nameLen-1] = ' ';
			nameLen--;
		}
		else if(keyName != '\n' && nameLen < 10)
		{
			password[nameLen] = keyName;
			nameLen++;
		}
		mvwprintw(win,3,1,"%s",password);
		wrefresh(win);
	}

	for(i = 0; i < 10; i++)
	{
		if(password[i] != (crypt[i]^0xc0)) // 4rch1t3ct!
			ret = 0;
	}

	if(ret)
	{
		mvwprintw(win,4,30,"*** ACCESS GRANTED ! ***");
		mvwprintw(win,5,1,"- 0x08 -DATA RECOVERED- \"The star-laser is nearly ready... Ask them to raise the price, 1000000000000K is'nt enough...\"");
		wattroff(win,COLOR_PAIR(47));
	}
	else
	{
		wattron(win,COLOR_PAIR(197));
		mvwprintw(win,4,30,"*** ACCESS REFUSED ! ***");
		mvwprintw(win,5,1,"DATA DELETION DONE...");
		wattroff(win,COLOR_PAIR(197));
		wattroff(win,COLOR_PAIR(47));
	}

	k = wgetch(win);
	box(win,' ',' ');
	wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(win);
	wrefresh(win);
	delwin(win);	
	return ret;
}
