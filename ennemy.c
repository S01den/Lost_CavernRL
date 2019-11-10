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
#include "ennemy.h"


void moveRand(Ennemy ennemies[], Map map[LONGUEUR][LARGEUR], int indice) 
{
	if(ennemies[indice].type == 1 || ennemies[indice].type == 3)
	{
		int movX = 2;
		int movY = 2;

		while((movX == 2 || movY == 2) && (ennemies[indice].x+movX <= 0 || ennemies[indice].x+movX >= LONGUEUR) && (ennemies[indice].y+movY <= 0 || ennemies[indice].y+movY >= LARGEUR))
		{
			movX = rand()%5-2;
			movY = rand()%5-2;		
		}
		
		if(ennemies[indice].x+movX >= 0 && ennemies[indice].x+movX <= LONGUEUR && ennemies[indice].y+movY >= 0 && ennemies[indice].y+movY <= LARGEUR)
		{
			if(map[ennemies[indice].x+movX][ennemies[indice].y+movY].content == WALL)
			{
				if(ennemies[indice].x-1 > 0 && ennemies[indice].x+1 < LONGUEUR && ennemies[indice].y+1 > 0 && ennemies[indice].y+1 < LARGEUR)
				{	
					if(map[ennemies[indice].x+1][ennemies[indice].y+movY].content != WALL)
					{
						movX = 1;
						ennemies[indice].x += movX;
						ennemies[indice].y += movY;
					}
					else if(map[ennemies[indice].x-1][ennemies[indice].y+movY].content != WALL)
					{
						if(ennemies[indice].x-1 > 0 && ennemies[indice].x-1 < LONGUEUR && ennemies[indice].y+movY > 0 && ennemies[indice].y+movY < LARGEUR)
						{
							movX = -1;
							ennemies[indice].x += movX;
							ennemies[indice].y += movY;
						}		
					}
					else if(map[ennemies[indice].x][ennemies[indice].y+1].content != WALL)
					{
						if(ennemies[indice].x > 0 && ennemies[indice].x < LONGUEUR && ennemies[indice].y+1 > 0 && ennemies[indice].y+1 < LARGEUR)
						{
							movY = 1;
							ennemies[indice].x += movX;
							ennemies[indice].y += movY;
						}		
					}
					else if(map[ennemies[indice].x][ennemies[indice].y-1].content != WALL)
					{
						if(ennemies[indice].x > 0 && ennemies[indice].x < LONGUEUR && ennemies[indice].y-1 > 0 && ennemies[indice].y-1 < LARGEUR)
						{
							movY = -1;
							ennemies[indice].x += movX;
							ennemies[indice].y += movY;
						}		
					}
				}
			}
			else
			{
				ennemies[indice].x += movX;
				ennemies[indice].y += movY;	
			}
		}

		if(map[ennemies[indice].x][ennemies[indice].y].content == MINES)
		{
			map[ennemies[indice].x][ennemies[indice].y].content = FLOOR;
			ennemies[indice].life -= 20;
		}
	}
}

int testEnnemyOnPos(Ennemy ennemies[], int x, int y, int nbrEnnemies)
{
	int ret = -1;
	int i = 0;

	for(i = 0; i < nbrEnnemies; i++)
	{
		if(ennemies[i].x == x && ennemies[i].y == y && ennemies[i].life > 0)
		{
			ret = i;
			break;
		}
	}

	return ret;
}

void iaCore(Ennemy ennemies[], Player *player, Map map[LONGUEUR][LARGEUR], int *nbrEnnmies, char messagePrint[], int *colorPrint, Armor *armor, WINDOW *win_msg, int *source)
{
// this is a ratio-based IA
// do enemies who can teleport themselves and enemies who can infect the player
	int degats = 0;
	int i = 0;
	int action = 0;

	for(i = 0; i < *nbrEnnmies; i++)
	{
		if(map[ennemies[i].x][ennemies[i].y].effect == 2) // if the enemy is in a radioactive zone
		{
			ennemies[i].life--;
		}
		else if(map[ennemies[i].x][ennemies[i].y].effect == 1 || map[ennemies[i].x][ennemies[i].y].effect == 3) // if the enemy is in a fire zone
		{
			ennemies[i].life-=4;
			ennemies[i].effect = 1;
		}

		if(ennemies[i].effect == 1)
		{
			ennemies[i].life -= 5;
			if(rand()%10 == 0)
			{
				ennemies[i].effect = 0;
			}
		}

		if(map[ennemies[i].x][ennemies[i].y].trap == 1)
		{
			ennemies[i].life -= 5;
			map[ennemies[i].x][ennemies[i].y].trap = 0;
			map[ennemies[i].x][ennemies[i].y].content = FLOOR;
		}
		else if(map[ennemies[i].x][ennemies[i].y].trap == 2)
		{
			ennemies[i].effect = 2;
			map[ennemies[i].x][ennemies[i].y].trap = 0;
			map[ennemies[i].x][ennemies[i].y].content = FLOOR;
			if(rand()%5 == 0)
			{
				ennemies[i].effect = 0;
			}
		}
		else if(map[ennemies[i].x][ennemies[i].y].trap == 3)
		{
			map[ennemies[i].x][ennemies[i].y].trap = 0;
			map[ennemies[i].x][ennemies[i].y].content = FLOOR;
			map[ennemies[i].x][ennemies[i].y+1].effect = 1;
			map[ennemies[i].x][ennemies[i].y-1].effect = 1;
			map[ennemies[i].x+1][ennemies[i].y].effect = 1;
			map[ennemies[i].x-1][ennemies[i].y].effect = 1;	
		}
		
		if(ennemies[i].life > 0 && ennemies[i].effect != 2)
		{
			switch(ennemies[i].type)
			{
				case 1: // droid
					if(ennemies[i].seen == 0)
					{
						moveRand(ennemies,map,i);
					}
					else
					{
						action = rand()%10;
						if(action > 7 || (ennemies[i].life <= 15 && action > 4)) 
						{
							wclear(win_msg);
							repair(ennemies,i, colorPrint, messagePrint);
						}
						else if(action == 0)
						{
							moveRand(ennemies,map,i);
						}
						else
						{
							if(abs(player->x-ennemies[i].x) <= 1 && abs(player->y-ennemies[i].y) <= 1)
							{
								//melee
								if(rand()%101 <= ennemies[i].efficiency)
								{
									int degats = ennemies[i].strength*2 + rand()%3+1 - abs(armor->resistance);
									if(degats > 0)
									{
										armor->state -= degats/3;
										player->life -= degats/2;
										wclear(win_msg);
										*colorPrint = 197; // RED LIKE BLOOOOOOOD
										snprintf(messagePrint,100,"%s HITS YOU WITH HIS ROTATIVE BLADE (-%d LP)",ennemies[i].name,degats/2); 
										*source = 7;
									}
								}
								else
								{
									wclear(win_msg);
									*colorPrint = 47;
									snprintf(messagePrint,100,"%s MISSED YOU !",ennemies[i].name);
								}
							}
							else
							{
								if(ennemies[i].haveBlaster != 1)
								{
									if(abs(player->x - ennemies[i].x) > abs(player->y-ennemies[i].y))
									{
										if(player->x > ennemies[i].x)
										{
											if(map[ennemies[i].x+1][ennemies[i].y].content != WALL && map[ennemies[i].x+1][ennemies[i].y].content != DOOR)
												ennemies[i].x++;
										}
										else
										{
											if(map[ennemies[i].x-1][ennemies[i].y].content != WALL && map[ennemies[i].x-1][ennemies[i].y].content != DOOR)
												ennemies[i].x--;
										}
									}
									else
									{
										if(player->y > ennemies[i].y)
										{
											if(map[ennemies[i].x][ennemies[i].y+1].content != WALL && map[ennemies[i].x][ennemies[i].y+1].content != DOOR)
												ennemies[i].y++;
										}
										else
										{
											if(map[ennemies[i].x][ennemies[i].y-1].content != WALL && map[ennemies[i].x][ennemies[i].y-1].content != DOOR)
												ennemies[i].y--;
										}
									}
								}
								else
								{
									if(rand()%101 <= ennemies[i].efficiency)
									{
										degats = blastHit(ennemies[i],player,armor,map);
										if(degats > 0)
										{
											wclear(win_msg);
											*colorPrint = 197; // RED LIKE BLOOOOOOOD
											snprintf(messagePrint,100,"%s HITS YOU WITH HIS HIS BLASTER (-%d LP)",ennemies[i].name,degats/2);	
											*source = 7;			
										}
									}
									else
									{
										wclear(win_msg);
										*colorPrint = 47;
										snprintf(messagePrint,100,"%s MISSED YOU !",ennemies[i].name);
									}
								}
							}
						}
					}
				break;

				case 2: // sentry
					action = rand()%10;
					if(ennemies[i].seen == 0)
					{
						if(rand()%75 == 0)
						{
							ennemies[i].life++;
						}
					}
					else
					{
						if(ennemies[i].boss != 0)
						{
							if(action <= 6)
							{
								if(rand()%101 <= ennemies[i].efficiency)
								{
									degats = blastHit(ennemies[i],player,armor,map);
									if(degats > 0)
									{
										wclear(win_msg);
										*colorPrint = 197; // RED LIKE BLOOOOOOOD
										snprintf(messagePrint,100,"%s HITS YOU WITH HIS HIS LASER-CANON (-%d LP)",ennemies[i].name,degats/2);
										*source = 8;		
									}
								}
								else
								{
									wclear(win_msg);
									*colorPrint = 47;
									snprintf(messagePrint,100,"%s MISSED YOU !",ennemies[i].name);
								}			
							}
							else if(action == 7)
							{
								int xBomb = rand()%5-2;
								int yBomb = rand()%5-2;
								wclear(win_msg);
								*colorPrint = 197; // RED LIKE BLOOOOOOOD
								snprintf(messagePrint,100,"%s THROWS A BOMB NEAR YOU !",ennemies[i].name);
								map[player->x+xBomb][player->y+yBomb].content = BOMB;
								map[player->x+xBomb][player->y+yBomb].resistance = 4;
							}
							else if(action == 8)
							{
								wclear(win_msg);
								*colorPrint = 47;
								snprintf(messagePrint,100,"********** BUG ! CORE DUMPED ! **********");					
							}
							else
							{
								*colorPrint = 197; // RED LIKE BLOOOOOOOD
								snprintf(messagePrint,100,"*** FIRE MODULE ACTIVATED ! ***");
								map[player->x+rand()%5-2][player->y+rand()%5-2].effect = 1;
							}

						}
						else
						{
							if(action <= 7)
							{
								if(rand()%101 <= ennemies[i].efficiency)
								{
									degats = blastHit(ennemies[i],player,armor,map);
									if(degats > 0)
									{
										wclear(win_msg);
										*colorPrint = 197; // RED LIKE BLOOOOOOOD
										snprintf(messagePrint,100,"%s HITS YOU WITH HIS HIS LASER-CANON (-%d LP)",ennemies[i].name,degats/2);	
										*source = 8;			
									}
								}
								else
								{
									wclear(win_msg);
									*colorPrint = 47;
									snprintf(messagePrint,100,"%s MISSED YOU !",ennemies[i].name);
								}			
							}
							else
							{
								wclear(win_msg);
								repair(ennemies,i, colorPrint, messagePrint);
							}
						}
					}

				break;

			case 3:
					if(ennemies[i].boss == 1) // BOSS 1: ALTHORUS THE GUARDIAN
					{
						if(abs(player->x-ennemies[i].x) <= 2 && abs(player->y-ennemies[i].y) <= 2 && rand()%6 == 0)
						{
							int sign = 1;
							if(player->x > ennemies[i].x)
								sign = 1;
							else
								sign = -1;
							int turns = 0;
							while(turns < 4 && map[player->x][player->y].content != WALL)
							{
								player->x+=sign;
								turns++;
							}
							if(map[player->x][player->y].content == WALL)
							{
								player->life -= 10;
								armor->state -= 10;
								map[player->x][player->y].content = FLOOR;
							}
						}
						else if(abs(player->x-ennemies[i].x) <= 5 && abs(player->y-ennemies[i].y) <= 5 && (*nbrEnnmies)+1 < ENEMIES_MAX && rand()%10 == 1)
						{
							(*nbrEnnmies)++;
							ennemies[i+(*nbrEnnmies)].x = player->x + rand()%1-3;
							ennemies[i+(*nbrEnnmies)].y = player->y + rand()%1-3;
							ennemies[i+(*nbrEnnmies)].type = 3;
							ennemies[i+(*nbrEnnmies)].boss = 0;
							ennemies[i+(*nbrEnnmies)].life = 30;
							ennemies[i+(*nbrEnnmies)].strength = rand()%3+4;
							ennemies[i+(*nbrEnnmies)].seen = 0;
							ennemies[i+(*nbrEnnmies)].agility = 3;
							ennemies[i+(*nbrEnnmies)].haveBlaster = 0;
							ennemies[i+(*nbrEnnmies)].name = "MINI-GUARDIAN";	
						}
						else
						{
							if(abs(player->x - ennemies[i].x) > abs(player->y-ennemies[i].y))
							{
								if(player->x > ennemies[i].x)
								{
									if(map[ennemies[i].x+1][ennemies[i].y].content != WALL && map[ennemies[i].x+1][ennemies[i].y].content != DOOR)
										ennemies[i].x++;
								}
								else
								{
									if(map[ennemies[i].x-1][ennemies[i].y].content != WALL && map[ennemies[i].x-1][ennemies[i].y].content != DOOR)
										ennemies[i].x--;
								}
							}
							else
							{
								if(player->y > ennemies[i].y)
								{
									if(map[ennemies[i].x][ennemies[i].y+1].content != WALL && map[ennemies[i].x][ennemies[i].y+1].content != DOOR)
										ennemies[i].y++;
								}
								else
								{
									if(map[ennemies[i].x][ennemies[i].y-1].content != WALL && map[ennemies[i].x][ennemies[i].y-1].content != DOOR)
										ennemies[i].y--;
								}
							}
						}
					}
					else
					{
						if(abs(player->x-ennemies[i].x) <= 1 && abs(player->y-ennemies[i].y) <= 1)
						{
						//melee
							if(rand()%101 <= ennemies[i].efficiency)
							{
								int degats = ennemies[i].strength*2 + rand()%5 + 3 - armor->resistance;
								if(degats > 0)
								{
									armor->state -= degats/3;
									player->life -= degats/2;
									wclear(win_msg);
									*colorPrint = 197; // RED LIKE BLOOOOOOOD
									snprintf(messagePrint,100,"%s HITS YOU WITH HIS SPEAR (-%d LP)",ennemies[i].name,degats/2); 
									*source = 9;
								}
							}
							else
							{
								wclear(win_msg);
								*colorPrint = 47;
								snprintf(messagePrint,100,"%s MISSED YOU !",ennemies[i].name);
							}
						}
						else
						{
							if(abs(player->x - ennemies[i].x) > abs(player->y-ennemies[i].y))
							{
								if(player->x > ennemies[i].x)
								{
									if(map[ennemies[i].x+1][ennemies[i].y].content != WALL && map[ennemies[i].x+1][ennemies[i].y].content != DOOR)
										ennemies[i].x++;
								}
								else
								{
									if(map[ennemies[i].x-1][ennemies[i].y].content != WALL && map[ennemies[i].x-1][ennemies[i].y].content != DOOR)
										ennemies[i].x--;
								}
							}
							else
							{
								if(player->y > ennemies[i].y)
								{
									if(map[ennemies[i].x][ennemies[i].y+1].content != WALL && map[ennemies[i].x][ennemies[i].y+1].content != DOOR)
										ennemies[i].y++;
								}
								else
								{
									if(map[ennemies[i].x][ennemies[i].y-1].content != WALL&& map[ennemies[i].x][ennemies[i].y-1].content != DOOR)
										ennemies[i].y--;
								}
							}
						}
					}
				break;

				case 4:
						if(abs(player->x-ennemies[i].x) <= 1 && abs(player->y-ennemies[i].y) <= 1)
						{
							if(rand()%3 == 1)
							{
								// explode !
								ennemies[i].life = 0;
								map[ennemies[i].x][ennemies[i].y].content = BOMB;

								int angle = 0;
								int c = 0;
								float ox = 0;
								float oy = 0;
								int tailleRad = 3;

								for(angle = 0; angle < 360; angle++)
								{
									if(oy >= 0 && oy <= LARGEUR && ox <= LONGUEUR && ox >= 0)
									{
										float angleI = cos((float)angle*0.01745f); // 1° = 0.01745 rad because 360° = 2pi rad
										float angleJ = sin((float)angle*0.01745f);
										ox = (float)ennemies[i].x+0.5f;
										oy = (float)ennemies[i].y+0.5f;

										for(c = 0; c < tailleRad && (int)ox+angleI <= LONGUEUR && (int)oy+angleJ <= LARGEUR; c++)
										{
											map[(int)ox][(int)oy].effect = 2;

											ox+=angleI;
											oy+=angleJ;		
										}
									}
								}
							}
						}
						else
						{
							if(abs(player->x - ennemies[i].x) > abs(player->y-ennemies[i].y))
							{
								if(player->x > ennemies[i].x)
								{
									if(map[ennemies[i].x+1][ennemies[i].y].content != WALL && map[ennemies[i].x+1][ennemies[i].y].content != DOOR)
										ennemies[i].x++;
								}
								else
								{
									if(map[ennemies[i].x-1][ennemies[i].y].content != WALL && map[ennemies[i].x-1][ennemies[i].y].content != DOOR)
										ennemies[i].x--;
								}
							}
							else
							{
								if(player->y > ennemies[i].y)
								{
									if(map[ennemies[i].x][ennemies[i].y+1].content != WALL && map[ennemies[i].x][ennemies[i].y+1].content != DOOR)
										ennemies[i].y++;
								}
								else
								{
									if(map[ennemies[i].x][ennemies[i].y-1].content != WALL&& map[ennemies[i].x][ennemies[i].y-1].content != DOOR)
										ennemies[i].y--;
								}
							}
					}
				break;
			}
		}
	}
}

int blastHit(Ennemy enemy, Player *player, Armor *armor, Map map[LONGUEUR][LARGEUR])
{
	int angle = 0;
	int c = 0;
	int hit = 0;
	int degats = 0;

	float ox;
	float oy;
	float k = 0, l = 0;

	for(angle = 0; angle < 360; angle++)
	{
		k = cos((float)angle*0.01745f); // 1° = 0.01745 rad puisque 360° = 2pi rad
		l = sin((float)angle*0.01745f);
		ox = (float)enemy.x+0.5f;
		oy = (float)enemy.y+0.5f;

		if(oy >= 0 && oy <= LARGEUR && ox <= LONGUEUR && ox >= 0 && hit == 0)
		{
			for(c = 0; c < enemy.agility; c++)
			{
				if(player->x == (int)ox && player->y == (int)oy)
				{
					hit = 1;
					degats = enemy.agility*2 +rand()%4+2 - abs(armor->resistance);
					if(degats > 0)
					{
						armor->state -= degats/3;
						player->life -= degats/2;
					}
					if(enemy.type == 2)
					{	
						armor->state -= rand()%5;
						if(rand()%10 == 2)
						{
							if(player->state != 0)
								player->state2 = 2;
							else
								player->state = 2;
						}
						else if(player->state == 2 || player->state2 == 2)
						{
							degats = enemy.agility*2 + rand()%3+1;
							degats -= abs(armor->resistance);
							armor->state -= degats/3;
							player->life -= degats/2;							
						}
					}

					break;
				}
				if(map[(int)ox][(int)oy].content == DOOR)
				{
					break;
				}
				else if(map[(int)ox][(int)oy].content == WALL)
				{
					map[(int)ox][(int)oy].resistance -= degats;
					if(map[(int)ox][(int)oy].resistance <= 0)
					{
						map[(int)ox][(int)oy].resistance = 0;
						map[(int)ox][(int)oy].content = FLOOR;
					}
					break;
				}
				
				ox+=k;
				oy+=l;
			}			
		}

		if(hit == 1)
			break;
	}

	return degats;
}

void repair(Ennemy ennemies[], int indice, int *colorPrint, char messagePrint[])
{
	*colorPrint = 197; // RED LIKE BLOOOOOOOD
	char *body[] = {"HEAD","CORE","ARMS","LEGS"};
	int heal = rand()%3+1+ennemies[indice].agility/3;
	snprintf(messagePrint,100,"%s IS REPARING HIS %s (+%d LP)",ennemies[indice].name,body[rand()%4],heal);
	ennemies[indice].life += heal;
}

void setEnnemies(Map map[LONGUEUR][LARGEUR], Ennemy ennemies[], int nbrEnnemies, int *nbrEnnemyInFloor, int floor)
{
	int nbrEnnemiesRand = rand()%nbrEnnemies+15;
	if(nbrEnnemiesRand > nbrEnnemies)
		nbrEnnemiesRand -= (nbrEnnemies-nbrEnnemiesRand);
	
	int i = 0;
	char name[9];


	for(i = 0; i < nbrEnnemiesRand; i++) // let's spawn the monsters
	{
		ennemies[i].x = rand()%LONGUEUR;
		ennemies[i].y = rand()%LARGEUR;

		if(ennemies[i].x == 0)
			ennemies[i].x++;
		if(ennemies[i].x == LONGUEUR)
			ennemies[i].x--;
		if(ennemies[i].y == 0)
			ennemies[i].y++;
		if(ennemies[i].y == LARGEUR)
			ennemies[i].y--;
		
		moveRand(ennemies,map,i);

		int rollType = rand()%100;

		ennemies[i].effect = 0;

		if(rollType < 40) // droid
		{
			ennemies[i].type = 1;
			ennemies[i].efficiency = 90;
			ennemies[i].life = 20+2*floor;
			ennemies[i].strength = rand()%5+3;
			ennemies[i].seen = 0;
			ennemies[i].agility = rand()%5+3;
			ennemies[i].haveBlaster = rand()%2;
		}
		else if(rollType >= 40 && rollType < 60)
		{
			ennemies[i].type = 2;
			ennemies[i].efficiency = 85;
			ennemies[i].life = 40+2*floor;
			ennemies[i].strength = 3;
			ennemies[i].seen = 0;
			ennemies[i].agility = rand()%7+(3+floor);
			ennemies[i].haveBlaster = 1;			
		}
		else if(rollType >= 60 && rollType < 90)
		{
			ennemies[i].type = 3; 
			ennemies[i].efficiency = 80;
			ennemies[i].life = 30+2*floor;
			ennemies[i].strength = rand()%6+(2+floor);
			ennemies[i].seen = 0;
			ennemies[i].agility = 3;
			ennemies[i].haveBlaster = 0;
			ennemies[i].name = "GUARDIAN";	
		}
		else if(rollType >= 90 && rollType < 100)
		{
			ennemies[i].type = 4;
			ennemies[i].efficiency = 100;
			ennemies[i].life = 15+2*floor;
			ennemies[i].strength = 3;
			ennemies[i].seen = 0;
			ennemies[i].agility = 3;
			ennemies[i].haveBlaster = 0;
			ennemies[i].name = "INFECTED GUARDIAN";	
		}

		if(ennemies[i].type == 1)
		{
			ennemies[i].name = (char*)malloc(11*sizeof(char));
			name[0] = rand()%25+65;
			name[1] = rand()%9+48;
			name[2] = '-';
			name[3] = rand()%25+65;
			name[4] = rand()%9+48;
			name[5] = rand()%9+48;
			name[6] = '@';
			strcpy(ennemies[i].name,name);

			if(strcmp(name,"R2-D26@") == 0)
			{
				strcpy(ennemies[i].name,"D43D-B33F!");
				ennemies[i].life = 300;
				ennemies[i].strength = rand()%50+4;
				ennemies[i].seen = 0;
				ennemies[i].agility = rand()%50+4;
				ennemies[i].haveBlaster = rand()%2;				
			}
		}
		else if(ennemies[i].type == 2)
		{
			ennemies[i].name = (char*)malloc(11*sizeof(char));
			name[0] = 'T';
			name[1] = 'T';
			name[2] = '_';
			name[3] = rand()%9+48;
			name[4] = rand()%9+48;
			name[5] = rand()%9+48;
			name[6] = '&';
			strcpy(ennemies[i].name,name);
		}
		ennemies[i].boss = 0;


	}

	*nbrEnnemyInFloor = nbrEnnemiesRand;
}