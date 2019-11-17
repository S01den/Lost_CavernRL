#include "item.h"

void shuffleDrugs(Map map[LONGUEUR][LARGEUR], Drug drugs[], int nbrDrugs, int *nbrDrugsInFloor)
{
	srand(time(NULL));

	int i = 0;

	*nbrDrugsInFloor = rand()%nbrDrugs+4;
	for(i = 0; i < *nbrDrugsInFloor; i++)
	{
		int indiceDrug = rand()%NBR_DRUGS;
		drugs[i].color = indiceDrug+COLOR_DRUGS;
		drugs[i].indice = i;
		drugs[i].type = indiceDrug;
		drugs[i].poisoned = rand()%4;
		drugs[i].discovered = 0;

		int posX = 0;
		int posY = 0;

		while(posX == 0 || posY == 0 || posX == LONGUEUR || posY == LARGEUR)
		{
			posX = rand()%LONGUEUR;
			posY = rand()%LARGEUR;
		}

		drugs[i].x = posX;
		drugs[i].y = posY;
		map[posX][posY].content = DRUG;
	}

}




int testDrug(Drug drugList[], int x, int y, int nbrDrugsInFloor)
{
	int ret = -1;
	int i = 0;

	for(i = 0; i < nbrDrugsInFloor; i++)
	{
		if(drugList[i].x == x && drugList[i].y == y)
		{
			ret = i;
			break;
		}
	}

	return ret;
}

int testWeapon(Weapons weapons[],int x, int y, int nbrWeapons)
{
	int ret = -1;
	int i = 0;

	for(i = 0; i < nbrWeapons; i++)
	{
		if(weapons[i].x == x && weapons[i].y == y)
		{
			ret = i;
			break;
		}
	}


	return ret;
}

int testArmor(Armor armor[],int x, int y, int nbrArmors)
{
	int ret = -1;
	int i = 0;

	for(i = 0; i < nbrArmors; i++)
	{
		if(armor[i].x == x && armor[i].y == y)
		{
			ret = i;
			break;
		}
	}


	return ret;
}

void setObjects(Map map[LONGUEUR][LARGEUR], Weapons listWeapons[], Weapons weponsOnFloor[], int nbrMaxWeapons, int *nbrWeapons, Armor armorList[], Armor armorOnFloor[], int nbrMaxArmor, int *nbrArmors)
{
	srand(time(NULL));

	int i = 0;
	int indice = 0;

	*nbrWeapons = rand()%nbrMaxWeapons+1;
	*nbrArmors = rand()%nbrMaxArmor+1;

	for(i = 0; i < *nbrWeapons; i++)
	{
		int rarity = rand()%100+1;
		if(rarity < 70)
		{
			indice = rand()%7;
			if(indice == 3)
				indice = 11;
			if(indice == 4)
				indice = 12;
			if(indice == 6)
				indice = 13;
		}
		else if(rarity >= 70 && rarity < 95)
		{
			indice = rand()%4+4;
			if(indice == 5)
				indice = 8;
			if(indice == 7)
				indice = 9;
		}
		else
		{
			indice = rand()%3+7;
			if(indice == 8)
				indice = 10;
			if(indice == 9)
				indice = 14;
		}

		indice = rand()%NBR_WEAPONS;
		weponsOnFloor[i].indice = indice;
		weponsOnFloor[i].name = listWeapons[indice].name;
		weponsOnFloor[i].rarity = listWeapons[indice].rarity;
		weponsOnFloor[i].degat = listWeapons[indice].degat;
		weponsOnFloor[i].efficiency = listWeapons[indice].efficiency;
		weponsOnFloor[i].strengthRequired = listWeapons[indice].strengthRequired;
		weponsOnFloor[i].state = listWeapons[indice].state;
		weponsOnFloor[i].range = listWeapons[indice].range;
		weponsOnFloor[i].type = listWeapons[indice].type;
		weponsOnFloor[i].cost = listWeapons[indice].cost;

		int posX = rand()%LONGUEUR;
		int posY = rand()%LARGEUR;
		if(posX == 0)
			posX++;
		if(posX == LONGUEUR-1)
			posX--;
		if(posY == 0)
			posY++;
		if(posY == LARGEUR || posY == LARGEUR-1)
			posY-=2;
		weponsOnFloor[i].x = posX;
		weponsOnFloor[i].y = posY;

		if(listWeapons[indice].type == 0)
			map[posX][posY].content = CONTACT_WEAPON;
		else
			map[posX][posY].content = DIST_WEAPON;
	}

	for(i = 0; i < *nbrArmors; i++)
	{
		int rarity = rand()%100+1;
		if(rarity < 70)
		{
			indice = rand()%4;
			if(indice == 1)
				indice = 4;
		}
		else if(rarity >= 70 && rarity < 95)
		{
			indice = rand()%3+4;
			if(indice == 4)
				indice = 1;
		}
		else
		{
			indice = rand()%2+7;
		}

		armorOnFloor[i].indice = indice;
		armorOnFloor[i].name = armorList[indice].name;
		armorOnFloor[i].rarity = armorList[indice].rarity;
		armorOnFloor[i].resistance = armorList[indice].resistance;
		armorOnFloor[i].strengthRequired = armorList[indice].strengthRequired;
		armorOnFloor[i].state = armorList[indice].state;

		int posX = rand()%LONGUEUR;
		int posY = rand()%LARGEUR;
		if(posX == 0)
			posX++;
		if(posX == LONGUEUR)
			posX--;
		if(posY == 0)
			posY++;
		if(posY == LARGEUR)
			posY--;

		armorOnFloor[i].x = posX;
		armorOnFloor[i].y = posY;
		map[posX][posY].content = ARMOR;
	}

}

void initObjects(Weapons listWeapons[], Armor listArmor[])
{
	int i = 0;

	for(i = 0; i < NBR_ARMORS; i++)
	{
		listArmor[i].state = 100;
		listArmor[i].indice = i;
		listArmor[i].x = 0;
		listArmor[i].y = 0;
	}
	for(i = 0; i < NBR_WEAPONS; i++)
	{
		listWeapons[i].indice = i;
		listWeapons[i].x = 0;
		listWeapons[i].y = 0;
	}

	listWeapons[0].name = "BONE SPEAR";
	listWeapons[0].degat = 6;
	listWeapons[0].efficiency = 85;
	listWeapons[0].strengthRequired = 11;
	listWeapons[0].state = 100;
	listWeapons[0].range = 1;
	listWeapons[0].type = 0;
	listWeapons[0].cost = 5;
	listWeapons[0].rarity = 1;
	listWeapons[0].description = "Your best friend's femur";

	listWeapons[1].name = "DARTS";
	listWeapons[1].degat = 8;
	listWeapons[1].efficiency = 65;
	listWeapons[1].strengthRequired = 5;
	listWeapons[1].state = 10;
	listWeapons[1].range = 4;
	listWeapons[1].type = 1;
	listWeapons[1].cost = 5;
	listWeapons[1].rarity = 1;
	listWeapons[1].description = "Nothing more to add";

	listWeapons[2].name = "BLASTER V1";
	listWeapons[2].degat = 9;
	listWeapons[2].efficiency = 80;
	listWeapons[2].strengthRequired = 5;
	listWeapons[2].state = 10;
	listWeapons[2].range = 6;
	listWeapons[2].type = 1;
	listWeapons[2].cost = 10;
	listWeapons[2].rarity = 1;
	listWeapons[2].description = "A blaster, nothing else";
	
	listWeapons[3].name = "FISTS";
	listWeapons[3].degat = 0;
	listWeapons[3].efficiency = 100;
	listWeapons[3].strengthRequired = 0;
	listWeapons[3].state = -1;
	listWeapons[3].range = 1;
	listWeapons[3].type = 0;
	listWeapons[3].cost = 0;
	listWeapons[3].rarity = 1;
	listWeapons[3].description = "Just your hands...";

	listWeapons[4].name = "BLASTER MK-X";
	listWeapons[4].degat = 15;
	listWeapons[4].efficiency = 90;
	listWeapons[4].strengthRequired = 5;
	listWeapons[4].state = 5;
	listWeapons[4].range = 12;
	listWeapons[4].type = 1;
	listWeapons[4].cost = 20;
	listWeapons[4].rarity = 2;
	listWeapons[4].description = "Only 99999.99$";

	listWeapons[5].name = "POCKET BLASTER";
	listWeapons[5].degat = 8;
	listWeapons[5].efficiency = 100;
	listWeapons[5].strengthRequired = 1;
	listWeapons[5].state = 15;
	listWeapons[5].range = 9;
	listWeapons[5].type = 1;
	listWeapons[5].cost = 5;
	listWeapons[5].rarity = 1;
	listWeapons[5].description = "This toy is offered by MultiplexCorp-ToysTM, all rights reserved.";

	listWeapons[6].name = "FLAME THROWER XZ-3";
	listWeapons[6].degat = 3;
	listWeapons[6].efficiency = 100;
	listWeapons[6].strengthRequired = 10;
	listWeapons[6].state = 6;
	listWeapons[6].range = 6;
	listWeapons[6].type = 2; // type 2 = flame
	listWeapons[6].cost = 20;
	listWeapons[6].rarity = 2;
	listWeapons[6].description = "A smart flame thrower running on bbqOS 12.3.5, jailbreaked by geoHOT";

	listWeapons[7].name = "CRYSTAL CANON";
	listWeapons[7].degat = 0;
	listWeapons[7].efficiency = 100;
	listWeapons[7].strengthRequired = 11;
	listWeapons[7].state = 100;
	listWeapons[7].range = 4;
	listWeapons[7].type = 3; // crystal-canon type
	listWeapons[7].cost = 30;
	listWeapons[7].rarity = 3;
	listWeapons[7].description = "A strange artefact which shines when gems are close. Probably dangerous.";

	listWeapons[8].name = "SOLAR GUN";
	listWeapons[8].degat = 6;
	listWeapons[8].efficiency = 80;
	listWeapons[8].strengthRequired = 10;
	listWeapons[8].state = 10;
	listWeapons[8].range = 6;
	listWeapons[8].type = 2;
	listWeapons[8].cost = 15;
	listWeapons[8].rarity = 2;
	listWeapons[8].description = "Works even where there isn't sun !";

	listWeapons[9].name = "PORTATIVE CANON";
	listWeapons[9].degat = 9;
	listWeapons[9].efficiency = 80;
	listWeapons[9].strengthRequired = 12;
	listWeapons[9].state = 50;
	listWeapons[9].range = 6;
	listWeapons[9].type = 1;
	listWeapons[9].cost = 10;
	listWeapons[9].rarity = 2;
	listWeapons[9].description = "BE CAREFUL !";

	listWeapons[10].name = "CRYSTAL SWORD";
	listWeapons[10].degat = 1;
	listWeapons[10].efficiency = 100;
	listWeapons[10].strengthRequired = 9;
	listWeapons[10].state = 1;
	listWeapons[10].range = 1;
	listWeapons[10].type = 0;
	listWeapons[10].cost = 20;
	listWeapons[10].rarity = 3;
	listWeapons[10].description = "A strange artefact which shines when gems are close. Probably indestructible";

	listWeapons[11].name = "ELECTRIC SPEAR";
	listWeapons[11].degat = 7;
	listWeapons[11].efficiency = 85;
	listWeapons[11].strengthRequired = 10;
	listWeapons[11].state = 100;
	listWeapons[11].range = 1;
	listWeapons[11].type = 0;
	listWeapons[11].cost = 7;
	listWeapons[11].rarity = 1;
	listWeapons[11].description = "Power supply: 3735928559V";

	listWeapons[12].name = "HUNTING KNIFE";
	listWeapons[12].degat = 6;
	listWeapons[12].efficiency = 95;
	listWeapons[12].strengthRequired = 5;
	listWeapons[12].state = 100;
	listWeapons[12].range = 1;
	listWeapons[12].type = 0;
	listWeapons[12].cost = 5;
	listWeapons[12].rarity = 1;
	listWeapons[12].description = "Who is the idiot who brought that in a cavern ?";

	listWeapons[13].name = "AXE";
	listWeapons[13].degat = 8;
	listWeapons[13].efficiency = 75;
	listWeapons[13].strengthRequired = 12;
	listWeapons[13].state = 100;
	listWeapons[13].range = 1;
	listWeapons[13].type = 0;
	listWeapons[13].cost = 10;
	listWeapons[13].rarity = 1;
	listWeapons[13].description = "Old but gold";

	listWeapons[14].name = "GOD TENTACLE";
	listWeapons[14].degat = 100;
	listWeapons[14].efficiency = 10;
	listWeapons[14].strengthRequired = 15;
	listWeapons[14].state = 100;
	listWeapons[14].range = 1;
	listWeapons[14].type = 0;
	listWeapons[14].cost = 0;
	listWeapons[14].rarity = 3;
	listWeapons[14].description = "Yes, a god tentacle.";

	listArmor[0].name = "BONE ARMOR";
	listArmor[0].resistance = 9;
	listArmor[0].state = 100;
	listArmor[0].strengthRequired = 11;
	listArmor[0].rarity = 1;
	listArmor[0].description = "A bit creepy";
	listArmor[0].cost = 5;

	listArmor[1].name = "MEDICAL SUIT"; // heal the player
	listArmor[1].resistance = 8;
	listArmor[1].state = 100;
	listArmor[1].strengthRequired = 7;
	listArmor[1].rarity = 2;
	listArmor[1].description = "This armor, gracefully offered by MultiplexCorp, will heal all your wounds";
	listArmor[1].cost = 20;

	listArmor[2].name = "EXPLORER SUIT";
	listArmor[2].resistance = 10;
	listArmor[2].state = 100;
	listArmor[2].strengthRequired = 8;	
	listArmor[2].rarity = 1;
	listArmor[2].description = "This suit belonged to another idiot";
	listArmor[2].cost = 10;

	listArmor[3].name = "HUNTER ARMOR";
	listArmor[3].resistance = 12;
	listArmor[3].state = 100;
	listArmor[3].strengthRequired = 11;
	listArmor[3].rarity = 1;
	listArmor[3].description = "There aren't anything to hunt in this cavern...";
	listArmor[3].cost = 5;

	listArmor[4].name = "GUARDIAN HABIT";
	listArmor[4].resistance = 11;
	listArmor[4].state = 100;
	listArmor[4].strengthRequired = 9;
	listArmor[4].rarity = 1;
	listArmor[4].description = "Theft is bad.";	
	listArmor[4].cost = 5;		

	listArmor[5].name = "EXOSKELETON MK-1";
	listArmor[5].resistance = 14;
	listArmor[5].state = 100;
	listArmor[5].strengthRequired = 12;
	listArmor[5].rarity = 2;
	listArmor[5].description = "Can brave the flames ! Flying will be added during the next update.";
	listArmor[5].cost = 35;

	listArmor[6].name = "ANTI-RAD ARMOR"; // this armor can avoid radiations on radioactive zones
	listArmor[6].resistance = 8;
	listArmor[6].state = 100;
	listArmor[6].strengthRequired = 10;	
	listArmor[6].rarity = 2;
	listArmor[6].description = "Ignore the power of radioactivity ! The name isn't explicit enough ?";
	listArmor[6].cost = 20;

	listArmor[7].name = "CRYSTAL ARMOR";
	listArmor[7].resistance = 1;
	listArmor[7].state = 50;
	listArmor[7].strengthRequired = 1;
	listArmor[7].rarity = 3;
	listArmor[7].description = "Could resist to nuke with a certain ammount of gem...";
	listArmor[7].cost = 30;

	listArmor[8].name = "SYMBIOT"; // when you're equiped with this, every time you kill an enemy you get HP
	listArmor[8].resistance = 0;
	listArmor[8].state = 100;
	listArmor[8].strengthRequired = 0;
	listArmor[8].rarity = 3;
	listArmor[8].description = "Can make you a bit... hungry";
	listArmor[8].cost = 0;

	listArmor[9].name = "ARMORLESS";
	listArmor[9].resistance = 0;
	listArmor[9].strengthRequired = 0;
	listArmor[9].state = -1;
	listArmor[9].rarity = 1;
	listArmor[9].description = "Exhibitionist !";
	listArmor[9].cost = 0;
}
