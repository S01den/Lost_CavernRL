#include "common.h"

// Lost Cavern; A roguelike by S01den
/*
 |                |      ___|                            
 |      _ \   __| __|   |      _` |\ \   / _ \  __| __ \ 
 |     (   |\__ \ |     |     (   | \ \ /  __/ |    |   |
_____|\___/ ____/\__|  \____|\__,_|  \_/ \___|_|   _|  _|
                                                         */
// ----------------------------- TO DO LIST -----------------------------
// CODER LE SECRET (-> ARCHITECT (META))
// ----------------------------------------------------------------------

//idée de concept: on incarne un écolo radical qui se bat contre une entreprise multiplanetaire (MultiplexCorp) qui detruit une caverne ancestrale pour recolter des gems. 
//cette entreprise est dirigée par un mystérieux personnage appelé "l'Architecte" (qui est également le boss de fin)
//les gardiens sont des clones au service de MultiplexCorp
//pas de classe mais un pouvoir qui dépend de la haine, parmi deux à choisir, se regen à chaque étage:
//absorption (permet de transformer x% de la maladie en vie, x depend de la haine), puissance (permet d'augmenter la force du perso pendant 2 tours)

// ******** TRICKS & TIPS *******
// you must pick gems with a contact weapon
// use the shields !
// Fire cancel radioactivity
// block a turret with shield and then kill it easily with distance weapons
// the resistance of crystal armor and the degats of the crystal sword depends on the number of gems the player own.

// HIGH SCORE perso: 1852

void quit();

int main(int argc, char **argv)
{
	srand(time(NULL));

	Player player;
	char name[20];
	int nameLen = 0;
	for(nameLen = 0; nameLen < 20; nameLen++)
		name[nameLen] = 0;

	int powerUsed = 0;
	int score = 0;
	int nbrQuestsDone = 0;
	int source = 0;

	char *diseaseName[] = {"Yurix B","H9YB-3","Zoko"}; // yurix B = parasite extraterrestre ; H9YB-3 = Virus sythétisé par le Multiplex ; Zoko = Malédiction
	// yurix B = AGRESSIVE, A LOT OF DEGATS ; H9YB-3 = COLOR BLIND AND VIEW; ZOKO = DECREASE STATS AND CAN PARALYSE
	char *typeDisease[] = {"parasite","virus","curse"};
	char *classEnemy[] = {"???","DROID","SENTRY","GUARDIAN","INFECTED GUARDIAN"};
	char *bossNames[] = {"Althorus","GH-28","The Architect"};
	char *body[] = {"HEAD","BODY","ARMS","LEGS"};

	int nbrWeaponMax = WEAPON_MAX;
	int nbrWeapons = 0;
	int nbrArmorMax = ARMOR_MAX;
	int nbrArmors = 0;

	Weapons listWeapons[NBR_WEAPONS];
	Weapons weaponsOnFloor[WEAPON_MAX];
	Weapons weaponHold;

	Armor armorHold;
	Armor listArmors[NBR_ARMORS];
	Armor armorOnFloor[ARMOR_MAX];

	Pnj pnjOnFloor[NBR_PNJ_MAX];

	double diseaseLevel = 0;
	int diseaseIndice = rand()%3;

	unsigned int nbrEnnemiesKilled = 0;
	int component = 0;
	int exoticBlood = 0;
	int gem = 0;

	int ennemyPresent = 0;

	Inventory inventory[INVENTORY_MAX]; 
	initPlayer(&player, inventory);

	int floor = 1;
	int i = 0;
	int s = 0;
	int indiceEnnemy = 0;
	int killTurret = 0;
	int killGuardian = 0;
	int killDroid = 0;

// Initialisation
	initscr(); 
	raw();
	noecho();
	curs_set(0); // on eteint le curseur
	start_color();
	init_color(COLOR_RED,800,0,0);
	for(i = 0; i < 255; i++)
	{
		if(i >= ENEMY_COLORS)
		{
			init_pair(i + 1, i/3-4, COLOR_RED);
		}
		else if(i+1 >= COLOR_DRUGS-1 && i+1 <= COLOR_DRUGS+NBR_DRUGS)
		{
			init_pair(i + 1, (rand()%(100+1))*2, COLOR_BLACK);
		}
		else
		{
			init_pair(i + 1, i, COLOR_BLACK);
		}
	}

	init_pair(244,COLOR_WHITE,COLOR_WHITE);
	init_pair(COLOR_LAVA,1,COLOR_BLACK);
	init_pair(BURN_EFFECT,COLOR_RED,203);
	init_pair(247,0,COLOR_BLUE);
	init_pair(248,COLOR_RED,COLOR_RED);
	init_pair(249,COLOR_GREEN,COLOR_GREEN);
	init_pair(COLOR_BH,COLOR_RED, COLOR_WHITE);
	init_pair(COLOR_MEDIC,COLOR_RED, COLOR_WHITE);
	init_pair(COLOR_TRACER,COLOR_RED, COLOR_WHITE);
	init_pair(COLOR_PACIFICATOR,COLOR_RED, COLOR_WHITE);
	init_pair(STAIRS_COLOR,COLOR_BLACK,COLOR_WHITE); 

	int xScreen = 0, yScreen = 0;
	getmaxyx(stdscr, xScreen, yScreen); // 55 204 for my screen

	WINDOW *win_Begin;
	win_Begin = newwin(LONGUEUR, 60,xScreen/5,yScreen/3);

	// ************** MAIN MENU *****************
	keypad(win_Begin, TRUE);

	wattron(win_Begin,COLOR_PAIR(124));
	mvwprintw(win_Begin,1,1," |                |      ___|                            ");
	mvwprintw(win_Begin,2,1," |      _ \\   __| __|   |      _` |\\ \\   / _ \\  __| __ \\ ");
	mvwprintw(win_Begin,3,1," |     (   |\\__ \\ |     |     (   | \\ \\ /  __/ |    |   |");
	mvwprintw(win_Begin,4,1,"_____|\\___/ ____/\\__|  \\____|\\__,_|  \\_/ \\___|_|   _|  _|");
	mvwprintw(win_Begin,6,19,"(A RogueLike by S01den)");
	wattroff(win_Begin,COLOR_PAIR(124));

	mvwprintw(win_Begin,12,1,"----------------------------------------------------------");
	mvwprintw(win_Begin,14,1,"----------------------------------------------------------");
	mvwprintw(win_Begin,13,1,"| Enter your name:                                       |");

	int keyName = 0;
	nameLen = 0;
	while(keyName != '\n')
	{
		keyName = wgetch(win_Begin);
		if(keyName == KEY_BACKSPACE && nameLen > 0)
		{
			player.name[nameLen-1] = ' ';
			nameLen--;
		}
		else if(keyName != '\n' && nameLen <= 14)
		{
			player.name[nameLen] = keyName;
			nameLen++;
		}
		mvwprintw(win_Begin,13,20,"%s",player.name);
		wrefresh(win_Begin);
	}

	mvwprintw(win_Begin,16,0,"Hi %s, we were waiting for you.",name);
	mvwprintw(win_Begin,17,0,"Unfortunately, you were infected by the %s",typeDisease[diseaseIndice]);
	wattron(win_Begin,COLOR_PAIR(197));
	mvwprintw(win_Begin,17,50,"%s",diseaseName[diseaseIndice]);
	wattroff(win_Begin,COLOR_PAIR(197));
	mvwprintw(win_Begin,18,0,"However; we,");
	wattron(win_Begin,COLOR_PAIR(124));
	mvwprintw(win_Begin,18,13,"the Gem's priests");
	wattroff(win_Begin,COLOR_PAIR(124));
	mvwprintw(win_Begin,18,31,"offer you a special power\nFollow one of the two ways:");
	wattron(win_Begin,A_BOLD);
	mvwprintw(win_Begin,21,0,"1) Way of power (INCREASES STRENGTH FOR ONE TURN)");
	mvwprintw(win_Begin,22,0,"2) Way of life (CONVERT DISEASE TO LIFE POINTS)");
	wattroff(win_Begin,A_BOLD);

	int choiceWay = wgetch(win_Begin);
	if(choiceWay == '1')
		player.power = 1;
	else
		player.power = 0;
	if(rand()%2000 == 42)
	{
		player.view++;
		player.strength++;
		player.agility++;
		wattron(win_Begin,COLOR_PAIR(12));
		mvwprintw(win_Begin,23,0,"Congratulation ! You're blessed by the God of the Gem, the Cosmic Octopus !");
		wattroff(win_Begin,COLOR_PAIR(12));
	}

	mvwprintw(win_Begin,24,0,"Our ancestral cavern is highly threated by the MultiplexCorpThey are picking our gems, destroying our live and\nceremonial places with a mysterious machine...");
	mvwprintw(win_Begin,27,0,"They are destroying all the nature of our planet.\nNot even this ancestral cave is safe.");
	mvwprintw(win_Begin,30,0,"A rumor say that the MultiplexCorp's founder, who calls\nhimself \"The Architect\", is here, at the edge of the cavern.\nPlease avenge us, destroy the machine and kill the ArchitectOtherwise our cavern will be lost!");

	wattron(win_Begin,A_BOLD);
	mvwprintw(win_Begin,36,15,"**PRESS A KEY TO BEGIN**");
	wattroff(win_Begin,A_BOLD);

	wgetch(win_Begin);

	wborder(win_Begin, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wclear(win_Begin);
	wrefresh(win_Begin);
	delwin(win_Begin);
	//*******************************************

	WINDOW *win_userSpace;
	win_userSpace = newwin(xScreen/3,yScreen/7,2,yScreen/6+2);

	WINDOW *win_map;
	if(xScreen >= LONGUEUR+2 && yScreen >= LARGEUR+2)
		win_map = newwin(LONGUEUR+2,LARGEUR+2,4,yScreen/3);
	else
		win_map = newwin(xScreen-4,2*yScreen/3,4,yScreen/3);

	WINDOW *win_ennemySpace;
	win_ennemySpace = newwin(xScreen-5,yScreen/7+1,xScreen/3+2,yScreen/6+2);

	WINDOW *win_weapon;
	win_weapon = newwin(8,yScreen/7+1,xScreen/2+6,4);

	WINDOW *win_inventory;
	win_inventory = newwin(xScreen/2+3,yScreen/7+1,2,4);

	WINDOW *win_messages;
	if(xScreen >= LONGUEUR+7 && yScreen >= LARGEUR+2)
		win_messages = newwin(5,LARGEUR+2,LONGUEUR+7,yScreen/3+1);
	else
		win_messages = newwin(5,2*yScreen/3,xScreen-7,yScreen/3+1);

	WINDOW *win_armor;
	win_armor = newwin(8,30,42,4);

	char messagePrint[300];
	int colorPrint = 0;
	snprintf(messagePrint,100,"Press <ESC> for help at any time");

	keypad(win_map, TRUE);
    scrollok(win_map, TRUE);
	cbreak();
	resizeterm(55,204);

	int nbrEnnemiesMax = 18;
	Ennemy ennemies[ENEMIES_MAX];
	Map map[LONGUEUR][LARGEUR];

	generMap(map,win_map);

	player.x = 1;
	player.y = LARGEUR/2;

	int c = 0;
	int gameStatus = 1; // new turn = 1, no turn = 0
	int turn = 0;
	int compBefore = 0;
	int turnBurned = 0;
	int turnParalysed = 3;
	int stone = 0;
	int nbrEnnemyInFloor = 0;
	int nbrPnjOnFloor = 0;
	int questDrug = 0;
	int drugUsed = 0;
	int turnWithoutDrug = 0;
	int turnArmor = 0;

	setEnnemies(map,ennemies, nbrEnnemiesMax, &nbrEnnemyInFloor, floor);
	initObjects(listWeapons,listArmors);
	setObjects(map, listWeapons, weaponsOnFloor, nbrWeaponMax, &nbrWeapons, listArmors, armorOnFloor, nbrArmorMax, &nbrArmors);

	armorHold = listArmors[0];
	weaponHold = listWeapons[0];

	int nbrDrugsMax = DRUGS_MAX;
	int nbrDrugsOnFloor = 0;
	
	Drug drugList[NBR_DRUGS];
	Drug drugs[DRUGS_MAX];

	for(i = 0; i < NBR_DRUGS; i++)
	{
		drugList[i].indice = i;
		drugList[i].poisoned = 0;
		drugList[i].discovered = 0;
		drugList[i].type = i;
		drugs[i].x = 0;
		drugs[i].y = 0;
	}

	shuffleDrugs(map, drugs, nbrDrugsMax, &nbrDrugsOnFloor);

	int placeDrugTaken = -1;
	int saveWeap = weaponHold.degat;
	int turnPower = 0;
	int indicePnj = -1;
	int nbrQuestAct = 0;
	int boostFinished = 0;

	setPnj(map, pnjOnFloor, &nbrPnjOnFloor);

	Quest actQuest[NBR_QUESTS_MAX];
	Quest listQuest[NBR_QUESTS];
	initQuest(listQuest);
	clearQuest(actQuest);

	while(1)
	{
		source = 10;

		iaCore(ennemies,&player,map,&nbrEnnemyInFloor,messagePrint,&colorPrint,&armorHold,win_messages,&source);
		
		indicePnj = -1;
		int indiceQuest = 0;	
		int retHit = 0;
		int bufferLife = player.life;

		if(gameStatus == 1 || gameStatus == 2)
		{
			if(armorHold.indice == 1) // medical combinaison
			{
				if(rand()%35 == 1)
					player.life++;
			}

			ennemyPresent = 0;
			player.colorBlind = 0;
			turn++;

			diseaseLevel += 0.02;
			/* diseases functions with stades: stade 1, stade 2, stade 3 and stade 4
			STADE 1: 0-25	-> FRIST SYMPTOMS: THE PLAYER HAS 1 CHANCE ON 10 TO LOOSE 1 LP ON EACH TURN
			STADE 2: 25-50  -> INCONFORTABLE STADE: THE PLAYER HAS 1 CHANCE ON 4 TO LOOSE 1 LP ON EACH TURN + APPEARANCE OF SYMPTOMS
			STADE 3: 50-75  -> ADVANCED STADE:  THE PLAYER HAS 1 CHANCE ON 2 TO LOOSE 3 LP ON EACH TURN + SYMPTOMS MORE FRENQUENTS
			STADE 4: 75-100 -> DEATH IMMINENT ! THE PLAYER LOOSES 5LP PER TURN 
			*/
			if(diseaseLevel < 0)
				diseaseLevel = 0;
		
			if(diseaseLevel > 25 && diseaseLevel < 50)
			{
				if(rand()%50 == 0)
				{
					source = 5;
					player.life -= 2;
					if(diseaseIndice == 0)
					{
						player.life -= 3;
					}
					if(diseaseIndice == 1)
					{
						player.colorBlind = 1;
					}
					if(diseaseIndice == 2)
					{
						if(rand()%5 == 0)
						{
							int statModified = rand()%3;
							if(statModified == 0)
								player.agility--;
							else if(statModified == 1)
								player.strength--;
							else
								player.hate--;
						}
					}
				}
			}
			if(diseaseLevel > 50 && diseaseLevel < 75)
			{
				if(rand()%40 == 0)
				{
					player.life -= 3;
					if(diseaseIndice == 0)
					{
						source = 5;
						player.life -= 4;
					}
					if(diseaseIndice == 1)
					{
						player.colorBlind = 1;
						if(rand()%20 == 0)
							player.view--;
					}
					if(diseaseIndice == 2)
					{
						if(rand()%2 == 0)
						{
							int statModified = rand()%3;
							if(statModified == 0)
								player.agility--;
							else if(statModified == 1)
								player.strength--;
							else
								player.hate--;
						}
						else
						{
							if(player.state == 0)
								player.state = 2;
							else
								player.state2 = 2;
						}
					}
				}
			}
			if(diseaseLevel > 75 && diseaseLevel < 100 && rand()%30 == 0)
			{
				source = 5;
				player.life -= 5;
				if(diseaseIndice == 0)
				{
					player.life -= 3;
				}
				if(diseaseIndice == 1)
				{
					player.colorBlind = 1;
					if(rand()%40 == 0)
						wclear(win_map);
					if(rand()%10 == 0)
						player.view--;
				}
				if(diseaseIndice == 2)
				{
					if(rand()%2 == 0)
					{
						int statModified = rand()%3;
						if(statModified == 0)
							player.agility--;
						else if(statModified == 1)
							player.strength--;
						else
							player.hate--;
					}
					else
					{
						if(player.state == 0)
							player.state = 2;
						else
							player.state2 = 2;
					}
				}
			}
			if(diseaseLevel >= 100)
			{
				source = 5;
				player.life = 0;
			}

			if(player.state == 3 || player.state2 == 3)
				stone = 1;
			else
				stone = 0;

			//if(rand()%10 == 0)
				//destroyMap(map,player);

			recalcScreen(win_map,map,nbrEnnemyInFloor,ennemies);
			calcFOV(map,player.view,player.x,player.y,win_map,ennemies,nbrEnnemyInFloor,&ennemyPresent,player.colorBlind,stone,drugs,nbrDrugsOnFloor,weaponsOnFloor,nbrWeapons,armorOnFloor,nbrArmors,pnjOnFloor,nbrPnjOnFloor);
			printScreenUser(win_userSpace,player,powerUsed,diseaseName[diseaseIndice],(int)diseaseLevel, nbrEnnemiesKilled);
			
			/*if(ennemyPresent == 1)
			{*/
			printEnnemy(win_ennemySpace,ennemies,nbrEnnemyInFloor);
			//}

			setMessage(win_messages,messagePrint,colorPrint);
			printWeapon(win_weapon,weaponHold);
			printInventory(win_inventory, component, exoticBlood, gem, player, inventory, drugs, listWeapons, listArmors);
			printArmor(win_armor, armorHold);
			mvwprintw(stdscr,6+LONGUEUR,70,", : QUESTS    [ENTER] : SPECIAL POWER   c : CRAFTING MENU   [ESC] : MENU");
			mvwprintw(stdscr,3,70+LARGEUR/2-4,"*** FLOOR: -%d ***",floor);

			if(gameStatus == 2)
			{	
				score += 50*floor;
				
				if(questDrug)
				{
					turnWithoutDrug++;
				}
				else
				{
					turnWithoutDrug = 0;
					drugUsed = 0;
				}
				
				int erase = 0;
				for(erase = 0; erase < nbrEnnemyInFloor; erase++)
				{
					ennemies[erase].life = 0;
					map[ennemies[erase].x][ennemies[erase].y].typeEnnemy = 0;
				}
				wclear(win_messages);
				colorPrint = 47; 
				snprintf(messagePrint,100,"YOU DISCOVERED A NEW FLOOR");
				wclear(win_userSpace);
				wclear(win_messages);
				wclear(win_inventory);

				powerUsed = 0;

				if(floor % 5 == 0)
				{
					generMap(map,win_map);
					int xErase = LONGUEUR/2-5;
					int yErase = LARGEUR/2-5;
					for(xErase = LONGUEUR/2-5; xErase < LONGUEUR/2+5; xErase++)
					{
						for(yErase = LARGEUR/2-5; yErase < LARGEUR/2+5; yErase++)
						{
							map[xErase][yErase].content = FLOOR;
						}
					}
					switch(floor)
					{
						case 5:
							nbrEnnemyInFloor = 1;
							ennemies[0].life = 200;
							ennemies[0].x = LONGUEUR/2;
							ennemies[0].y = LARGEUR/2;
							ennemies[0].seen = 0;
							ennemies[0].strength = 12;
							ennemies[0].agility = 15;
							ennemies[0].haveBlaster = 1;
							ennemies[0].type = 2;
							ennemies[0].effect = 0;
							ennemies[0].name = "GH-28 THE SUPER SENTRY";
							ennemies[0].boss = 2;
							break;
						case 10:
							nbrEnnemyInFloor = 1;
							ennemies[0].life = 250;
							ennemies[0].x = LONGUEUR/2;
							ennemies[0].y = LARGEUR/2;
							ennemies[0].seen = 0;
							ennemies[0].strength = 12;
							ennemies[0].agility = 10;
							ennemies[0].haveBlaster = 0;
							ennemies[0].type = 3;
							ennemies[0].effect = 0;
							ennemies[0].name = "Althorus";
							ennemies[0].boss = 1;
							break;
						case 15:
							player.x = LONGUEUR/2+5;
							player.y = LARGEUR/2;
							nbrEnnemyInFloor = 1;
							ennemies[0].life = 300;
							ennemies[0].x = LONGUEUR/2;
							ennemies[0].y = LARGEUR/2;
							ennemies[0].seen = 0;
							ennemies[0].strength = 14;
							ennemies[0].agility = 14;
							ennemies[0].haveBlaster = 1;
							ennemies[0].type = 1;
							ennemies[0].effect = 0;
							ennemies[0].name = "The Architect";
							ennemies[0].boss = 3;

							snprintf(messagePrint,100,"This is just a game, you can't win...");
							colorPrint = 166;

							xErase = 0;
							yErase = 0;
							for(xErase = 0; xErase < LONGUEUR; xErase++)
							{
								for(yErase = 0; yErase < LARGEUR; yErase++)
								{
									map[xErase][yErase].content = FLOOR;
									map[xErase][yErase].effect = 0;
								}
							}
							break;
					}
				}
				else
				{
					nbrEnnemiesMax+=2;
					generMap(map,win_map);
					setEnnemies(map,ennemies, nbrEnnemiesMax, &nbrEnnemyInFloor, floor);
					setPnj(map,pnjOnFloor,&nbrPnjOnFloor);
					nbrDrugsOnFloor = rand()%nbrDrugsMax+4;

					for(i = 0; i < rand()%nbrDrugsOnFloor; i++)
					{
						drugs[i] = drugList[rand()%NBR_DRUGS];
						int posX = rand()%LONGUEUR;
						int posY = rand()%LARGEUR;
						map[posX][posY].content = DRUG;
					}

					setObjects(map, listWeapons, weaponsOnFloor, nbrWeaponMax, &nbrWeapons, listArmors ,armorOnFloor, nbrArmorMax, &nbrArmors);

					for(indiceQuest = 0; indiceQuest < NBR_QUESTS_MAX; indiceQuest++)
					{
						if(actQuest[indiceQuest].indice == 1)
						{
							turnArmor++;
							break;
						}
						if(actQuest[indiceQuest].indice == 3)
						{
							map[rand()%LONGUEUR][rand()%LARGEUR].content = COMPUTER;
							break;
						}
					}
				}
			}
			if(map[player.x][player.y].content == WALL)
			{
				map[player.x][player.y].content = FLOOR;
				map[player.x][player.y+1].content = FLOOR;
				map[player.x][player.y-1].content = FLOOR;
				map[player.x+1][player.y+1].content = FLOOR;
				map[player.x+1][player.y-1].content = FLOOR;
				map[player.x+1][player.y].content = FLOOR;
				map[player.x-1][player.y].content = FLOOR;
				map[player.x-1][player.y+1].content = FLOOR;
				map[player.x-1][player.y-1].content = FLOOR;
				map[player.x][player.y].effect = 0;
			}

			placeDrugTaken = -1;
			gameStatus = 0;
		}

		if(player.state == 1 || player.state2 == 1)
		{
			wattron(win_map,COLOR_PAIR(BURN_EFFECT));
			mvwprintw(win_map,player.x+1,player.y+1,"@");
			wattroff(win_map,COLOR_PAIR(BURN_EFFECT));			
		}
		else
		{
			wattron(win_map,COLOR_PAIR(COLOR_BH+1));
			mvwprintw(win_map,player.x+1,player.y+1,"@");
			wattroff(win_map,COLOR_PAIR(COLOR_BH+1));
		}

		refresh();	
		wrefresh(win_map);

		c = wgetch(win_map);
		
		if(player.state != 2 && player.state2 != 2 && (turnParalysed > 2 || turnParalysed < 0))
		{
			turnParalysed = 3;
			if(c >= '1' && c <= '9')
			{				
				int caseInventory = c-'0'-1;
				if(inventory[caseInventory].type == 2)
				{
					placeDrugTaken = caseInventory;
				}
				else if(inventory[caseInventory].type == 0 || inventory[caseInventory].type == 1)
				{
					objectMenu(&weaponHold, listWeapons, &armorHold, listArmors, inventory, &player, &component, caseInventory, &colorPrint, messagePrint, inventory[caseInventory].type+1);
				}
				else if(inventory[caseInventory].type == 3)
				{
					if(inventory[caseInventory].object == 1) // set a nuclear charge
					{
						inventory[caseInventory].type = 0;
						inventory[caseInventory].object = -1;
						map[player.x][player.y].content = BOMB;
						map[player.x][player.y].effect = 2;
						map[player.x][player.y].resistance = 7; // 7 turns before the explosion

					}
					else if(inventory[caseInventory].object == 2) // set a shield
					{
						inventory[caseInventory].type = 0;
						inventory[caseInventory].object = -1;
						map[player.x][player.y].content = DOOR;
					}
					else if(inventory[caseInventory].object == 3) // set an electric trap
					{
						inventory[caseInventory].type = 0;
						inventory[caseInventory].object = -1;
						map[player.x+1][player.y].content = MINES;
						map[player.x+1][player.y].trap = 1;
					}
					else if(inventory[caseInventory].object == 4) // set a paralysing trap
					{
						inventory[caseInventory].type = 0;
						inventory[caseInventory].object = -1;
						map[player.x+1][player.y].content = MINES;
						map[player.x+1][player.y].trap = 2;
					}
					else if(inventory[caseInventory].object == 5) // set lava
					{
						inventory[caseInventory].type = 0;
						inventory[caseInventory].object = -1;
						map[player.x+1][player.y].effect = 3;
					}
				}
				wclear(win_inventory);
				wclear(win_weapon);
				wclear(win_userSpace);
				wclear(win_messages);
			}
			else
			{		
				switch(c)
				{
					case KEY_UP:
						if(map[player.x-1][player.y].content != WALL)
							player.x--;
						gameStatus = 1;
						break;
					case KEY_DOWN:
						if(map[player.x+1][player.y].content != WALL)
							player.x++;
						gameStatus = 1;
						break;
					case KEY_LEFT:
						if(map[player.x][player.y-1].content != WALL)					
							player.y--;
						gameStatus = 1;
						break;
					case KEY_RIGHT:
						if(map[player.x][player.y+1].content != WALL)
							player.y++;
						gameStatus = 1;
						break;
					case '\n':
						{
							if(powerUsed == 0)
							{
								powerUsed = 1;
								switch(player.power)
								{
									case 0: // absorbtion
										wclear(win_messages);
										wclear(win_userSpace);
										colorPrint = 47;
										snprintf(messagePrint,150,"BY USING YOUR POWER OF ABSORBTION, YOU CONVERT %d DISEASE POINTS ON LIFE POINTS. HOWEVER YOU CAN'T USE THIS POWER UNTIL THE NEXT FLOOR",(int)(diseaseLevel*5*player.hate/100));
										player.life += (int)((int)diseaseLevel*(5*player.hate)/100);
										diseaseLevel -= (int)((int)diseaseLevel*(5*player.hate)/100);
										if(diseaseLevel < 0)
											diseaseLevel = 0;
										break;
									case 1: // power
										wclear(win_messages);
										wclear(win_userSpace);
										colorPrint = 47;
										snprintf(messagePrint,150,"YOU USE YOUR POWER OF STRENGTH !");
										player.state = 4;
										saveWeap = weaponHold.degat;
										break;
								}
							}
						}
						break;
					case 27:
						if(pauseMenu() == 1)
						{
							quit();
							return 0;
						}

						break;
					case ' ':
						gameStatus = 1;
						break;	
					case 'z':
						wclear(win_weapon);
						wclear(win_ennemySpace);
						wclear(win_userSpace);
						hit('z', -1, 0, win_map, &player, &weaponHold, listWeapons, map, &gem, ennemies, nbrEnnemyInFloor, &component, &exoticBlood, &compBefore, &indiceEnnemy, &retHit, pnjOnFloor, nbrPnjOnFloor, &indicePnj);
						gameStatus = 1;
						break;

					case 's':
						wclear(win_weapon);
						wclear(win_ennemySpace);
						wclear(win_userSpace);
						hit('s', 1, 0, win_map, &player, &weaponHold, listWeapons, map, &gem, ennemies, nbrEnnemyInFloor, &component, &exoticBlood, &compBefore, &indiceEnnemy, &retHit, pnjOnFloor, nbrPnjOnFloor, &indicePnj);
						gameStatus = 1;
						break;

					case 'q':
						wclear(win_weapon);
						wclear(win_ennemySpace);
						wclear(win_userSpace);
						hit('q', 0, -1, win_map, &player, &weaponHold, listWeapons, map, &gem, ennemies, nbrEnnemyInFloor, &component, &exoticBlood, &compBefore, &indiceEnnemy, &retHit, pnjOnFloor, nbrPnjOnFloor, &indicePnj);
						gameStatus = 1;
						break;

					case 'd':
						wclear(win_weapon);
						wclear(win_ennemySpace);
						wclear(win_userSpace);
						hit('d', 0, 1, win_map, &player, &weaponHold, listWeapons, map, &gem, ennemies, nbrEnnemyInFloor, &component, &exoticBlood, &compBefore, &indiceEnnemy, &retHit, pnjOnFloor, nbrPnjOnFloor, &indicePnj);
						gameStatus = 1;
						break;

					case 'a':
						wclear(win_weapon);
						wclear(win_ennemySpace);
						wclear(win_userSpace);
						hit('a', -1, -1, win_map, &player, &weaponHold, listWeapons, map, &gem, ennemies, nbrEnnemyInFloor, &component, &exoticBlood, &compBefore, &indiceEnnemy, &retHit, pnjOnFloor, nbrPnjOnFloor, &indicePnj);
						gameStatus = 1;
						break;

					case 'e':
						wclear(win_weapon);
						wclear(win_ennemySpace);
						wclear(win_userSpace);
						hit('e', -1, 1, win_map, &player, &weaponHold, listWeapons, map, &gem, ennemies, nbrEnnemyInFloor, &component, &exoticBlood, &compBefore, &indiceEnnemy, &retHit, pnjOnFloor, nbrPnjOnFloor, &indicePnj);
						gameStatus = 1;
						break;

					case 'x':
						wclear(win_weapon);
						wclear(win_ennemySpace);
						wclear(win_userSpace);
						hit('x', 1, 1, win_map, &player, &weaponHold, listWeapons, map, &gem, ennemies, nbrEnnemyInFloor, &component, &exoticBlood, &compBefore, &indiceEnnemy, &retHit, pnjOnFloor, nbrPnjOnFloor, &indicePnj);
						gameStatus = 1;
						break;

					case 'w':
						wclear(win_weapon);
						wclear(win_ennemySpace);
						wclear(win_userSpace);
						hit('w', 1, -1, win_map, &player, &weaponHold, listWeapons, map, &gem, ennemies, nbrEnnemyInFloor, &component, &exoticBlood, &compBefore, &indiceEnnemy, &retHit, pnjOnFloor, nbrPnjOnFloor, &indicePnj);

						gameStatus = 1;
						break;

					case 'c':
						craftMenu(inventory,drugs,player,&component,&exoticBlood,&gem);
						wclear(win_userSpace);
						wclear(win_messages);	
						wclear(win_inventory);				
						break;

					case ',':
						questsMenu(actQuest, nbrQuestAct, listWeapons, listArmors, killGuardian, killTurret, killDroid);
						wclear(win_userSpace);
						wclear(win_messages);	
						wclear(win_inventory);
						break;

					default:
						placeDrugTaken = -1;
						break;
				}
			}

			if(placeDrugTaken >= 0)
			{
				takeDrug(map,inventory,drugs,placeDrugTaken,messagePrint,&colorPrint,&player,&diseaseLevel);
				drugUsed = 1;
				gameStatus = 1;
				wclear(win_userSpace);
				wclear(win_messages);	
			}
		}
		else
		{
			turnParalysed--;
			if(player.state == 2)
				player.state = 0;
			else
				player.state2 = 0;

			colorPrint = 197; 
			snprintf(messagePrint,100,"YOU ARE PARALYSED !");
			wclear(win_userSpace);
			wclear(win_messages);	
			wclear(win_armor);		
		}

		if(player.state == 4 || player.state2 == 4)
		{
			if(boostFinished == 0)
			{
				player.strength += (3*player.hate);
				weaponHold.degat = player.strength;
			}
			
			turnPower++;
			if(turnPower == 2)
			{
				turnPower = 0;
				if(player.state == 4)
					player.state = 0;
				else if(player.state2 == 4)
					player.state2 = 0;
			}
			boostFinished = 1;
		}
		else
		{
			if(boostFinished != 0)
			{
				boostFinished = 0;
				player.strength -= (3*player.hate);
				weaponHold.degat = saveWeap;
				turnPower = 0;
			}
		}

		if(indicePnj >= 0)
		{
			sayQuest(pnjOnFloor[indicePnj], listQuest, actQuest, &nbrQuestAct, listWeapons, listArmors, &diseaseLevel, &questDrug, &drugUsed);
		}

		for(indiceQuest = 0; indiceQuest < NBR_QUESTS_MAX; indiceQuest++)
		{
			if(actQuest[indiceQuest].isSet)
			{
				if(actQuest[indiceQuest].indice == 2 && weaponHold.type == 1)
				{
					actQuest[indiceQuest].isSet = 0;
					player.life =  0;
					colorPrint = 197; 
					snprintf(messagePrint,100,"YOU UNLEACHED THE DIVINE WRATH !");
					wclear(win_userSpace);
					wclear(win_messages);
					source = 4;
					break;
				}
				else if(actQuest[indiceQuest].indice == 0)
				{
					if(drugUsed)
					{
						actQuest[indiceQuest].isSet = 0;
						colorPrint = 197;
						snprintf(messagePrint,100,"YOU USED A DRUG, THE QUEST \"EXPIATION\" IS CANCELLED !");
						wclear(win_userSpace);
						wclear(win_messages);		
					}
					else
					{
						if(turnWithoutDrug == 3)
						{
							diseaseLevel -= 20;
							nbrQuestsDone++;
							actQuest[indiceQuest].isSet = 0;
							colorPrint = 47;
							snprintf(messagePrint,100,"YOU FINISHED THE QUEST \"EXPIATION\", YOUR DISEASE IS HEALED BY 20 PTS");
							score += 40;
							wclear(win_userSpace);
							wclear(win_messages);	
						}
					}
				}
				else if(actQuest[indiceQuest].indice == 1)
				{
					if(armorHold.indice != 9)
					{
						actQuest[indiceQuest].isSet = 0;
						colorPrint = 197;
						snprintf(messagePrint,100,"YOU WEAR AN ARMOR, THE QUEST \"EXHIBITIONIST\" IS CANCELLED !");
						wclear(win_userSpace);
						wclear(win_messages);		
					}
					else
					{
						if(turnArmor)
						{
							diseaseLevel -= 15;
							nbrQuestsDone++;
							actQuest[indiceQuest].isSet = 0;
							colorPrint = 47;
							snprintf(messagePrint,100,"YOU FINISHED THE QUEST \"EXHIBITIONIST\", YOUR DISEASE IS HEALED BY 15 PTS");
							score += 35;
							wclear(win_userSpace);
							wclear(win_messages);	
						}
					}
				}
				else if(actQuest[indiceQuest].indice == 7)
				{
					if(weaponHold.indice == 0)
					{
						weaponHold = listWeapons[3];
						weaponHold.degat = player.strength/3;
						colorPrint = 47; 
						nbrQuestsDone++;
						actQuest[indiceQuest].isSet = 0;
						snprintf(messagePrint,100,"YOU GIVE YOUR BONE SPEAR, YOU FINISHED THE QUEST \"THE LOST BONE\", YOU EARNED A DRUG");
						score += 25;
						wclear(win_userSpace);
						wclear(win_messages);
						wclear(win_weapon);
						for(s = 0; s < player.inventorySize; s++)
						{
							if(inventory[s].type == -1)
							{
								inventory[s].type = 2;
								inventory[s].object = rand()%NBR_DRUGS;
								break;
							}
						}
					}
					else
					{
						for(s = 0; s < player.inventorySize; s++)
						{
							if(inventory[s].type == 0 && inventory[s].object == 0)
							{
								inventory[s].type = 2;
								inventory[s].object = rand()%NBR_DRUGS;
								colorPrint = 47; 
								nbrQuestsDone++;
								actQuest[indiceQuest].isSet = 0;
								snprintf(messagePrint,100,"YOU GIVE YOUR BONE SPEAR, YOU FINISHED THE QUEST \"THE LOST BONE\", YOU EARNED A DRUG");
								score += 25;
								wclear(win_userSpace);
								wclear(win_messages);
								wclear(win_weapon);
								break;
							}
						}
					}
				}
				else if(actQuest[indiceQuest].indice == 3)
				{
					if(map[player.x][player.y].content == COMPUTER)
					{
						map[player.x][player.y].content = FLOOR;
						actQuest[indiceQuest].isSet = 0;
						if(passwordComputer())
						{
							nbrQuestsDone++;
							colorPrint = 47;
							snprintf(messagePrint,100,"YOU RESTORED THE DATA, YOU FINISHED THE QUEST \"THE LOST COMPUTER\", YOU EARNED 1000 COMPONENTS");
							score += 100;
							wclear(win_userSpace);
							wclear(win_messages);
						}
						else
						{
							colorPrint = 197;
							snprintf(messagePrint,100,"WRONG PASSWORD ! YOU FAILED THE QUEST \"THE LOST COMPUTER\"...");
							wclear(win_userSpace);
							wclear(win_messages);				
						}
					}
				}
				else if(actQuest[indiceQuest].indice == 4)
				{
					if(retHit == 1 && ennemies[indiceEnnemy].type == 2)
					{
						retHit = 0;
						killTurret++;
					}
					if(killTurret >= 5)
					{
						nbrQuestsDone++;
						for(s = 0; s < player.inventorySize; s++)
						{
							if(inventory[s].type == -1)
							{
								inventory[s].type = 1;
								inventory[s].object = listArmors[actQuest[indiceQuest].reward].indice;
								break;
							}
						}
						actQuest[indiceQuest].isSet = 0;
						nbrQuestsDone++;
						killTurret = 0;
						colorPrint = 47; 
						snprintf(messagePrint,120,"AFTER KILLING 5 TURRETS, YOU FINISHED THE QUEST \"KILL THE TURRETS, KILL THE TURRETS !\", YOU EARNED A %s",listArmors[actQuest[indiceQuest].reward].name);
						score += 155;
						wclear(win_userSpace);
						wclear(win_messages);
						wclear(win_weapon);
					}
				}
				else if(actQuest[indiceQuest].indice == 6)
				{
					if(retHit == 1 && ennemies[indiceEnnemy].type == 4)
					{
						retHit = 0;
						killGuardian++;
					}
					if(killGuardian >= 5)
					{
						actQuest[indiceQuest].isSet = 0;
						nbrQuestsDone++;
						for(s = 0; s < player.inventorySize; s++)
						{
							if(inventory[s].type == -1)
							{
								inventory[s].type = 2;
								inventory[s].object = rand()%NBR_DRUGS;
								break;
							}
						}	
						killGuardian = 0;
						colorPrint = 47; 
						snprintf(messagePrint,120,"AFTER KILLING 5 INFECTED GUARDIANS, YOU FINISHED THE QUEST \"THERE ARE MONSTERS HERE...\", YOU EARNED A DRUG");
						score += 85;
						wclear(win_userSpace);
						wclear(win_messages);
						wclear(win_weapon);
					}
				}
				else if(actQuest[indiceQuest].indice == 8)
				{
					if(retHit == 1 && ennemies[indiceEnnemy].type == 1)
					{
						retHit = 0;
						killDroid++;
					}
					if(killDroid >= 10)
					{
						actQuest[indiceQuest].isSet = 0;
						nbrQuestsDone++;
						for(s = 0; s < player.inventorySize; s++)
						{
							if(inventory[s].type == -1)
							{
								inventory[s].type = 0;
								inventory[s].object = listWeapons[actQuest[indiceQuest].reward].indice;
								break;
							}
						}	
						killDroid = 0;
						colorPrint = 47; 
						snprintf(messagePrint,120,"AFTER KILLING 10 DROIDS, YOU FINISHED THE QUEST \"BORN OF A BORKEN MAN\", YOU EARNED A %s",listWeapons[actQuest[indiceQuest].reward].name);
						score += 115;
						wclear(win_userSpace);
						wclear(win_messages);
						wclear(win_weapon);
					}
				}
			}
		}	

		if(retHit == 1)
		{
			retHit = 0;

			wclear(win_ennemySpace);
			colorPrint = 47; 
			if(ennemies[indiceEnnemy].type == 1 || ennemies[indiceEnnemy].type == 2)
			{
				snprintf(messagePrint,100,"YOU KILLED A %s ! YOU EARN %d COMPONENTS",classEnemy[ennemies[indiceEnnemy].type],abs(component-compBefore));
				score += 50;
			}
			else
			{
				snprintf(messagePrint,100,"YOU KILLED A %s ! YOU EARN EXOTIC BLOOD",classEnemy[ennemies[indiceEnnemy].type]);
				score += 35;
			}
			compBefore = component;

			wclear(win_userSpace);
			wclear(win_messages);
			wclear(win_armor);

			if(armorHold.indice == 8) // if we are equiped to symbiot
			{
				player.life += (10+rand()%5);
			}

			nbrEnnemiesKilled++;
			if(nbrEnnemiesKilled % 10 == 0)
			{
				player.life += 5;
				player.hate++;
				colorPrint = 47; 
				snprintf(messagePrint,100,"AFTER KILLING %d ENEMIES, YOUR HATE LEVEL INCREASES",nbrEnnemiesKilled);
				score += 100;
				wclear(win_userSpace);
				wclear(win_messages);
			}

			if(ennemies[indiceEnnemy].boss == 1 || ennemies[indiceEnnemy].boss == 2 || ennemies[indiceEnnemy].boss == 3)
			{
				player.hate++;
				colorPrint = 47; 
				int gemsWin = rand()%5+5;
				gem+=gemsWin;
				snprintf(messagePrint,100,"YOU KILLED %s, YOUR HATE LEVEL INCREASES AND YOU GOT %d GEMS",bossNames[ennemies[indiceEnnemy].boss-1], gemsWin);
				score += 250;
				wclear(win_userSpace);
				wclear(win_messages);	
			}

			if(player.state == 3 || player.state2 == 3)
			{
				int indiceQuest = 0;
				for(indiceQuest = 0; indiceQuest < NBR_QUESTS_MAX; indiceQuest++)
				{
					if(actQuest[indiceQuest].indice == 9)
					{
						actQuest[indiceQuest].isSet = 0;
						exoticBlood += 20;
						colorPrint = 47; 
						nbrQuestsDone++;
						snprintf(messagePrint,100,"YOU FINISHED THE QUEST \"ANOTHER UNIVERSE\", YOU EARNED 20 EXOTIC BLOOD");
						wclear(win_userSpace);
						wclear(win_messages);
						break;
					}
				}
			}
		}
		else if(retHit == 2)
		{
			retHit = 0;
			colorPrint = 47; 
			int bodyHit = rand()%5;
			snprintf(messagePrint,100,"CRITICAL HIT ! YOU HIT HIS %s",body[bodyHit]);
			score+=20;
			if(bodyHit == 3 || bodyHit == 4)
			{
				int roll = rand()%2;
				if(roll)
				{
					if(ennemies[indiceEnnemy].agility > 0)
						ennemies[indiceEnnemy].agility--;
				}
				else
				{
					if(ennemies[indiceEnnemy].strength > 0)
						ennemies[indiceEnnemy].strength--;
				}
			}
			wclear(win_userSpace);
			wclear(win_messages);			
		}

		if(bufferLife != player.life)
		{
			wclear(win_userSpace);
			wclear(win_weapon);
			wclear(win_armor);
		}

		if(player.state2 == 1 || player.state == 1)
		{
			if(rand()%10 == 0)
			{
				if(player.state == 1)
					player.state = 0;
				else
					player.state2 = 0;
			}
			player.life -= 3;
			source = 6;
			turnBurned++;
			colorPrint = 197;
			snprintf(messagePrint,100,"YOU ARE BURNING !");
			wclear(win_userSpace);
			wclear(win_messages);					
		}

		if(map[player.x][player.y].effect == 2) // if the player is in a radioactive-zone
		{
			if(armorHold.indice != 6) // if the player doesn't hold an anti-rad armor
			{
				if(rand()%4 == 0)
					player.life--;
				diseaseLevel++;
			}

			colorPrint = 197;
			snprintf(messagePrint,100,"YOU ENTERED IN A RADIOACTIVE ZONE");
			wclear(win_userSpace);
			wclear(win_messages);	
			source = 3;
		}
		else if((map[player.x][player.y].effect == 1 || map[player.x][player.y].effect == 3) &&  armorHold.indice != 5)// if the player is in a fire-zone (fire or lava)
		{
			if(player.state == 0)
				player.state = 1;
			else
				player.state2 = 1;

			colorPrint = 197;
			snprintf(messagePrint,100,"YOU CATCH FIRE !");
			wclear(win_userSpace);
			wclear(win_messages);	
		}

		if(map[player.x][player.y].content == STAIRS)
		{
			gameStatus = 2; // new floor
			floor++;
			diseaseLevel-=5;

		}
		else if(map[player.x][player.y].content == MINES)
		{
			switch(map[player.x][player.y].trap)
			{
				case 1:
					map[player.x][player.y].trap = 0;
					map[player.x][player.y].content = FLOOR;
					player.life -= 5;
					source = 1;
					colorPrint = 197; // RED LIKE BLOOOOOOOD
					snprintf(messagePrint,100,"YOU WERE TRAPPED BY AN ELECTRIC TRAP ! (-5 LP)");
					wclear(win_userSpace);
					wclear(win_messages);
					break;

				case 2:
					map[player.x][player.y].trap = 0;
					map[player.x][player.y].content = FLOOR;

					if(player.state == 0)
						player.state = 2;
					else
						player.state2 = 2;

					colorPrint = 197; 
					snprintf(messagePrint,100,"YOU WERE TRAPPED BY A PARALYSING TRAP !");
					wclear(win_userSpace);
					wclear(win_messages);
					break;

				case 3:
						map[player.x][player.y].trap = 0;
						map[player.x][player.y].content = FLOOR;

						player.life -= 2;
						source = 2;
						if(player.y > 0)
							map[player.x][player.y-1].effect = 1;
						if(player.y < LARGEUR-1)
							map[player.x][player.y+1].effect = 1;
						if(player.x > 0)
							map[player.x-1][player.y].effect = 1;		
						if(player.x < LONGUEUR-1)
							map[player.x+1][player.y].effect = 1;
						flashBomb(win_map);

						colorPrint = 197; 
						snprintf(messagePrint,100,"YOU WERE TRAPPED BY AN EXPLOSIVE TRAP !");
						wclear(win_userSpace);
						wclear(win_messages);
					 break;
				}
		}
		else if(map[player.x][player.y].content == DRUG)
		{
			int drug = testDrug(drugs,player.x,player.y,nbrDrugsOnFloor);

			if(drug >= 0)
			{
				for(s = 0; s < player.inventorySize; s++)
				{
					if(inventory[s].type == -1)
					{
						score += 15;
						inventory[s].type = 2;
						inventory[s].object = drugs[drug].indice;
						map[player.x][player.y].content = FLOOR;
						colorPrint = 47; 
						snprintf(messagePrint,100,"YOU FOUND A DRUG !");
						wclear(win_userSpace);
						wclear(win_messages);
						break;
					}
				}
			}
		}
		else if(map[player.x][player.y].content == DIST_WEAPON || map[player.x][player.y].content == CONTACT_WEAPON)
		{
			int w = testWeapon(weaponsOnFloor,player.x,player.y,nbrWeapons);

			if(w >= 0)
			{
				for(s = 0; s < player.inventorySize; s++)
				{
					if(inventory[s].type == -1)
					{
						score += 15;
						inventory[s].type = 0;
						inventory[s].object = weaponsOnFloor[w].indice;
						map[player.x][player.y].content = FLOOR;
						colorPrint = 47; 
						snprintf(messagePrint,100,"YOU FOUND A %s !",listWeapons[weaponsOnFloor[w].indice].name);
						wclear(win_userSpace);
						wclear(win_messages);
						break;
					}
				}
			}
		}
		else if(map[player.x][player.y].content == ARMOR)
		{
			int a = testArmor(armorOnFloor,player.x,player.y,nbrArmors);

			if(a >= 0)
			{
				for(s = 0; s < player.inventorySize; s++)
				{
					if(inventory[s].type == -1)
					{
						score += 10;
						inventory[s].type = 1;
						inventory[s].object = armorOnFloor[a].indice;
						map[player.x][player.y].content = FLOOR;
						colorPrint = 47; 
						snprintf(messagePrint,100,"YOU FOUND A %s !",listArmors[armorOnFloor[a].indice].name);
						wclear(win_userSpace);
						wclear(win_messages);
						break;
					}
				}		
			}
		}

		if(weaponHold.state <= 0 && weaponHold.indice != 3) // if the player's weapon is destoryed, he must use his fists
		{
			colorPrint = 197; 
			snprintf(messagePrint,100,"YOU BROKE YOUR WEAPON (%s)",weaponHold.name);
			wclear(win_userSpace);
			wclear(win_messages);	

			component += weaponHold.cost;

			weaponHold = listWeapons[3];
			weaponHold.degat = player.strength/3;
		}

		if(armorHold.state <= 0 && armorHold.indice != 5)
		{
			colorPrint = 197; 
			snprintf(messagePrint,100,"YOU BROKE YOUR ARMOR (%s)",armorHold.name);
			wclear(win_userSpace);
			wclear(win_messages);	
			wclear(win_armor);
			armorHold = listArmors[ARMORLESS];	
			wclear(win_armor);
		}

		if(armorHold.indice == 7) // crystal armor
			armorHold.resistance = gem*5;

		if(weaponHold.indice == 10) // crystal sword
		{
			weaponHold.degat = gem*4;
			weaponHold.state = gem*4;
		}

		if(armorHold.indice == 8 && rand()%51 == 2)
			armorHold.state++;

		if(player.life <= 0)
		{
			colorPrint = 197; 
			snprintf(messagePrint,100,"YOU DIED !");
			wclear(win_userSpace);
			wclear(win_messages);
			score += exoticBlood*2;
			score += component;
			score += gem*10;
			score += nbrQuestsDone*20;
			gameOver(source,score);
			break;
		}

		if(bufferLife != player.life)
		{
			if(player.state == 3)
			{
				player.state = 0;
				player.view -= 2;
			}
			else if(player.state2 == 3)
			{
				player.state2 = 0;
				player.view -= 2;
			}
			wclear(win_armor);
			wclear(win_userSpace);
		}

		if(component >= 50)
		{
			int indiceQuest = 0;
			for(indiceQuest = 0; indiceQuest < NBR_QUESTS_MAX; indiceQuest++)
			{
				if(actQuest[indiceQuest].indice == 5)
				{
					score += 50;
					component -= 50;
					actQuest[indiceQuest].isSet = 0;
					player.life += 20;
					colorPrint = 47; 
					nbrQuestsDone++;
					snprintf(messagePrint,100,"YOU FINISHED THE QUEST \"FIXING\", YOU EARNED 20 LP");
					wclear(win_userSpace);
					wclear(win_messages);
					break;
				}
			}	
		}

		if(gem >= 5)
		{
			int indiceQuest = 0;
			for(indiceQuest = 0; indiceQuest < NBR_QUESTS_MAX; indiceQuest++)
			{
				if(actQuest[indiceQuest].indice == 10)
				{
					score += 75;
					nbrQuestsDone++;
					gem -= 10;
					actQuest[indiceQuest].isSet = 0;
					
					for(s = 0; s < player.inventorySize; s++)
					{
						if(inventory[s].type == -1)
						{
							inventory[s].type = 0;
							inventory[s].object = listWeapons[actQuest[indiceQuest].reward].indice;
							break;
						}
					}
					colorPrint = 47; 
					snprintf(messagePrint,100,"YOU FINISHED THE QUEST \"GEMS ARE BLUE\", YOU EARNED %s",listWeapons[actQuest[indiceQuest].reward].name);
					wclear(win_userSpace);
					wclear(win_messages);
				}
				break;
			}
		}

		if(floor >= 5)
		{
			if(rand()%1000 == 42)
				popUpArchitect();
		}			
	}

	quit();

	return 0;
}

void quit()
{
	clrtoeol();
	refresh();
	getch();
	endwin();
}
