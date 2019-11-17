#include "map.h"

int generMap(Map map[LONGUEUR][LARGEUR],WINDOW *win) 
// Based on a cellular automata (from http://www.roguebasin.com/index.php?title=Cellular_Automata_Method_for_Generating_Random_Cave-Like_Levels a bit modified)
{
	initMap(map,win);
	srand(time(NULL));
	
	int i = 0;
	int j = 0;
	int x = 0;
	int y = 0;
	int n = 0;
	int stairsAreSet = 0;
	int data = 0;
	int ret = 1;
	int posRandX = 0;
	int posRandY = 0;

	posRandX = rand()%LARGEUR;
	posRandY = rand()%LONGUEUR;
	if(posRandX == 0)
		posRandX++;
	if(posRandX == LARGEUR)
		posRandX--;

	Map map2[LONGUEUR][LARGEUR];

	for(i = 0; i < LARGEUR; i++)
	{
		for(j = 0; j < LONGUEUR; j++)
		{
			if(i == 0 || j == 0 || i == LARGEUR-1 || j == LONGUEUR-1)
			{
				map[j][i].content = WALL;
				map2[j][i].content = WALL;
			}
			else if(i == 1 || j == 1 || i == LARGEUR-2 || j == LONGUEUR-2)
			{
				map[j][i].content = FLOOR;
				map2[j][i].content = FLOOR;
			}	
			else
			{
				data = rand()%3;
				map[j][i].content = data;
				map2[j][i].content = data;
			}
		}
	}

	for(n = 0; n < 5; n++)
	{
		for(i = 1; i < LARGEUR-2; i++)
		{
			for(j = 2; j < LONGUEUR-1; j++)
			{
				int count = 1;
				for(x = i-1; x < i+2; x++)
				{
					for(y = j-2; y < j+1; y++)
					{
						if(map[y][x].content == 2)
						{
							count++;
						}
					}
				}
				if(count >= 5)
				{
					map2[j][i].content = 2;
				}
				else if(map2[j][i].content != 3)
				{
					map2[j][i].content = 1;
				}
			}
		}

		for(i = 0; i < LARGEUR; i++)
		{
			for(j = 0; j < LONGUEUR; j++)
			{
				map[j][i].content = map2[j][i].content;
				

					if(map[j][i].content == WALL)
					{
						map[j][i].resistance = rand()%30+1;
						
						if(n == 4 && rand()%300 == 0 && map[j+1][i].content == WALL && map[j-1][i].content == WALL && map[j+1][i+1].content == WALL && map[j][i-1].content == WALL && map[j][i+1].content == WALL && map[j-1][i-1].content == WALL && map[j-1][i+1].content == WALL && map[j+1][i-1].content == WALL)
						{
							map[j][i].content = FLOOR;
							map[j][i].effect = 3; // set lava
						}
						else if(n == 4 && rand()%(LONGUEUR*LARGEUR/16) == 0)
						{
							map[j][i].effect = 4; // set a gem
						}
					}
					else
					{
						map[j][i].resistance = 0;
						if(rand()%(LONGUEUR*LARGEUR/16) == 0)
						{
							int trapType = rand()%NBR_TYPES_TRAPS;

							switch(trapType)
							{
								case 0: // electric mine
									map[j][i].content = MINES;
									map[j][i].trap = 1;
									break;
								case 1: // paralysant trap
									map[j][i].content = MINES;
									map[j][i].trap = 2;
									break;
								case 2:
									map[j][i].content = MINES;
									map[j][i].trap = 3;
									break;
								default:
									map[j][i].content = FLOOR;
									map[j][i].trap = 0;
									break;
							}
						}
					}
				}
		}	
	}

	while(stairsAreSet < 5)
	{
		posRandX = rand()%LARGEUR;
		posRandY = rand()%LONGUEUR;

		if(posRandX == 0)
			posRandX++;
		if(posRandX == LARGEUR)
			posRandX--;

		stairsAreSet++;
		map[posRandY][posRandX].content = STAIRS;
	}

	int nbrRadZones = rand()%3+4;
	j = 0;
	i = 0;
	int angle = 0;
	int c = 0;
	float ox = 0;
	float oy = 0;

	for(n = 0; n < nbrRadZones; n++)
	{
		int tailleRad = rand()%5+5;
		int xRadioactiveSeed = rand()%LONGUEUR;
		int yRadioactiveSeed = rand()%LARGEUR;

		if(xRadioactiveSeed+tailleRad >= LONGUEUR)
			xRadioactiveSeed -= tailleRad;
		else if(yRadioactiveSeed+tailleRad >= LARGEUR)
			yRadioactiveSeed -= tailleRad;
	
		for(angle = 0; angle < 360; angle++)
		{
			if(oy >= 0 && oy <= LARGEUR && ox <= LONGUEUR && ox >= 0)
			{
				float angleI = cos((float)angle*0.01745f); // 1° = 0.01745 rad because 360° = 2pi rad
				float angleJ = sin((float)angle*0.01745f);
				ox = (float)xRadioactiveSeed+0.5f;
				oy = (float)yRadioactiveSeed+0.5f;

				for(c = 0; c < tailleRad; c++)
				{
					map[(int)ox][(int)oy].effect = 2;

					ox+=angleI;
					oy+=angleJ;		
				}
			}
		}
	 }

	 for(n = 0; n < LARGEUR; n++)
	 {
	 	map[0][n].content = WALL;
	 	map[0][n].trap = 0;
	 	map[0][n].resistance = 2000000000;
	 	map[LONGUEUR-1][n].content = WALL;
	 	map[LONGUEUR-1][n].trap = 0;
	 	map[LONGUEUR-1][n].resistance = 2000000000;
	 }
	 for(n = 0; n < LONGUEUR; n++)
	 {
	 	map[n][LARGEUR-1].content = WALL;
	 	map[n][LARGEUR-1].trap = 0;
	 	map[n][LARGEUR-1].resistance = 2000000000;
	 	map[n][0].content = WALL;
	 	map[n][0].trap = 0;
	 	map[n][0].resistance = 2000000000;
	 }

	return ret;
}

void initMap(Map map[LONGUEUR][LARGEUR], WINDOW *win)
{
	wclear(win);
	box(win,0,0);
	
	int i,j = 0;

	for(i = 0; i < LARGEUR; i++)
	{
		for(j = 0; j < LONGUEUR; j++)
		{
			map[j][i].content = 0;
			map[j][i].seen = 0;
			map[j][i].effect = 0;
			map[j][i].typeEnnemy = 0;
			map[j][i].typePnj = -1;
			map[j][i].trap = 0;
		}
	}
}

void destroyWall(Map map[LONGUEUR][LARGEUR], int x, int y, Weapons *weaponHold, int *gem)
{	
	if(x != 0 && x != LONGUEUR && y != 0 && y != LARGEUR)
	{
		if(map[x][y].resistance-weaponHold->degat > 0)
		{
			map[x][y].resistance -= weaponHold->degat;
			if(weaponHold->type == 0)
				weaponHold->state -= map[x][y].resistance/5+3;
			else
				weaponHold->state--;
		}
		else
		{
			map[x][y].resistance = 0;
			if(map[x][y].effect == 4) // if the player breaks a gem
			{
				(*gem)++;
				map[x][y].content = FLOOR;
			}
			else
			{
				if(rand()%100 <= 90)
					map[x][y].content = FLOOR;
				else
				{
					map[x][y].content = MINES;
					map[x][y].trap = 2;
				}
			}
		}
	}
}

void destroyMap(Map map[LONGUEUR][LARGEUR], Player player)
{
	int x = rand()%LONGUEUR;
	int y = rand()%LARGEUR;
	int i = 0;
	int nbrDestroyed = rand()%3+1;

	if(x-5 <= 0)
		x+=5;
	if(y-5 <= 0)
		y+=5;
	if(x+5 >= LONGUEUR)
		x-=5;
	if(y+5 <= LARGEUR)
		y-=5;
	
	if((map[x][y].content >= 5 && map[x][y].content <= 9) || (x == player.x && y == player.y))
	{
		x = rand()%LONGUEUR;
		y = rand()%LARGEUR;

		if(x-5 <= 0)
			x+=5;
		if(y-5 <= 0)
			y+=5;
		if(x+5 >= LONGUEUR)
			x-=5;
		if(y+5 <= LARGEUR)
			y-=5;	
	}

	for(i = 0; i < nbrDestroyed; i++)
	{
		if(map[x][y].content != STAIRS)
			map[x][y].content = HOLE;
		x += 1-rand()%3;
		y += 1-rand()%3;
	}
}
