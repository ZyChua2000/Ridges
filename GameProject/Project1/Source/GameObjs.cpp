#include "GameObjs.h"

//player health 
void GameObjInst::deducthealth(int damage)
{
	if (health > 0)
	{
		health -= damage;
	}
}
//player health recover
void GameObjInst::recoverhealth(int recover)
{
	if (health < 3)
	{
		health += recover;
	}
}

//player walk
void GameObjInst::walk()
{
	if ((int)(timetracker*4) % 2 == 1) {
		TextureMap.x = 3;
		TextureMap.y = 12;
	}
	else {
		TextureMap.x = 4;
		TextureMap.y = 12;
	}
}


float GameObjInst::calculateDistance(GameObjInst dynamicObj) {
	return sqrt((posCurr.x - dynamicObj.posCurr.x) * (posCurr.x - dynamicObj.posCurr.x) +
		(posCurr.y - dynamicObj.posCurr.y) * (posCurr.y - dynamicObj.posCurr.y));
}

float GameObjInst::calculateDistance(staticObjInst staticObj) {
	return sqrt((posCurr.x - staticObj.posCurr.x) * (posCurr.x - staticObj.posCurr.x) +
		(posCurr.y - staticObj.posCurr.y) * (posCurr.y - staticObj.posCurr.y));
}
