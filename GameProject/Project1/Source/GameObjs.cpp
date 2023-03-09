#include "main.h"

void staticObjInst::spikeUpdate() {
	if (timetracker2 == 0) {
		timetracker += g_dt;
	}
	if (timetracker > SPIKE_TRANSITION_t) {
		timetracker = SPIKE_TRANSITION_t;
	}

	if (timetracker == SPIKE_TRANSITION_t) {
		timetracker2 += g_dt;
	}

	if (timetracker2 > SPIKE_LIFE_t) {
		timetracker2 = SPIKE_LIFE_t;
	}

	if (timetracker2 == SPIKE_LIFE_t) {
		timetracker -= g_dt;
	}

	if (timetracker < 0) {
		timetracker = 0;
	}

	if (timetracker == 0) {
		timetracker2 -= (g_dt / SPIKE_DEATH_t) * SPIKE_LIFE_t;
	}
	if (timetracker2 < 0) {
		timetracker2 = 0;
	}

	Alpha = 1.0f - timetracker / SPIKE_TRANSITION_t;
}

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
void GameObjInst::walk(float walkCD)
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


float GameObjInst::calculateDistance(GameObjInst dynamicObj) {
	return sqrt((posCurr.x - dynamicObj.posCurr.x) * (posCurr.x - dynamicObj.posCurr.x) +
		(posCurr.y - dynamicObj.posCurr.y) * (posCurr.y - dynamicObj.posCurr.y));
}

float GameObjInst::calculateDistance(staticObjInst staticObj) {
	return sqrt((posCurr.x - staticObj.posCurr.x) * (posCurr.x - staticObj.posCurr.x) +
		(posCurr.y - staticObj.posCurr.y) * (posCurr.y - staticObj.posCurr.y));
}

void GameObjInst::calculateBB() {
	boundingBox.min.x = -(BOUNDING_RECT_SIZE / 2.0f) * abs(scale) + posCurr.x;
	boundingBox.min.y = -(BOUNDING_RECT_SIZE / 2.0f) * abs(scale) + posCurr.y;
	boundingBox.max.x = (BOUNDING_RECT_SIZE / 2.0f) * abs(scale) + posCurr.x;
	boundingBox.max.y = (BOUNDING_RECT_SIZE / 2.0f) * abs(scale) + posCurr.y;
}

void staticObjInst::calculateBB() {
	boundingBox.min.x = -(BOUNDING_RECT_SIZE / 2.0f) * abs(scale) + posCurr.x;
	boundingBox.min.y = -(BOUNDING_RECT_SIZE / 2.0f) * abs(scale) + posCurr.y;
	boundingBox.max.x = (BOUNDING_RECT_SIZE / 2.0f) * abs(scale) + posCurr.x;
	boundingBox.max.y = (BOUNDING_RECT_SIZE / 2.0f) * abs(scale) + posCurr.y;
}

void GameObjInst::velToPos(float speed) {
	posCurr += velCurr * g_dt * speed;
}

void GameObjInst::calculateTransMatrix() {
	AEMtx33		 transMat = { 0 }, rotMat = { 0 }, scaleMat = { 0 };

	float scaleY;
	// Compute the scaling matrix
	if (scale < 0) {
		scaleY = -scale;
	}
	else {
		scaleY = scale;
	}
	AEMtx33Scale(&scaleMat, scale * SPRITE_SCALE, scaleY * SPRITE_SCALE);
	// Compute the rotation matrix 
	AEMtx33Rot(&rotMat, dirCurr);
	// Compute the translation matrix
	AEMtx33Trans(&transMat, posCurr.x * SPRITE_SCALE, posCurr.y * SPRITE_SCALE);
	// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
	AEMtx33Concat(&transform, &rotMat, &scaleMat);
	AEMtx33Concat(&transform, &transMat, &transform);
}

void staticObjInst::calculateTransMatrix() {
	AEMtx33		 transMat = { 0 }, rotMat = { 0 }, scaleMat = { 0 };


	AEMtx33Scale(&scaleMat, scale * SPRITE_SCALE, scale * SPRITE_SCALE);
	// Compute the rotation matrix 
	AEMtx33Rot(&rotMat, dirCurr);
	// Compute the translation matrix
	AEMtx33Trans(&transMat, posCurr.x * SPRITE_SCALE, posCurr.y * SPRITE_SCALE);
	// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
	AEMtx33Concat(&transform, &rotMat, &scaleMat);
	AEMtx33Concat(&transform, &transMat, &transform);
}

void staticObjInst::mapEditorObjectSpawn() {
	scale = 0.7f;
	posCurr = { mouseX + camX + 0.3f, mouseY + camY + 0.3f };
	if (AEInputCheckTriggered(AEVK_K) && TextureMap.y < TEXTURE_MAXHEIGHT / TEXTURE_CELLSIZE) {
		TextureMap.y += 1;
	}
	if (AEInputCheckTriggered(AEVK_I) && TextureMap.y > 0) {
		TextureMap.y -= 1;
	}
	if (AEInputCheckTriggered(AEVK_J) && TextureMap.x > 0) {
		TextureMap.x -= 1;
	}
	if (AEInputCheckTriggered(AEVK_L) && TextureMap.x < TEXTURE_MAXWIDTH / TEXTURE_CELLSIZE) {
		TextureMap.x += 1;
	}
}

