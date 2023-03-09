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

void GameObjInst::Player_Slash(float angle) {
	AEVec2 Pos = posCurr + velCurr;
	Pos.x += -cos(angle) / 1.3f;
	Pos.y += -sin(angle) / 1.3f;
	staticObjInst* slashObj = staticObjInstCreate(TYPE_SLASH, 1.5, &Pos, angle + PI);
	slashObj->timetracker = 0;
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

void staticObjInst::shootBullet() {
	AEVec2 velocity;
	AEVec2 position = posCurr;
	switch ((int)(dirCurr * 57)) {
	case 0: // facing down
		velocity = { 0, -1 };
		position.y -= 0.6f;
		break;
	case 89: // facing right
		velocity = { 1, 0 };
		position.x += 0.6f;
		break;
	case 179: // facing up
		velocity = { 0, 1 };
		position.y += 0.6f;
		break;
	case -89: // facing left
		velocity = { -1, 0 };
		position.x -= 0.6f;
		break;
	default:
		break;
	}
	GameObjInst* jInst = gameObjInstCreate(TYPE_BULLET, 0.5f, &position, &velocity, 0);
	jInst->TextureMap = TEXTURE_BULLET;
}

void GameObjInst::dustParticles() {
	AEVec2 reverse;
	AEVec2Neg(&reverse, &velCurr);
	internalTimer += g_dt;
	if (internalTimer > 0.25f)
	{
		AEVec2 particlecoords = posCurr;
		particlecoords.y = posCurr.y - 0.48;
		internalTimer -= 0.25f;
		ParticleSystemRequest(0, 10.6f, &particlecoords,
			&reverse, 1.0f, 0.15f, 10);
	}
	else
	{
		internalTimer += g_dt;
	}
}

void GameObjInst::mobsKilled() {

	srand(time(NULL));
	if (rand() % 2 == 0)
	{
		AEVec2 Pos = { posCurr.x, posCurr.y };
		staticObjInst* Potion = staticObjInstCreate(TYPE_ITEMS, 1, &Pos, 0);
		Potion->TextureMap = TEXTURE_POTION;
	}

	gameObjInstDestroy(this);
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

void staticObjInst:: chest2Potion() {
	TextureMap = TEXTURE_OPENEDCHEST;
	staticObjInst* Potion = staticObjInstCreate(TYPE_ITEMS, 1, &posCurr, 0);
	Potion->TextureMap = TEXTURE_POTION;
}

/******************************************************************************/
/*!
	This function creates a game object instance.

	It takes in input of the type
	of object, the scale, a vector of the position, a vector of the velocity and
	a float of the direction

	It returns a pointer to the GameObjInst that is stored in the Game object
	Instance List.
*/
/******************************************************************************/
GameObjInst* gameObjInstCreate(unsigned long type,
	float scale,
	AEVec2* pPos,
	AEVec2* pVel,
	float dir)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	AE_ASSERT_PARM(type < sGameObjNum);

	// loop through the object instance list to find a non-used object instance
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = sGameObjList + type;
			pInst->flag = FLAG_ACTIVE;
			pInst->scale = scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;

			// return the newly created instance
			sGameObjInstNum++; //Increment the number of game object instance
			return pInst;
		}
	}

	// cannot find empty slot => return 0
	return 0;
}

/******************************************************************************/
/*!
	This function destroys a Game Object Instance pointed to inside the Game
	Object Instance List.
*/
/******************************************************************************/
void gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	sGameObjInstNum--; //Decrement the number of game object instance
	pInst->flag = 0;
}

/******************************************************************************/
/*!
	This function creates a game object instance.

	It takes in input of the type
	of object, the scale, a vector of the position, a vector of the velocity and
	a float of the direction

	It returns a pointer to the GameObjInst that is stored in the Game object
	Instance List.
*/

/******************************************************************************/
staticObjInst* staticObjInstCreate(unsigned long type, float scale, AEVec2* pPos, float dir)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	// loop through the object instance list to find a non-used object instance
	for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
	{
		staticObjInst* pInst = sStaticObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = sGameObjList + type;
			pInst->flag = FLAG_ACTIVE;
			pInst->scale = scale;
			pInst->dirCurr = dir;
			pInst->posCurr = pPos ? *pPos : zero;

			// return the newly created instance
			sStaticObjInstNum++; //Increment the number of game object instance
			return pInst;
		}
	}

	// cannot find empty slot => return 0
	return 0;
}

/******************************************************************************/
/*!
	This function destroys a Game Object Instance pointed to inside the Game
	Object Instance List.
*/
/******************************************************************************/
void staticObjInstDestroy(staticObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	sStaticObjInstNum--; //Decrement the number of game object instance
	pInst->flag = 0;
}
