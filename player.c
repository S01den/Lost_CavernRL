#include "player.h"


int hit(char dirChar, int dirX, int dirY, WINDOW *win_map, Player *player, Weapons *weaponHold, Weapons listWeapons[], Map map[LONGUEUR][LARGEUR], int *gem, Ennemy ennemies[], int nbrEnnemyInFloor, int *component, int *exoticBlood, int *compBefore, int *indiceEnnemy, int *retHit, Pnj pnj[], int nbrPnjInFloor, int *indicePnj)
{
	int ret = 0;

	if(weaponHold->type == 0)
	{
		if(rand()%100 <= weaponHold->efficiency)
		{
			if(map[player->x+dirX][player->y+dirY].content == WALL)
			{
				if(weaponHold->indice == 3) // if we use our fists
				{
					player->life-=2;
				}
				destroyWall(map,player->x+dirX,player->y+dirY,weaponHold, gem);
			}

			*indiceEnnemy = testEnnemyOnPos(ennemies,player->x+dirX,player->y+dirY,nbrEnnemyInFloor);
			if(*indiceEnnemy >= 0)
			{
				*compBefore = *component;
				*retHit = hitEnnemy(*player, &ennemies[*indiceEnnemy], *weaponHold, component, exoticBlood);
				ret = *retHit;
			}
		}
	}
	else if(weaponHold->indice == 1 || weaponHold->indice == 2 || weaponHold->indice == 4 || weaponHold->indice == 5 || weaponHold->indice == 9)
	{
		*retHit = shotLine(map,weaponHold,ennemies,dirChar,*player,nbrEnnemyInFloor,component);
		ret = *retHit;
	}
	else if(listWeapons[weaponHold->indice].type == 2)
	{
		*retHit = burn(map,weaponHold,ennemies,dirChar,*player,nbrEnnemyInFloor,component,win_map);
		ret = *retHit;
	}
	else if(listWeapons[weaponHold->indice].type == 3 && *gem-1 >= 0) // crystal canon
	{
		(*gem)--;
		flashBomb(win_map);
		crystalCanon(map,*player,dirChar);
	}

	*indicePnj = testPnjOnPos(pnj,player->x+dirX,player->y+dirY,nbrPnjInFloor);	

	return ret;
}


int hitEnnemy(Player player, Ennemy *ennemy, Weapons weaponHold, int *components, int *blood)
{
	int ret = 0;
	if(rand()%100 < 100-player.hate)
	{
		if(weaponHold.type == 0)
			ennemy->life -= abs(weaponHold.degat+(int)(player.strength/2));
	}
	else
	{
		ret = 2;
		ennemy->life -= weaponHold.degat*(int)(player.hate/2);
	}
	if(ennemy->life <= 0)
	{
		ret = 1;
		if(ennemy->type == 1)
		{
			int compWin = rand()%10+5;
			*components += compWin;	
		}
		else if(ennemy->type == 2)
		{
			int compWin = rand()%15+7;
			*components += compWin;	
		}
		else if(ennemy->type == 3 || ennemy->type == 4)
		{
			*blood += rand()%10+5;
		}
	}

	return ret;
}

int burn(Map map[LONGUEUR][LARGEUR],Weapons *weaponHold,Ennemy ennemies[], char dir, Player player, int nbrEnnemies, int *components,WINDOW *screen)
{
	int ret = 0;
	int x, y = 0;
	int posX = player.x;
	int posY = player.y;

	weaponHold->state--;
	
	switch(dir)
	{
		case 'z':
			x = -1;
			y = 0;
			break;
		case 's':
			x = 1;
			y = 0;
			break;
		case 'q':
			x = 0;
			y = -1;
			break;
		case 'd':
			x = 0;
			y = 1;
			break;
		case 'a':
			x = -1;
			y = -1;
			break;
		case 'e':
			x = -1;
			y = 1;
			break;
		case 'w':
			x = 1;
			y = -1;
			break;
		case 'x':
			x = 1;
			y = 1;
			break;
	}

	int hit = 0;
	int turn = 0;
	int animationDelay = 0;
	int noGem = 0; // you must pick a gem with contact weapons

	posX += x;
	posY += y;

	while(hit == 0 && turn < weaponHold->range)
	{
		animationDelay = 0;

		wattron(screen,COLOR_PAIR(203));
		mvwprintw(screen,posX+1,posY+1,"o");
		wattroff(screen,COLOR_PAIR(203));
		mvwprintw(screen,posX+1-x,posY+1-y,".");
		wrefresh(screen);
		while(animationDelay < 30000000)
		{
			animationDelay++;
		}

		if(map[posX][posY].typeEnnemy > 0)
		{
			int ennemySeenIndice = testEnnemyOnPos(ennemies,posX,posY,nbrEnnemies);
			if(ennemySeenIndice >= 0)
			{
				ennemies[ennemySeenIndice].life -= weaponHold->degat;

				if(ennemies[ennemySeenIndice].life <= 0)
				{
					ret = 1;
					if(ennemies[ennemySeenIndice].type == 1)
					{
						int compWin = rand()%10+5;
						*components += compWin;	
					}
					else if(ennemies[ennemySeenIndice].type == 2)
					{
						int compWin = rand()%15+7;
						*components += compWin;	
					}
				}

				hit = 1;
			}
		}
		else if(map[posX][posY].content == WALL)
		{
			destroyWall(map, posX, posY, weaponHold, &noGem);
			hit = 1;
		}
		else
		{
			turn++;
			posX += x;
			posY += y;
		}
	}

	int i, j = 0;
	for(i = posX-4; i < posX+4; i++)
	{
		for(j = posY-4; j < posY+4; j++)
		{
			if(rand()%4 < 3)
				map[i][j].effect = 1;
		}
	}

	return ret;
}

void crystalCanon(Map map[LONGUEUR][LARGEUR], Player player, char dir)
{

		int x, y = 0;
		int posX = player.x;
		int posY = player.y;

		switch(dir)
		{
			case 'z':
				x = -1;
				y = 0;
				break;
			case 's':
				x = 1;
				y = 0;
				break;
			case 'q':
				x = 0;
				y = -1;
				break;
			case 'd':
				x = 0;
				y = 1;
				break;
			case 'a':
				x = -1;
				y = -1;
				break;
			case 'e':
				x = -1;
				y = 1;
				break;
			case 'w':
				x = 1;
				y = -1;
				break;
			case 'x':
				x = 1;
				y = 1;
				break;
		}

		while(posX < LONGUEUR && posX > 0 && posY > 0 && posY < LARGEUR)
		{
			map[posX][posY].content = FLOOR;
			posX += x;
			posY += y;
		}
}

int shotLine(Map map[LONGUEUR][LARGEUR],Weapons *weaponHold, Ennemy ennemies[], char dir, Player player, int nbrEnnemies, int *components)
{
	int x, y = 0;
	int posX = player.x;
	int posY = player.y;
	int noGem = 0; // you must pick a gem with contact weapons

	weaponHold->state--;

	switch(dir)
	{
		case 'z':
			x = -1;
			y = 0;
			break;
		case 's':
			x = 1;
			y = 0;
			break;
		case 'q':
			x = 0;
			y = -1;
			break;
		case 'd':
			x = 0;
			y = 1;
			break;
		case 'a':
			x = -1;
			y = -1;
			break;
		case 'e':
			x = -1;
			y = 1;
			break;
		case 'w':
			x = 1;
			y = -1;
			break;
		case 'x':
			x = 1;
			y = 1;
			break;
	}

	int hit = 0;
	int turn = 0;
	int ret = 0;

	while(hit == 0 && turn < weaponHold->range)
	{
		if(map[posX][posY].typeEnnemy > 0 && rand()%101 <= weaponHold->efficiency)
		{
			int ennemySeenIndice = testEnnemyOnPos(ennemies,posX,posY,nbrEnnemies);
			if(ennemySeenIndice >= 0)
			{
				ennemies[ennemySeenIndice].life -= abs(weaponHold->degat+player.agility/4);

				if(ennemies[ennemySeenIndice].life <= 0)
				{
					ret = 1;
					if(ennemies[ennemySeenIndice].type == 1)
					{
						int compWin = rand()%10+5;
						*components += compWin;	
					}
					else if(ennemies[ennemySeenIndice].type == 2)
					{
						int compWin = rand()%15+7;
						*components += compWin;	
					}
				}

				hit = 1;
			}
		}
		else if(map[posX][posY].content == WALL)
		{
			destroyWall(map, posX, posY, weaponHold,&noGem);
			hit = 1;
		}
		else
		{
			turn++;
			posX += x;
			posY += y;
		}

	}

	return ret;
}

void initPlayer(Player *player, Inventory inventory[])
{
	int i = 0;

	player->power = 1;
	player->colorBlind = 0;
	player->state = 0;
	player->state2 = 0;

	player->life = 100;
	player->strength = 11;
	player->hate = 10;
	player->agility = 11;
	player->view = 5;

	player->inventorySize = player->strength+1;

	for(i = 0; i < player->inventorySize; i++)
	{
		inventory[i].type = -1;
		inventory[i].object = 0;
		//inventory[i].number = 0;
	}
}
