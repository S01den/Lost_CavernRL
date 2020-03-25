#include "interface.h"

void printScreenUser(WINDOW *win, Player player, int powerUsed, char diseaseName[20], unsigned int disease, unsigned int nbrEnnemiesKilled)
{
	const char *powers[] = {"Way of life", "Way of power", "Way of protection"};
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	wattron(win,A_UNDERLINE);
	mvwprintw(win,2,(30-strlen(player.name))/2,"%s",player.name);
	wattroff(win,A_UNDERLINE);

	if(powerUsed)
	{
		wattron(win,COLOR_PAIR(197));
		mvwprintw(win,3,5,"%s",powers[player.power]);
		wattroff(win,COLOR_PAIR(197));
	}
	else
	{
		wattron(win,COLOR_PAIR(47));
		mvwprintw(win,3,5,"%s",powers[player.power]);
		wattroff(win,COLOR_PAIR(47));		
	}

	switch(player.state)
	{
		case 0:
			wattron(win,COLOR_PAIR(197));
			mvwprintw(win,4,5,"          ");
			wattroff(win,COLOR_PAIR(197));		
			break;
		case 1:
			wattron(win,COLOR_PAIR(197));
			mvwprintw(win,4,5,"(ON FIRE)");
			wattroff(win,COLOR_PAIR(197));		
			break;
		case 2:
			wattron(win,COLOR_PAIR(197));
			mvwprintw(win,4,5,"(PARALYSED)");
			wattroff(win,COLOR_PAIR(197));		
			break;
		case 3:
			wattron(win,COLOR_PAIR(197));
			mvwprintw(win,4,5,"(STONE)");
			wattroff(win,COLOR_PAIR(197));		
			break;
		case 4:
			wattron(win,COLOR_PAIR(47));
			mvwprintw(win,4,5,"(BOOSTED)");
			wattroff(win,COLOR_PAIR(47));		
			break;
	}

	printBar(win,"LIFE",player.life,6,2,249);
	printBar(win,diseaseName,disease,7,2,248);

	if(disease <= 20)
	{
		mvwprintw(win,8,9,"(STADE 1)");
	}
	if(disease > 20 && disease <= 50)
	{
		mvwprintw(win,8,9,"(STADE 2)");
	}
	if(disease > 50 && disease <= 75)
	{
		mvwprintw(win,8,9,"(STADE 3)");
	}
	if(disease > 75 && disease <= 100)
	{
		mvwprintw(win,8,9,"(STADE 4)");
	}

	mvwprintw(win,10,2,"strength  Hate   Agility");
	mvwprintw(win,11,2," %d         %d      %d  ",player.strength,player.hate,player.agility);
	mvwprintw(win,13,2,"Souls taken: %d",nbrEnnemiesKilled);

	wrefresh(win);
}

void printBar(WINDOW *win, char name[15],int attribute, int x, int y, int colorpair)
{
	unsigned int i = 0;
	mvwprintw(win,x,y,"%s: ",name);

	if(attribute > 0)
	{
		wattron(win,COLOR_PAIR(colorpair));
		for(i = 14+strlen(name); i >= strlen(name)+4; i--)
		{
			// 1 case = 10 pv
			if((i-(strlen(name)+4))*10 <= attribute)
			{
				mvwprintw(win,x,i," ");
			}
		}
		wattroff(win,COLOR_PAIR(colorpair));

		wattron(win,A_BOLD);
		mvwprintw(win,x,strlen(name)+16,"(%d)  ",attribute);
		wattroff(win,A_BOLD);
	}
	else
	{
		wattron(win,A_BOLD);
		mvwprintw(win,x,strlen(name)+16,"(0)  ");
		wattroff(win,A_BOLD);
	}
}

void printEnnemy(WINDOW *win, Ennemy ennemies[], int nbrEnnemyFloor)
{
	box(win,0,0);
	char *classeEnnemy[] = {"???","DROID","SENTRY","GUARDIAN","GUARDIAN"};
	int i = 0;
	int c = 1;
	mvwprintw(win,1,1,"ENEMIES :");
	for(i = 0; i < nbrEnnemyFloor; i++)
	{
		if(ennemies[i].type > 0 && ennemies[i].seen == 1 && ennemies[i].life > 0)
		{
			wattron(win,A_UNDERLINE);
			mvwprintw(win,c+2,5,"%s (%s)",ennemies[i].name,classeEnnemy[ennemies[i].type]);
			wattroff(win,A_UNDERLINE);
			printBar(win,"LIFE",ennemies[i].life,c+4,2,249);
			c+=3;
		}
		/*else
		{
			break;
		}*/
	}

	wrefresh(win);
}

void printWeapon(WINDOW *win, Weapons weaponHold)
{
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(win,1,1,"WEAPON :");
	wattron(win,A_UNDERLINE);
	mvwprintw(win,2,(30-strlen(weaponHold.name))/2,"%s",weaponHold.name);
	wattroff(win,A_UNDERLINE);

	if(weaponHold.type > 0)
	{
		printBar(win,"AMMO",weaponHold.state,4,2,247);
		mvwprintw(win,5,2,"RANGE: %d",weaponHold.range);
	}
	else if(weaponHold.indice != 3)
	{
		printBar(win,"STATE",weaponHold.state,4,2,247);
	}

	mvwprintw(win,6,2,"DAMAGES: %d",weaponHold.degat);

	wrefresh(win);
}

void printInventory(WINDOW *win, int component, int exoticBlood, int gem, Player player, Inventory inventory[], Drug drugList[], Weapons listWeapons[], Armor listArmors[])
{
	char *typeOfDrugs[] = {"VACCIN","STEROID","ANTIDOTE","STIMULANT","HALLUCINOGEN","MEDIPAC","NEUROPARALYSANT"};
	int i = 0;

	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	wattron(win,A_UNDERLINE);
	mvwprintw(win,1,1,"Inventory");
	wattroff(win,A_UNDERLINE);
	mvwprintw(win,2,1,"Electronic components: %d",component);
	mvwprintw(win,3,1,"Exotic blood: %d",exoticBlood);
	mvwprintw(win,4,1,"Gems: %d",gem);

	for(i = 0; i < player.inventorySize; i++)
	{
		if(inventory[i].type == -1)
		{
			mvwprintw(win,6+i,1,"                    ");
		}
		else if(inventory[i].type == 0)
		{
			Weapons weaponHold = listWeapons[inventory[i].object];
			switch(weaponHold.rarity)
			{
				case 1:
					wattron(win,COLOR_PAIR(22));
					if(weaponHold.type == 0)
						mvwprintw(win,6+i,1,"%c) /",49+i);
					else
						mvwprintw(win,6+i,1,"%c) ;",49+i);
					mvwprintw(win, 6+i,2,": %s",weaponHold.name);
					wattroff(win, COLOR_PAIR(22));
					break;
				case 2:
					wattron(win,COLOR_PAIR(166));
					if(weaponHold.type == 0)
						mvwprintw(win,6+i,1,"%c) /",49+i);
					else
						mvwprintw(win,6+i,1,"%c) ;",49+i);
					mvwprintw(win, 6+i,2,": %s",weaponHold.name);
					wattroff(win, COLOR_PAIR(166));
					break;
				case 3:
					wattron(win,COLOR_PAIR(12));
					if(weaponHold.type == 0)
						mvwprintw(win,6+i,1,"%c) /",49+i);
					else
						mvwprintw(win,6+i,1,"%c) ;",49+i);
					mvwprintw(win, 6+i,2,": %s",weaponHold.name);
					wattroff(win, COLOR_PAIR(12));
					break;
			}
		}
		else if(inventory[i].type == 1)
		{
			Armor armorHold = listArmors[inventory[i].object];
			switch(armorHold.rarity)
			{
				case 1:
					wattron(win,COLOR_PAIR(22));
					mvwprintw(win,6+i,1,"%c) ] : %s", 49+i, armorHold.name);
					wattroff(win, COLOR_PAIR(22));
					break;
				case 2:
					wattron(win,COLOR_PAIR(166));
					mvwprintw(win,6+i,1,"%c) ] : %s", 49+i, armorHold.name);
					wattroff(win, COLOR_PAIR(166));
					break;
				case 3:
					wattron(win,COLOR_PAIR(12));
					mvwprintw(win,6+i,1,"%c) ] : %s", 49+i, armorHold.name);
					wattroff(win, COLOR_PAIR(12));
					break;
			}
		}
		else if(inventory[i].type == 2)
		{
			int colorDrug = drugList[inventory[i].object].color;
			int discovered = drugList[inventory[i].object].discovered;
			int typeDrugUsed = drugList[inventory[i].object].type;

			wattron(win,COLOR_PAIR(colorDrug));
			if(discovered == 0)
			{
				mvwprintw(win, 6+i,1,"%c) ! : UNKNOWN DRUG", 49+i);
			}
			else
			{
				mvwprintw(win,6+i,1,"%c) ! : %s", 49+i, typeOfDrugs[typeDrugUsed]);
			}
			wattroff(win,COLOR_PAIR(colorDrug));
		}
		else if(inventory[i].type == 3)
		{
			switch(inventory[i].object)
			{
				case 1:
					wattron(win,COLOR_PAIR(47));
					mvwprintw(win,6+i,1,"%c) * : NUCLEAR CHARGE",49+i);
					wattroff(win,COLOR_PAIR(47));
					break;
				case 2:
					wattron(win,COLOR_PAIR(124));
					mvwprintw(win,6+i,1,"%c) ~ : SHIELD",49+i);
					wattroff(win,COLOR_PAIR(124));
					break;
				case 3:
					wattron(win,COLOR_PAIR(22));
					mvwprintw(win,6+i,1,"%c) = : ELECTRIC TRAP",49+i);
					wattroff(win,COLOR_PAIR(22));
					break;
				case 4:
					wattron(win,COLOR_PAIR(23));
					mvwprintw(win,6+i,1,"%c) = : PARALYSING TRAP",49+i);
					wattroff(win,COLOR_PAIR(23));
					break;
				case 5:
					wattron(win,COLOR_PAIR(COLOR_LAVA));
					mvwprintw(win,6+i,1,"%c) * : LAVA BUCKET",49+i);
					wattroff(win,COLOR_PAIR(COLOR_LAVA));
					break;
				case 6:
					wattron(win,COLOR_PAIR(12));
					mvwprintw(win,6+i,1,"THE CLONE CAPSULE");
					wattroff(win,COLOR_PAIR(12));	
					break;
				default:
					break;
			}
		}
		else
		{
			break;
		}
	}

	wrefresh(win);
}

void setMessage(WINDOW *win, char message[], int colorPair)
{
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	wattron(win,A_UNDERLINE);
	mvwprintw(win,1,1,"MESSAGES:");
	wattroff(win,A_UNDERLINE);

	if(colorPair != 0)
	{
		wattron(win,COLOR_PAIR(colorPair));
		mvwprintw(win,2,1,message);
		wattroff(win,COLOR_PAIR(colorPair));	
	}
	else
	{
		mvwprintw(win,2,1,message);
	}

	wrefresh(win);	
}

void printArmor(WINDOW *win, Armor armor)
{
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(win,1,1,"ARMOR :");
	wattron(win,A_UNDERLINE);
	mvwprintw(win,2,(30-strlen(armor.name))/2,"%s",armor.name);
	wattroff(win,A_UNDERLINE);

	if(armor.indice != 10)
		printBar(win,"STATE",armor.state,4,2,247);
	
	mvwprintw(win,6,2,"RESISTANCE: %d",armor.resistance);

	wrefresh(win);
}
