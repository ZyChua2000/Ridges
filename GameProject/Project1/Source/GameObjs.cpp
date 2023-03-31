#include "main.h"

const float MAX_ENEMY_DISTANCE = 1.0f;							// define the maximum distance at which enemies should stop moving

const float RANGE_FROM_PLAYER = 0.01f;							// define the range for enemy and player intereaction 

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
		AEAudioPlay(HeroDamaged, Damage, 0.3, 1, 0);
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
	AEVec2 temp_velo{velCurr};
	AEVec2Normalize(&velCurr, &temp_velo); // normalize
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

	//AE_ASSERT_PARM(type < sGameObjNum);

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
			pInst->timetracker = 0;
			pInst->damagetint = { 1,1,1 };
			pInst->damagebuffer = 0;
	
			switch (type) {
			case TYPE_CHARACTER:
				pInst->TextureMap = TEXTURE_PLAYER;
				break;
			case TYPE_ENEMY:
				pInst->TextureMap = TEXTURE_ENEMY;
				pInst->health = 3;
				pInst->pathfindtime = 0.25f;
				pInst->pathtimer = pInst->pathfindtime;
				break;
			case TYPE_BULLET:
				pInst->TextureMap = TEXTURE_BULLET;
				break;
			case TYPE_BOSS:
				pInst->TextureMap = TEXTURE_BOSS;
				pInst->health = 140;
				pInst->pathfindtime = 0.25f;
				pInst->pathtimer = pInst->pathfindtime;
				pInst->timetracker = 1;
				break;
			default:
				pInst->TextureMap = { 0,0 };
				break;
			}
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
			pInst->timetracker = 0;
			pInst->timetracker2 = 0;

			switch (type) {
			case TYPE_ITEMS:
				pInst->TextureMap = TEXTURE_POTION;
				break;
			case TYPE_HEALTH:
				pInst->TextureMap = TEXTURE_FULLHEART;
				break;
			case TYPE_LEVERS:
				pInst->TextureMap = TEXTURE_LEVERS;
				break;
			case TYPE_CHEST:
				pInst->TextureMap = TEXTURE_UNOPENEDCHEST;
				break;
			case TYPE_KEY:
				pInst->TextureMap = TEXTURE_KEYS;
				break;
			case TYPE_SPIKE:
				pInst->TextureMap = TEXTURE_SPIKE;
				break;
			case TYPE_TOWER:
				pInst->TextureMap = TEXTURE_TOWER;
				break;
			case TYPE_SPIKE_NONFADE:
				pInst->TextureMap = TEXTURE_SPIKE;
				break;
			case TYPE_BOSSCIRCLE:
				pInst->TextureMap = TEXTURE_BOSSCIRCLE;
				pInst->Alpha = 0.5f;
				break;
			case TYPE_BOSSCIRCLEATTACK:
				pInst->TextureMap = TEXTURE_BOSSCIRCLE;
				break;
			default:
				pInst->TextureMap = { 0,0 };
				break;
			}

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

void staticObjInst::tilt45() {
	dirCurr -= PI / 4;
	posCurr.x -= 0.2f;
}

void Inventory::itemPickUp(staticObjInst* item) {
	switch (item->pObject->type) {
	case TYPE_ITEMS:
		if (Potion < 9)
		{
			Potion++;
		}
		break;
	case TYPE_KEY:
		Key++;
		break;
	default:
		break;
	}
	staticObjInstDestroy(item);
}