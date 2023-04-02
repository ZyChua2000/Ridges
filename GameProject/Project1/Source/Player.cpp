#include "main.h"
/******************************************************************************/
/*!
\file		Player.cpp
\author 	Chua Zheng Yang, Liu Chengrong
\par    	email: c.zhengyang\@digipen.edu, chengrong.liu\@digipen.edu 
\date   	February 02, 2023
\brief		This source file contains definitions of Player functions

			- drinkPotion done by Liu Chengrong (10%)
			- The rest done by Chua Zheng Yang (90%)


Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

/*!***********************************************************************
	\brief
		This function updates slash's opacity, meant for slash objects
*************************************************************************/
void staticObjInst::playerSlashUpdate() {
	timetracker += g_dt;
	if (timetracker >= 0.2f) {
		Alpha = (timetracker - 0.2f) / 0.4f;
	}
	if (timetracker >= 0.6f) {
		staticObjInstDestroy(this);
	}
}

/*!***********************************************************************
\brief
	This function makes the player walk, inclusive of animation, velocity
	and audio.
\param[in] walkCD
	Buffer when player uses slash
*************************************************************************/
void GameObjInst::playerWalk(float walkCD)
{
	AEVec2 velNull = { 0,0 };
	//Audio
	if (AEInputCheckTriggered(AEVK_W) && !AEInputCheckCurr(AEVK_A) && !AEInputCheckCurr(AEVK_S) && !AEInputCheckCurr(AEVK_D)){
		AEAudioPlay(Movement, MovementGroup, 0.3f, 1, -1);
	}

	if (AEInputCheckTriggered(AEVK_A) && !AEInputCheckCurr(AEVK_W) && !AEInputCheckCurr(AEVK_S) && !AEInputCheckCurr(AEVK_D)) {
		AEAudioPlay(Movement, MovementGroup, 0.3f, 1, -1);
	}

	if (AEInputCheckTriggered(AEVK_S) && !AEInputCheckCurr(AEVK_A) && !AEInputCheckCurr(AEVK_W) && !AEInputCheckCurr(AEVK_D)) {
		AEAudioPlay(Movement, MovementGroup, 0.3f, 1, -1);
	}

	if (AEInputCheckTriggered(AEVK_D) && !AEInputCheckCurr(AEVK_A) && !AEInputCheckCurr(AEVK_S) && !AEInputCheckCurr(AEVK_W)) {
		AEAudioPlay(Movement, MovementGroup, 0.3f, 1, -1);
	}

	//Velocity
	if (walkCD == 0) {
		if (AEInputCheckCurr(AEVK_W) || AEInputCheckCurr(AEVK_UP)) // movement for W key 
		{
			velCurr.y = 1;// this is direction , positive y direction
			AEVec2 temp_velo{ velCurr };
			AEVec2Normalize(&velCurr, &temp_velo);// normalise velocity
			velCurr.y *= (g_dt * PLAYER_SPEED);
		}
		if (AEInputCheckCurr(AEVK_S) || AEInputCheckCurr(AEVK_DOWN))
		{
			velCurr.y = -1;// this is direction , negative y direction
			AEVec2 temp_velo{ velCurr };
			AEVec2Normalize(&velCurr, &temp_velo);// normalise velocity
			velCurr.y *= (g_dt * PLAYER_SPEED);
			
		}
		if (AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT))
		{
			velCurr.x = -1;// this is direction , negative x direction
			AEVec2 temp_velo{ velCurr };
			AEVec2Normalize(&velCurr, &temp_velo);// normalise velocity
			velCurr.x *= (g_dt * PLAYER_SPEED);
			scale = -1;
			
		}
		if (AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT))
		{
			velCurr.x = 1;// this is direction , positive x direction
			AEVec2 temp_velo{ velCurr };
			AEVec2Normalize(&velCurr, &temp_velo);// normalise velocity
			velCurr.x *= (g_dt * PLAYER_SPEED);
			scale = 1;
			
		}

		//Animation
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


/*!***********************************************************************
\brief
	This function creates the slash according to the angle
\param[in] angle
	The angle between the player and the mouse
*************************************************************************/
void GameObjInst::playerSlashCreate(float angle) {
	AEVec2 Pos = posCurr + velCurr;
	Pos.x += -cos(angle) / 1.3f;
	Pos.y += -sin(angle) / 1.3f;
	staticObjInst* slashObj = staticObjInstCreate(TYPE_SLASH, 1.5, &Pos, angle + PI);
	slashObj->timetracker = 0;
	slashObj->Alpha = 0;
	AEAudioPlay(HeroSlash, Damage, 0.3f, 1, 0);
}

/*!***********************************************************************
\brief
	This function makes the player stand still. Resets the velocity to 0
*************************************************************************/
void GameObjInst::playerStand() {
	velCurr = { 0,0 };
	TextureMap = TEXTURE_PLAYER;
}

/*!***********************************************************************
\brief
	This function knocks the player back from touching the mob
\param[in] mob
	The mob that touches the player
*************************************************************************/
void GameObjInst::playerKnockback(GameObjInst mob) {
	AEVec2 nil{ 0,0 };
	if (velCurr == nil) //If standing still
		posCurr += mob.velCurr/2; //Pushback by enemy's velocity
	else posCurr -= velCurr / 2; // If moving, pushback by player's velocity(walking into mob)
}

/*!***********************************************************************
\brief
	This function creates the slash according to the angle
\param[in] angle
	The angle between the player and the mouse
*************************************************************************/
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

/*!***********************************************************************
\brief
	This function makes the player tint from being damaged
\param[in] damageCD
	The time left for invulnerability
*************************************************************************/
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
		else if (damagetint.blue == 1 && damagebuffer > 0.1f){
			damagetint.blue = 0;
			damagetint.green = 0;
			damagebuffer = 0;
		}
	}
}