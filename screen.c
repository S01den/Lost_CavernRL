#include "screen.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <curses.h>
#include <termios.h>
#include <termio.h>
#include <sgtty.h> 
#include <signal.h>
#include "ennemy.h"
#include "item.h"
#include "pnj.h"


void calcFOV(Map map[LONGUEUR][LARGEUR], int view, int posX, int posY, WINDOW *win_map, Ennemy ennemies[], int nbrEnnemies, int *ennemyPresent, int colorBlind, int stone, Drug drugList[], int nbrDrugs, Weapons weaponOnFloor[], int nbrWeapons, Armor armorOnFloor[], int nbrArmors, Pnj pnjOnFloor[], int nbrPnjOnFloor)
{
	const char tileset_ennemies[] = "dtgn";
	const char tileset_pnj[] = "cwes";
	const char tileset[] = " .#>=!+]/;* ?";

	float i = 0;
	float j = 0;
	int angle = 0; // en radian
	float ox,oy = 0;
	int c = 0;
	int colorWeapon = 22;
	int colorArmor = 22;

	for(angle = 0; angle < nbrEnnemies; angle++)
	{
		ennemies[angle].seen = 0;
	}

	for(angle = 0; angle < 360; angle++)
	{
		i = cos((float)angle*0.01745f); // 1° = 0.01745 rad because 360° = 2pi rad
		j = sin((float)angle*0.01745f);
		ox = (float)posX+0.5f;
		oy = (float)posY+0.5f;

			for(c = 0; c < view && (int)ox+i <= LONGUEUR && (int)ox+i >= -1 && (int)oy+j >= -1 && (int)oy+j <= LARGEUR;c++)
			{
				if(!stone)
				{
					if(map[(int)ox][(int)oy].effect == 0)
					{
						wattron(win_map,A_BOLD);
						mvwprintw(win_map,(int)ox+1,(int)oy+1,"%c",tileset[map[(int)ox][(int)oy].content]);
						wattroff(win_map,A_BOLD);					
					}
					else if(map[(int)ox][(int)oy].effect == 1) // on fire
					{
						if(rand()%300 <= 298)
						{
							if(rand()%4 == 3)
							{
								wattron(win_map,COLOR_PAIR(10));
								mvwprintw(win_map,(int)ox+1,(int)oy+1,"%c",tileset[map[(int)ox][(int)oy].content]);
								wattroff(win_map,COLOR_PAIR(10));		
							}
							else
							{
								wattron(win_map,COLOR_PAIR(203));
								mvwprintw(win_map,(int)ox+1,(int)oy+1,"%c",tileset[map[(int)ox][(int)oy].content]);
								wattroff(win_map,COLOR_PAIR(203));
							}					
						}
						else
						{
							map[(int)ox][(int)oy].effect = 0;
							wattron(win_map,A_BOLD);
							mvwprintw(win_map,(int)ox+1,(int)oy+1,"%c",tileset[map[(int)ox][(int)oy].content]);
							wattroff(win_map,A_BOLD);						
						}

						if(map[(int)ox][(int)oy].content == WALL && rand()%10 == 2)
						{
							if(map[(int)ox][(int)oy].resistance-1 > 0)
							{
								map[(int)ox][(int)oy].resistance--;
							}
							else
							{
								map[(int)ox][(int)oy].resistance = 0;
								if(rand()%100 <= 90)
									map[(int)ox][(int)oy].content = FLOOR;
								else
								{
									map[(int)ox][(int)oy].content = MINES;
									map[(int)ox][(int)oy].trap = 1;
								}
							}
						}
					}
					else if(map[(int)ox][(int)oy].effect == 2) // radioactive
					{
						wattron(win_map,COLOR_PAIR(83));
						mvwprintw(win_map,(int)ox+1,(int)oy+1,"%c",tileset[map[(int)ox][(int)oy].content]);
						wattroff(win_map,COLOR_PAIR(83));				
					}
					else if(map[(int)ox][(int)oy].effect == 3) // lava
					{
						if(rand()%4 == 0)
						{
							wattron(win_map,COLOR_PAIR(203));
							mvwprintw(win_map,(int)ox+1,(int)oy+1,"*");
							wattroff(win_map,COLOR_PAIR(203));
						}
						else
						{
							wattron(win_map,COLOR_PAIR(COLOR_LAVA));
							mvwprintw(win_map,(int)ox+1,(int)oy+1,"*");
							wattroff(win_map,COLOR_PAIR(COLOR_LAVA));	
						}			
					}
					else if(map[(int)ox][(int)oy].effect == 4) // gem
					{
						wattron(win_map,COLOR_PAIR(124));
						mvwprintw(win_map,j+1,i+1,"#");
						wattroff(win_map,COLOR_PAIR(124));				
					}

					map[(int)ox][(int)oy].seen = 1;

					int testPnj = testPnjOnPos(pnjOnFloor,(int)ox,(int)oy,nbrPnjOnFloor);
					if(testPnj >= 0)
					{
							wattron(win_map,COLOR_PAIR(STAIRS_COLOR));
							mvwprintw(win_map,(int)ox+1,(int)oy+1,"%c",tileset_pnj[pnjOnFloor[testPnj].type]);
							wattroff(win_map,COLOR_PAIR(STAIRS_COLOR));	
							map[(int)ox][(int)oy].typePnj = pnjOnFloor[testPnj].type;
					}

					int testEnnemy = testEnnemyOnPos(ennemies,(int)ox,(int)oy,nbrEnnemies);
					
					if(testEnnemy >= 0 && ennemies[testEnnemy].life > 0 && ennemies[testEnnemy].type > 0)
					{
						*ennemyPresent = 1;
						ennemies[testEnnemy].seen = 1;
						map[(int)ox][(int)oy].typeEnnemy = ennemies[testEnnemy].type;
						if(ennemies[testEnnemy].effect == 1)
						{
							wattron(win_map,COLOR_PAIR(BURN_EFFECT));
							if(ennemies[testEnnemy].boss == 0)
								mvwprintw(win_map,(int)ox+1,(int)oy+1,"%c",tileset_ennemies[map[(int)ox][(int)oy].typeEnnemy-1]);
							else
								mvwprintw(win_map,(int)ox+1,(int)oy+1,"%c",tileset_ennemies[map[(int)ox][(int)oy].typeEnnemy-1]-32);
							wattroff(win_map,COLOR_PAIR(BURN_EFFECT));
						}
						else
						{
							wattron(win_map,COLOR_PAIR(ENEMY_COLORS+1));
							if(ennemies[testEnnemy].boss == 0)
								mvwprintw(win_map,(int)ox+1,(int)oy+1,"%c",tileset_ennemies[map[(int)ox][(int)oy].typeEnnemy-1]);
							else
								mvwprintw(win_map,(int)ox+1,(int)oy+1,"%c",tileset_ennemies[map[(int)ox][(int)oy].typeEnnemy-1]-32);
							wattroff(win_map,COLOR_PAIR(ENEMY_COLORS+1));
						}
					}
					else
					{
						map[(int)ox][(int)oy].typeEnnemy = 0;
					}

					if(map[(int)ox][(int)oy].typeEnnemy == 0)
					{
							if(map[(int)ox][(int)oy].content == WALL)
							{
								if(colorBlind == 0 && map[(int)ox][(int)oy].effect == 0)
								{
									switch(map[(int)ox][(int)oy].resistance/10)
									{
										case 0:
											wattron(win_map,COLOR_PAIR(COLOR_RESIST_1));
											mvwprintw(win_map,(int)ox+1,(int)oy+1,"#");
											wattroff(win_map,COLOR_PAIR(COLOR_RESIST_1));
											break;
										case 1:
											wattron(win_map,COLOR_PAIR(COLOR_RESIST_2));
											mvwprintw(win_map,(int)ox+1,(int)oy+1,"#");
											wattroff(win_map,COLOR_PAIR(COLOR_RESIST_2));
											break;
										case 2:
											wattron(win_map,COLOR_PAIR(COLOR_RESIST_3));
											mvwprintw(win_map,(int)ox+1,(int)oy+1,"#");
											wattroff(win_map,COLOR_PAIR(COLOR_RESIST_3));
											break;
										default:
											wattron(win_map,COLOR_PAIR(COLOR_RESIST_3));
											mvwprintw(win_map,(int)ox+1,(int)oy+1,"#");
											wattroff(win_map,COLOR_PAIR(COLOR_RESIST_3));
											break;
									}
								}
								else if(colorBlind == 1)
								{
									wattron(win_map,A_DIM);
									mvwprintw(win_map,(int)ox+1,(int)oy+1,"#");
									wattroff(win_map,A_DIM);
								}
								else if(map[(int)ox][(int)oy].effect == 2)
								{
									wattron(win_map,COLOR_PAIR(83));
									mvwprintw(win_map,(int)ox+1,(int)oy+1,"#");
									wattroff(win_map,COLOR_PAIR(83));
								}
								break;
							}
							else if(map[(int)ox][(int)oy].content == STAIRS)
							{
								wattron(win_map,COLOR_PAIR(STAIRS_COLOR));
								mvwprintw(win_map,(int)ox+1,(int)oy+1,">");
								wattroff(win_map,COLOR_PAIR(STAIRS_COLOR));		
							}
							else if(map[(int)ox][(int)oy].content == COMPUTER)
							{
								wattron(win_map,COLOR_PAIR(STAIRS_COLOR));
								mvwprintw(win_map,(int)ox+1,(int)oy+1,"?");
								wattroff(win_map,COLOR_PAIR(STAIRS_COLOR));					
							}
							else if(map[(int)ox][(int)oy].content == HOLE)
							{
								wattron(win_map,COLOR_PAIR(COLOR_HOLE));
								mvwprintw(win_map,(int)ox+1,(int)oy+1," ");
								wattroff(win_map,COLOR_PAIR(COLOR_HOLE));	
							}
							else if(map[(int)ox][(int)oy].content == MINES && map[(int)ox][(int)oy].trap > 0)
							{
								wattron(win_map,COLOR_PAIR(map[(int)ox][(int)oy].trap+21));
								mvwprintw(win_map,(int)ox+1,(int)oy+1,"=");
								wattroff(win_map,COLOR_PAIR(map[(int)ox][(int)oy].trap+21));		
							}
							else if(map[(int)ox][(int)oy].content == DRUG)
							{
								int drug = testDrug(drugList,(int)ox,(int)oy,nbrDrugs);
								if(drug >= 0)
								{
									//mvwprintw(win_map,0,0,"%d",drugList[drug].type);
									wattron(win_map,COLOR_PAIR(drugList[drug].color));
									mvwprintw(win_map,(int)ox+1,(int)oy+1,"!");
									wattroff(win_map,COLOR_PAIR(drugList[drug].color));					
								}
								else
								{
									map[(int)ox][(int)oy].content = FLOOR;
								}

							}
							else if(map[(int)ox][(int)oy].content == CONTACT_WEAPON)
							{
								int weapon = testWeapon(weaponOnFloor,(int)ox,(int)oy,nbrWeapons);
								switch(weaponOnFloor[weapon].rarity)
								{
									case 1:
										colorWeapon = 22;
										break;
									case 2:
										colorWeapon = 166;
										break;
									case 3:
										colorWeapon = 12;
										break;
									default:
										colorWeapon = 22;
										break;
								}
								if(weapon >= 0)
								{
									wattron(win_map,COLOR_PAIR(colorWeapon));
									mvwprintw(win_map,(int)ox+1,(int)oy+1,"/");
									wattroff(win_map,COLOR_PAIR(colorWeapon));
								}	
								else
								{
									map[(int)ox][(int)oy].content = FLOOR;
								}
							}
							else if(map[(int)ox][(int)oy].content == DIST_WEAPON)
							{
								int weapon = testWeapon(weaponOnFloor,(int)ox,(int)oy,nbrWeapons);
								switch(weaponOnFloor[weapon].rarity)
								{
									case 1:
										colorWeapon = 22;
										break;
									case 2:
										colorWeapon = 166;
										break;
									case 3:
										colorWeapon = 12;
										break;
									default:
										colorWeapon = 22;
										break;
								}
								if(weapon >= 0)
								{
									wattron(win_map,COLOR_PAIR(colorWeapon));
									mvwprintw(win_map,(int)ox+1,(int)oy+1,";");
									wattroff(win_map,COLOR_PAIR(colorWeapon));	
								}
								else
								{
									map[(int)ox][(int)oy].content = FLOOR;
								}
							}
							else if(map[(int)ox][(int)oy].content == ARMOR)
							{
								int armor = testArmor(armorOnFloor, (int)ox, (int)oy, nbrArmors);
								switch(armorOnFloor[armor].rarity)
								{
									case 1:
										colorArmor = 22;
										break;
									case 2:
										colorArmor = 166;
										break;
									case 3:
										colorArmor = 12;
										break;
									default:
										colorArmor = 22;
										break;
								}
								if(armor >= 0)
								{
									wattron(win_map,COLOR_PAIR(colorArmor));
									mvwprintw(win_map,(int)ox+1,(int)oy+1,"]");
									wattroff(win_map,COLOR_PAIR(colorArmor));	
								}
								else
								{
									map[(int)ox][(int)oy].content = FLOOR;
								}	
							}
						}
				}
				else
				{
					int colorRand = rand()%256;
					wattron(win_map,COLOR_PAIR(colorRand));
					mvwprintw(win_map,(int)ox+1,(int)oy+1,"%c",tileset[map[(int)ox][(int)oy].content]);
					wattroff(win_map,COLOR_PAIR(colorRand));
					map[(int)ox][(int)oy].seen = 1;
				}			

				ox+=i;
				oy+=j;
			}
	}

}

void delay(int timeDelay)
{
	int i = 0;
	for(i = 0; i < timeDelay*10000000; i++){}
}

void flashBomb(WINDOW *win_map)
{
	int k = 0;
	int l = 0;
	
	for(k = 1; k < LONGUEUR-1; k++)
	{
		for(l = 1; l < LARGEUR-1; l++)
		{
			wattron(win_map,COLOR_PAIR(244));
			mvwprintw(win_map,k,l," ");
			wattroff(win_map,COLOR_PAIR(244));
		}
	}
	
	wrefresh(win_map);
	delay(15);
	for(k = 1; k < LONGUEUR-1; k++)
	{
		for(l = 1; l < LARGEUR-1; l++)
		{
			wattron(win_map,COLOR_BLACK);
			mvwprintw(win_map,k,l," ");
			wattroff(win_map,COLOR_BLACK);
		}
	}
	wrefresh(win_map);
	delay(9);
}


void recalcScreen(WINDOW *win_map, Map map[LONGUEUR][LARGEUR], int nbrEnnemies, Ennemy ennemies[])
{
	const char tileset_ennemies[] = "dtgn";
	const char tileset[] = " .#>=!+]/;* ?";
	const char tileset_pnj[] = "cwes";
	
	int j = 0;
	int i = 0;

	for(i = 0; i < LARGEUR; i++)
	{
		for(j = 0; j < LONGUEUR; j++)
		{
			if(map[j][i].seen == 1)
			{
				if(map[j][i].typeEnnemy == 0)
				{
					if(map[j][i].content == STAIRS)
					{
						wattron(win_map,A_BOLD);
						mvwprintw(win_map,j+1,i+1,"%c",tileset[map[j][i].content]);
						wattroff(win_map,A_BOLD);
					}
					else if(map[j][i].content == HOLE)
					{
						wattron(win_map,COLOR_PAIR(COLOR_HOLE));
						mvwprintw(win_map,j+1,i+1," ");
						wattroff(win_map,COLOR_PAIR(COLOR_HOLE));	
					}
					else if(map[j][i].content == MINES || map[j][i].content == DIST_WEAPON || map[j][i].content == CONTACT_WEAPON || map[j][i].content == DRUG || map[j][i].content == COMPUTER)
					{
						wattron(win_map,A_BOLD);
						mvwprintw(win_map,j+1,i+1,"%c",tileset[map[j][i].content]);
						wattroff(win_map,A_BOLD);		
					}
					else if(map[j][i].content == DOOR)
					{
						wattron(win_map,COLOR_PAIR(15));
						mvwprintw(win_map,j+1,i+1,"%c",tileset[map[j][i].content]);
						wattroff(win_map,COLOR_PAIR(15));			
					}
					else if(map[j][i].content == PNJ)
					{
						wattron(win_map,A_BOLD);
						mvwprintw(win_map,j+1,i+1,"%c",tileset_pnj[map[j][i].typePnj]);
						wattroff(win_map,A_BOLD);
					}				
					else if(map[j][i].content == BOMB)
					{
						wattron(win_map,A_BOLD);
						mvwprintw(win_map,j+1,i+1,"%c",tileset[map[j][i].content]);
						wattroff(win_map,A_BOLD);
						if(map[j][i].resistance-1 > 0)
						{
							map[j][i].resistance--;
						}
						else // EXPLOSION !!!
						{
							map[j][i].resistance = 0;
							map[j][i].content = FLOOR;
							map[j][i].effect = 2;
							if(i > 0)
								map[j][i-1].effect = 2;
							if(i < LARGEUR-1)
								map[j][i+1].effect = 2;
							if(j > 0)
								map[j-1][i].effect = 2;		
							if(j < LONGUEUR-1)
								map[j+1][i].effect = 2;

							int k = 0;
							int l = 0;

							int minJ = j-5;
							if(minJ < 0)
								minJ = 0;
							int maxJ = j+5;
							if(maxJ >= LONGUEUR)
								minJ = LONGUEUR-1;
							int minI = i-5;
							if(minI < 0)
								minI = 0;
							int maxI = i+5;
							if(maxI >= LARGEUR)
								maxI = LARGEUR-1;

							for(k = minJ; k <= maxJ; k++)
							{
								for(l = minI; l <= maxI; l++)
								{
									if(testEnnemyOnPos(ennemies,k,l,nbrEnnemies) >= 0)
									{
										ennemies[testEnnemyOnPos(ennemies,k,l,nbrEnnemies)].life -= 21+rand()%10;
									}
									if(rand()%4 <= 2)
									{
										map[k][l].resistance = 0;
										map[k][l].content = FLOOR;
									}
									if(rand()%5 == 0) //set fire
										map[k][l].effect = 1;	
								}
							}	

							flashBomb(win_map);
						}
					}
					else if(map[j][i].effect == 0)
					{
						wattron(win_map,A_DIM);
						mvwprintw(win_map,j+1,i+1,"%c",tileset[map[j][i].content]);
						wattroff(win_map,A_DIM);
					}
					else if(map[j][i].effect == 1) // fire
					{
						if(rand()%300 <= 298)
						{
							if(rand()%4 == 3)
							{
								wattron(win_map,COLOR_PAIR(10));
								mvwprintw(win_map,j+1,i+1,"%c",tileset[map[j][i].content]);
								wattroff(win_map,COLOR_PAIR(10));		
							}
							else
							{
								wattron(win_map,COLOR_PAIR(203));
								mvwprintw(win_map,j+1,i+1,"%c",tileset[map[j][i].content]);
								wattroff(win_map,COLOR_PAIR(203));
							}					
						}
						else
						{
							map[j][i].effect = 0;
							wattron(win_map,A_BOLD);
							mvwprintw(win_map,j+1,i+1,"%c",tileset[map[j][i].content]);
							wattroff(win_map,A_BOLD);						
						}

						if(map[j][i].content == WALL && rand()%10 == 2)
						{
							if(map[j][i].resistance-1 > 0)
							{
								map[j][i].resistance--;
							}
							else
							{
								map[j][i].resistance = 0;
								if(rand()%100 <= 90)
									map[j][i].content = FLOOR;
								else
								{
									map[j][i].content = MINES;
									map[j][i].trap = 1;
								}
							}
						}						
					}
					else if(map[j][i].effect == 2) // radioactive
					{
						wattron(win_map,COLOR_PAIR(41));
						mvwprintw(win_map,j+1,i+1,"%c",tileset[map[j][i].content]);
						wattroff(win_map,COLOR_PAIR(41));				
					}
					else if(map[j][i].effect == 4) // gem
					{
						wattron(win_map,COLOR_PAIR(124));
						mvwprintw(win_map,j+1,i+1,"%c",tileset[map[j][i].content]);
						wattroff(win_map,COLOR_PAIR(124));				
					}
					else if(map[j][i].effect == 3) // lava
					{
						if(rand()%4 == 0)
						{
							wattron(win_map,COLOR_PAIR(203));
							mvwprintw(win_map,j+1,i+1,"*");
							wattroff(win_map,COLOR_PAIR(203));
						}
						else
						{
							wattron(win_map,COLOR_PAIR(COLOR_LAVA));
							mvwprintw(win_map,j+1,i+1,"*");
							wattroff(win_map,COLOR_PAIR(COLOR_LAVA));
						}

						int counterX = 0;
						int counterY = 0;

						for(counterX = -1; counterX <= 1; counterX++) // the lava flow
						{
							for(counterY = -1; counterY <= 1; counterY++)
							{
								if(rand()%10 == 0)
								{
									if(map[j+counterX][i+counterY].content == FLOOR || map[j+counterX][i+counterY].content == CONTACT_WEAPON || map[j+counterX][i+counterY].content == DIST_WEAPON)
									{
										map[j+counterX][i+counterY].effect = 3;
									}
								}
							}
						}		
					}				
				}
				else
				{
					int ennemy = testEnnemyOnPos(ennemies,j,i,nbrEnnemies);
					if(ennemy >= 0)
					{
						ennemies[ennemy].seen = 0;
						wattron(win_map,A_BOLD);
						mvwprintw(win_map,j+1,i+1,"%c",tileset_ennemies[map[j][i].typeEnnemy-1]);
						wattroff(win_map,A_BOLD);
					}
				}
			}				
		}
	}
}
