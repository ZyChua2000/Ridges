#include "main.h"

void staticObjInst::playerSlashUpdate() {
	timetracker += g_dt;
	if (timetracker >= 0.2f) {
		Alpha = (timetracker - 0.2f) / 0.4f;
	}
	if (timetracker >= 0.6f) {
		staticObjInstDestroy(this);
	}
}

//player walk
void GameObjInst::playerWalk(float walkCD)
{
	if (walkCD == 0) {
		if (AEInputCheckCurr(AEVK_W) || AEInputCheckCurr(AEVK_UP)) // movement for W key 
		{
			velCurr.y = 1;// this is direction , positive y direction
			AEVec2Normalize(&velCurr, &velCurr);// normalise velocity
			velCurr.y *= (g_dt * PLAYER_SPEED);
		}
		if (AEInputCheckCurr(AEVK_S) || AEInputCheckCurr(AEVK_DOWN))
		{
			velCurr.y = -1;// this is direction , negative y direction
			AEVec2Normalize(&velCurr, &velCurr);// normalise velocity
			velCurr.y *= (g_dt * PLAYER_SPEED);
		}
		if (AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT))
		{
			velCurr.x = -1;// this is direction , negative x direction
			AEVec2Normalize(&velCurr, &velCurr);// normalise velocity
			velCurr.x *= (g_dt * PLAYER_SPEED);
			scale = -1;
		}
		if (AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT))
		{
			velCurr.x = 1;// this is direction , positive x direction
			AEVec2Normalize(&velCurr, &velCurr);// normalise velocity
			velCurr.x *= (g_dt * PLAYER_SPEED);
			scale = 1;
		}

		if ((int)(timetracker * 4) % 2 == 1) {
			TextureMap.x = 3;
			TextureMap.y = 12;
		}
		else {
			TextureMap.x = 4;
			TextureMap.y = 12;
		}
	}
}

void GameObjInst::playerSlashCreate(float angle) {
	AEVec2 Pos = posCurr + velCurr;
	Pos.x += -cos(angle) / 1.3f;
	Pos.y += -sin(angle) / 1.3f;
	staticObjInst* slashObj = staticObjInstCreate(TYPE_SLASH, 1.5, &Pos, angle + PI);
	slashObj->timetracker = 0;
	slashObj->Alpha = 0;
}

void GameObjInst::playerStand() {
	velCurr = { 0,0 };
	TextureMap = TEXTURE_PLAYER;
}

void GameObjInst::playerKnockback(GameObjInst mob) {
	AEVec2 nil{ 0,0 };
	if (velCurr == nil) //If standing still
		posCurr += mob.velCurr / 4; //Pushback by enemy's velocity
	else posCurr -= velCurr / 4; // If moving, pushback by player's velocity(walking into mob)
}

void GameObjInst::drinkPotion(staticObjInst* menuObj[3], Inventory& backPack) {
	if (health > 0 && health < 3 && backPack.Potion > 0)
	{
		recoverhealth();
		switch (health)
		{
		case 2:
			menuObj[1]->TextureMap = TEXTURE_FULLHEART;
			break;
		case 3:
			menuObj[0]->TextureMap = TEXTURE_FULLHEART;
			break;
		}
		backPack.Potion--;
	}
}

void GameObjInst::playerDamaged(float damageCD) {
	if (damageCD == 0) {
		damagetint = { 1,1,1 };
		damagebuffer = 0;
	}
	else {
		damagebuffer += g_dt;
		if (damagetint.blue == 0 && damagebuffer > 0.1f) {
			damagetint.blue = 1;
			damagetint.green = 1;
			damagebuffer = 0;
		}
		else if (damagetint.blue == 1 && damagebuffer > 0.1f) {
			damagetint.blue = 0;
			damagetint.green = 0;
			damagebuffer = 0;
		}
	}
}