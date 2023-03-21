/******************************************************************************/
/*!
\file		World.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 02, 2023
\brief		This header file contains the functions for the level of World.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Main.h"
#include <fstream>
#include <iostream>
#include <time.h>


/*!
	Defines
*/
/******************************************************************************/
static saveData				data;
static Node* nodes{};


static const unsigned int	MAX_MOBS = 11;							// The total number of mobs
static int			CURRENT_MOBS = MAX_MOBS;
static const unsigned int	MAX_CHESTS = 6;						// The total number of chests
static const unsigned int	MAX_LEVERS = 3;						// The total number of levers
static const unsigned int	MAX_KEYS;							// The total number of keys

static bool					SLASH_ACTIVATE = false;				// Bool to run slash animation

static const int			MAP_CELL_WIDTH = 124;				// Total number of cell widths
static const int			MAP_CELL_HEIGHT = 42;				// Total number of cell heights

static const AEVec2			levelClearMin = { 38,-14 };
static const AEVec2			levelClearMax = { 40,-13 };

static unsigned int			state = 0;							// Debugging state
static unsigned int			mapeditor = 0;						// Map edtior state


// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
// object flag definition


/******************************************************************************/
/*!
	Struct/Class Definitions
*/
/******************************************************************************/

// ---------------------------------------------------------------------------


static AEVec2				MapObjInstList[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];	// 2D array of each map tile object
static int					binaryMap[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];	// 2D array of binary collision mapping

static float slashCD = 0;
static float walkCD = 0;

// pointer to the objects
static GameObjInst* Player;												// Pointer to the "Player" game object instance
static staticObjInst* mapEditorObj;										// Pointer to the reference map editor object instance
static staticObjInst* Health[3];										// Pointer to the player health statc object instance
static staticObjInst* Levers[3];										// Pointer to each enemy object instance
static staticObjInst* MenuObj[3];										// Pointer to each enemy object instance
static staticObjInst* NumObj[3];
static staticObjInst* Chest[MAX_CHESTS];
static staticObjInst* Key;
static Inventory Backpack;
static staticObjInst* Spike;
static staticObjInst* RefBox;

static AEVec2* Gates;
static int gatesNum;
static int levNum;
static int chestNum;

static const float timingFIRST = 0.0f;
static const float timingSECOND = 0.6f;
static const float timingTHIRD = 1.2f;
static const float timingFOURTH = 1.8f;



// ---------------------------------------------------------------------------

/******************************************************************************/


int CheckInstanceBinaryMapCollision(float PosX, float PosY,
	float scaleX, float scaleY);


/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the World level.
	It should be called once before the start of the level.clic
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_Tower_Load(void) {
	// zero the game object array
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	sGameObjNum = 0;

	// zero the game object instance array
	//memset(sGameObjInstList, 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sGameObjInstNum = 0;

	// zero the game object instance array
	//memset(sStaticObjInstList, 0, sizeof(staticObjInst) * STATIC_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sStaticObjInstNum = 0;

	// The ship object instance hasn't been created yet, so this "spShip" pointer is initialized to 0
	Player = nullptr;

	//IN CREATING GAME OBJECTS, MUST DO IN SAME ORDER AS ENUM

	GameObj* Character;
	Character = sGameObjList + sGameObjNum++;

	AEGfxMeshStart();

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFF00FF, 16.0f / TEXTURE_MAXWIDTH, 16.0f / TEXTURE_MAXHEIGHT,
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 16.0f / TEXTURE_MAXHEIGHT,
		0.5f, 0.5f, 0xFF00FFFF, 16.0f / TEXTURE_MAXWIDTH, 0.0f);

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 16.0f / TEXTURE_MAXHEIGHT,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 16.0f / TEXTURE_MAXWIDTH, 0.0f);

	Character->pMesh = AEGfxMeshEnd();
	Character->pTexture = AEGfxTextureLoad("Assets/Tilemap/tilemap_packed.png");
	Character->type = TYPE_CHARACTER;

	GameObj* NPC;
	NPC = sGameObjList + sGameObjNum++;
	NPC->pMesh = Character->pMesh;
	NPC->pTexture = Character->pTexture;
	NPC->type = TYPE_NPCS;
	NPC->refMesh = true;
	NPC->refTexture = true;

	GameObj* Item;
	Item = sGameObjList + sGameObjNum++;
	Item->pMesh = Character->pMesh;
	Item->pTexture = Character->pTexture;
	Item->type = TYPE_ITEMS;
	Item->refMesh = true;
	Item->refTexture = true;

	GameObj* Map;
	Map = sGameObjList + sGameObjNum++;
	Map->pMesh = Character->pMesh;
	Map->pTexture = Character->pTexture;
	Map->type = TYPE_MAP;
	Map->refMesh = true;
	Map->refTexture = true;



	//Enemy*  enemy;
	//enemy = static_pointer_cast<Ene*>(sGameObjList + sGameObjNum++);
	//enemy->pMesh = Character->pMesh;
	//enemy->pTexture = Character->pTexture;

	AEGfxMeshStart();

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
		0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f);

	GameObj* Slash;
	Slash = sGameObjList + sGameObjNum++;
	Slash->pMesh = AEGfxMeshEnd();
	Slash->pTexture = AEGfxTextureLoad("Assets/slash.png");
	Slash->type = TYPE_SLASH;

	GameObj* RefLine;
	RefLine = sGameObjList + sGameObjNum++;
	RefLine->pMesh = Slash->pMesh;
	RefLine->pTexture = AEGfxTextureLoad("Assets/Tilemap/RefBox.png");
	RefLine->type = TYPE_REF;
	RefLine->refMesh = true;

	GameObj* Health;
	Health = sGameObjList + sGameObjNum++;
	Health->pMesh = Character->pMesh;
	Health->pTexture = Character->pTexture;
	Health->type = TYPE_HEALTH;
	Health->refMesh = true;
	Health->refTexture = true;

	GameObj* Lever;
	Lever = sGameObjList + sGameObjNum++;
	Lever->pMesh = Character->pMesh;
	Lever->pTexture = Character->pTexture;
	Lever->type = TYPE_LEVERS;
	Lever->refMesh = true;
	Lever->refTexture = true;

	GameObj* Enemy;
	Enemy = sGameObjList + sGameObjNum++;
	Enemy->pMesh = Character->pMesh;
	Enemy->pTexture = Character->pTexture;
	Enemy->type = TYPE_ENEMY;
	Enemy->refMesh = true;
	Enemy->refTexture = true;

	GameObj* Chest;
	Chest = sGameObjList + sGameObjNum++;
	Chest->pMesh = Character->pMesh;
	Chest->pTexture = Character->pTexture;
	Chest->type = TYPE_CHEST;
	Chest->refMesh = true;
	Chest->refTexture = true;

	GameObj* Key;
	Key = sGameObjList + sGameObjNum++;
	Key->pMesh = Character->pMesh;
	Key->pTexture = Character->pTexture;
	Key->type = TYPE_KEY;
	Key->refMesh = true;
	Key->refTexture = true;

	GameObj* Spike;
	Spike = sGameObjList + sGameObjNum++;
	Spike->pMesh = Character->pMesh;
	Spike->pTexture = Character->pTexture;
	Spike->type = TYPE_SPIKE;
	Spike->refMesh = true;
	Spike->refTexture = true;


	GameObj* Spike_nonfade;
	Spike_nonfade = sGameObjList + sGameObjNum++;
	Spike_nonfade->pMesh = Character->pMesh;
	Spike_nonfade->pTexture = Character->pTexture;
	Spike_nonfade->type = TYPE_SPIKE_NONFADE;
	Spike_nonfade->refMesh = true;
	Spike_nonfade->refTexture = true;

	GameObj* Mask;
	Mask = sGameObjList + sGameObjNum++;
	Mask->pMesh = Character->pMesh;
	Mask->pTexture = Character->pTexture;
	Mask->type = TYPE_MASK;
	Mask->refMesh = true;
	Mask->refTexture = true;


	GameObj* Tower;
	Tower = sGameObjList + sGameObjNum++;
	Tower->pMesh = Character->pMesh;
	Tower->pTexture = Character->pTexture;
	Tower->type = TYPE_TOWER;
	Tower->refMesh = true;
	Tower->refTexture = true;

	GameObj* Bullet;
	Bullet = sGameObjList + sGameObjNum++;
	Bullet->pMesh = Character->pMesh;
	Bullet->pTexture = Character->pTexture;
	Bullet->type = TYPE_BULLET;
	Bullet->refMesh = true;
	Bullet->refTexture = true;
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the World state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_Tower_Init(void) {
	AEVec2* pos = nullptr;
	int num;

	utilities::loadObjs(Gates, gatesNum, "towerGates.txt");

	// =====================================
	//	Initialize map textures
	// =====================================

	utilities::importMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureTower.txt");

	// =====================================
	//	Initialize map binary
	// =====================================

	utilities::importMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *binaryMap, "binaryTower.txt");

	mapEditorObj = staticObjInstCreate(TYPE_MAP, 0, nullptr, 0);


	// =====================================
	//	Initialize objects for new game
	// =====================================
		//Initialise Player
		AEVec2 PlayerPos = { 58.5,-6 };
		Player = gameObjInstCreate(TYPE_CHARACTER, 1, &PlayerPos, 0, 0);
		
		std::ifstream ifs{ "Assets/save.txt" };
		ifs >> Player->health;
		ifs >> Backpack.Key; //set to player number of current potion
		ifs >> Backpack.Potion; //set to player number of current key
		ifs.close();

		//Initialise player health.
		for (int i = 0; i < 3; i++) {
			Health[i] = staticObjInstCreate(TYPE_HEALTH, 0.75, nullptr, 0);
		}

		//Initialise Levers in level
		utilities::loadObjs(pos, levNum, "towerLevers.txt");

		for (int i = 0; i < levNum; i++) {
			Levers[i] = staticObjInstCreate(TYPE_LEVERS, 1, &pos[i], 0);
		}

		utilities::unloadObjs(pos);

		//Initialise enemy in level
		utilities::loadObjs(pos, CURRENT_MOBS, "towerEnemy.txt");
		for (int i = 0; i < CURRENT_MOBS; i++) {
			GameObjInst* enemy = gameObjInstCreate(TYPE_ENEMY, 1, &pos[i], 0, 0);
			enemy->health = 3;
			enemy->pathfindtime = 0.25f;
			enemy->pathtimer = enemy->pathfindtime;
		}
		utilities::unloadObjs(pos);


	


	// =====================================
	//	Initialize objects for loaded game
	// =====================================
	utilities::loadObjs(pos, num, "towerTowersD.txt");

	float towerdowntiming[14] = {
	timingFIRST,
	timingSECOND,
	timingFIRST,
	timingSECOND,
	timingFIRST,
	timingSECOND,
	timingFIRST,
	timingSECOND,
	timingFIRST,
	timingSECOND,
	timingTHIRD,
	timingFOURTH,
	timingSECOND,
	timingTHIRD };
	for (int i = 0; i < num; i++)
	{
		staticObjInst* jInst = staticObjInstCreate(TYPE_TOWER, 1, &pos[i], 0);
		jInst->timetracker = towerdowntiming[i];
	}
	utilities::unloadObjs(pos);

	float towerrighttiming[2] =
	{
		timingFIRST,
		timingSECOND
	};

	utilities::loadObjs(pos, num, "towerTowersR.txt");
	for (int i = 0; i < num; i++)
	{
		staticObjInst* jInst = staticObjInstCreate(TYPE_TOWER, 1, &pos[i], TOWER_RIGHT);
		jInst->timetracker = towerrighttiming[i];
	}
	utilities::unloadObjs(pos);

	// =====================================
	//	Initialize UI objects
	// =====================================
	utilities::loadObjs(pos, num, "towerSpikes.txt");
	for (int i = 0; i < num; i++)
	{
		staticObjInstCreate(TYPE_SPIKE, 1, &pos[i], 0);
	}
	utilities::unloadObjs(pos);
	for (unsigned int i = 0; i < sStaticObjInstNum; i++)
	{
		staticObjInst* pInst = sStaticObjInstList + i;
		if (pInst->pObject->type == TYPE_SPIKE)
		{
			pInst->TextureMap = { 5,3 };
		}
	}

	//Init pathfinding nodes
	NodesInit(*binaryMap, MAP_CELL_WIDTH, MAP_CELL_HEIGHT);

	// Initialise camera pos
	camX = Player->posCurr.x, camY = Player->posCurr.y;

	// =====================================
	//	Initialize UI objects
	// =====================================


	MenuObj[0] = staticObjInstCreate(TYPE_ITEMS, 1, nullptr, 0); // Potions
	MenuObj[1] = staticObjInstCreate(TYPE_KEY, 1, nullptr, 0); // Keys


	NumObj[0] = staticObjInstCreate(TYPE_ITEMS, 1, nullptr, 0); // Potions
	NumObj[1] = staticObjInstCreate(TYPE_KEY, 1, nullptr, 0); // Keys


	ParticleSystemInit();

}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the World state.
*/
/******************************************************************************/

void GS_Tower_Update(void) {

	// Normalising mouse to 0,0 at the center
	s32 mouseIntX, mouseIntY;
	AEInputGetCursorPosition(&mouseIntX, &mouseIntY);
	mouseX = (float)(mouseIntX - AEGetWindowWidth() / 2) / SPRITE_SCALE;
	mouseY = (float)(-mouseIntY + AEGetWindowHeight() / 2) / SPRITE_SCALE;

	float angleMousetoPlayer = utilities::getAngle(Player->posCurr.x, Player->posCurr.y, mouseX + Player->posCurr.x, mouseY + Player->posCurr.y);
	if (mouseY + camY > Player->posCurr.y) {
		angleMousetoPlayer = -angleMousetoPlayer;
	}

	static float playerHitTime = 0;
	//Time-related variables
	utilities::decreaseTime(slashCD);
	utilities::decreaseTime(walkCD);
	utilities::decreaseTime(playerHitTime);


	// =====================================
	// User Input
	// =====================================
	//Debugging mode
	if (AEInputCheckTriggered(AEVK_F3)) {
		state ^= 1;
	}
	//Map editor mode
	if (AEInputCheckTriggered(AEVK_9)) {
		mapeditor ^= 1;
	}


	Player->playerStand();

	if (AEInputCheckCurr(AEVK_W) || AEInputCheckCurr(AEVK_UP) || AEInputCheckCurr(AEVK_S) || AEInputCheckCurr(AEVK_DOWN)
		|| AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT) || AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT)) {
		Player->playerWalk(walkCD);
	}

	//reducing heath for debugging
	if (AEInputCheckTriggered(AEVK_MINUS))
	{
		Player->deducthealth();
		switch (Player->health)
		{
		case 0:
			Health[2]->TextureMap = TEXTURE_DEADHEART;
			break;
		case 1:
			Health[1]->TextureMap = TEXTURE_DEADHEART;
			break;
		case 2:
			Health[0]->TextureMap = TEXTURE_DEADHEART;
		}
	}

	if (AEInputCheckTriggered(AEVK_E)) {

		//Interaction with levers
		for (int lev = 0; lev < levNum; lev++) {
			if (Player->calculateDistance(*Levers[lev]) < 1 && Levers[lev]->dirCurr == 0) {
				//Switch lever to face down
				Levers[lev]->tilt45();
				//Remove gates: Change texture & Binary map
				utilities::unlockGate(lev, *MapObjInstList, *binaryMap, Gates, MAP_CELL_HEIGHT);
			}
		}


		for (int i = 0; i < chestNum; i++)
		{
			//Interaction with Chest
			if (Player->calculateDistance(*Chest[i]) < 1 && Chest[i]->TextureMap.x != 8)
			{
				//change texture of chest
				Chest[i]->chest2Potion();
			}
		}
	}

	//if pickup potion then add player health
	if (AEInputCheckTriggered(AEVK_R))
	{
		Player->drinkPotion(Health, Backpack);
	}

	if (AEInputCheckTriggered(AEVK_LBUTTON) && slashCD == 0) {
		SLASH_ACTIVATE = true;
		slashCD = SLASH_COOLDOWN_t;
		walkCD = WALK_COOLDOWN_t;
		Player->playerStand();
	}

	if (SLASH_ACTIVATE == true) {
		Player->playerSlashCreate(angleMousetoPlayer);
		SLASH_ACTIVATE = false;
	}

	if (mapeditor == 1) {
		mapEditorObj->mapEditorObjectSpawn(mouseX, mouseY, camX, camY);

		utilities::changeMapObj(mouseX + camX, mouseY + camY, MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, *mapEditorObj);

	}
	else {
		mapEditorObj->scale = 0;
	}

	//Map editor printing
	if (AEInputCheckTriggered(AEVK_8)) {
		utilities::exportMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureTower.txt");

		utilities::exportMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "binaryTower.txt");
	}

	if (AEInputCheckTriggered(AEVK_M)) {
		gGameStateNext = GS_MAINMENU;
	}


	for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
	{
		staticObjInst* pInst = sStaticObjInstList + i;
		if (pInst->flag != FLAG_ACTIVE || (pInst->pObject->type != TYPE_KEY && pInst->pObject->type != TYPE_ITEMS))
		{
			continue;
		}
		//Interaction with items
		if (Player->calculateDistance(*pInst) < 0.5f)
		{
			Backpack.itemPickUp(pInst);
		}
	}


	// ======================================================
	// update physics of all active game object instances
	//  -- Get the AABB bounding rectangle of every active instance:
	//		boundingRect_min = -(BOUNDING_RECT_SIZE/2.0f) * instance->scale + instance->pos
	//		boundingRect_max = +(BOUNDING_RECT_SIZE/2.0f) * instance->scale + instance->pos


		// Pathfinding for Enemy AI
	for (int j = 0; j < GAME_OBJ_INST_NUM_MAX; j++)
	{
		GameObjInst* pEnemy = sGameObjInstList + j;


		// skip non-active object
		if (pEnemy->flag != FLAG_ACTIVE || pEnemy->pObject->type != TYPE_ENEMY)
			continue;

		if (Player->calculateDistance(*pEnemy) > 10)
			continue;

		pEnemy->mobsPathFind(*Player);
	}


	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;
		if (pInst->flag != FLAG_ACTIVE) {
			continue;
		}
		pInst->calculateBB();
	}

	for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++) {
		staticObjInst* pInst = sStaticObjInstList + i;
		if (pInst->flag != FLAG_ACTIVE) {
			continue;
		}
		if (pInst->pObject->type != TYPE_SLASH && pInst->pObject->type != TYPE_SPIKE) {
			continue;
		}
		pInst->calculateBB();
	}


	// ======================================================
	//	-- Positions of the instances are updated here with the already computed velocity (above)col
	// ======================================================
	
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;
		if (pInst->velCurr.x != 0 || pInst->velCurr.y != 0) //if player direction is not 0, as you cannot normalize 0.
		{
			if (pInst->pObject->type == TYPE_CHARACTER) {
				pInst->velToPos(PLAYER_SPEED);
			}
			//invert movement for binary map

			if (pInst->pObject->type == TYPE_ENEMY) {
				pInst->velToPos(NPC_SPEED);
			}

			if (pInst->pObject->type == TYPE_BULLET) {
				pInst->velToPos(BULLET_SPEED);
			}
		}
	}

	MenuObj[0]->posCurr = { (float)camX - 9.0f, (float)camY + 5.0f };
	NumObj[0]->posCurr = { (float)camX - 8.0f, (float)camY + 5.0f };

	MenuObj[1]->posCurr = { (float)camX - 6.0f, (float)camY + 5.0f };
	NumObj[1]->posCurr = { (float)camX - 5.0f, (float)camY + 5.0f };

	//player health following viewport
	Health[0]->posCurr = { (float)camX + 7.0f , (float)camY + 5.0f };
	Health[1]->posCurr = { (float)camX + 8.0f , (float)camY + 5.0f };
	Health[2]->posCurr = { (float)camX + 9.0f , (float)camY + 5.0f };

	// ====================
	// check for collision
	// ====================

	//if player receive damage from collision or from mob, player decrease health
	for (int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;
		if (pInst->flag != FLAG_ACTIVE) {
			continue;
		}

		if (pInst->pObject->type == TYPE_ENEMY) {

			if (CollisionIntersection_RectRect(Player->boundingBox, Player->velCurr, pInst->boundingBox, pInst->velCurr)
				&& playerHitTime == 0)
			{
				if (Player->health > 0)
				{
					Player->deducthealth();

					//Hit cooldown
					playerHitTime = DAMAGE_COODLDOWN_t;

					//knockback
					Player->playerKnockback(*pInst);

				}
			}

			for (int j = 0; j < STATIC_OBJ_INST_NUM_MAX; j++) {
				staticObjInst* jInst = sStaticObjInstList + j;
				if (jInst->flag != FLAG_ACTIVE || jInst->pObject->type != TYPE_SLASH) {
					continue;
				}

				if (pInst->calculateDistance(*jInst) < 0.9f
					&& jInst->Alpha == 0) {
					pInst->deducthealth(Player->damage);
					// Knockback
					pInst->mobKnockback(*jInst);
				}
			}
		}

		if (pInst->pObject->type == TYPE_BULLET) {
			int flag = CheckInstanceBinaryMapCollision(pInst->posCurr.x, -pInst->posCurr.y, pInst->scale, pInst->scale, binaryMap);
			if (CollisionIntersection_RectRect(Player->boundingBox, Player->velCurr, pInst->boundingBox, pInst->velCurr)) {
				Player->deducthealth();
				gameObjInstDestroy(pInst);
			}
			if (snapCollision(*pInst, flag)) {
				gameObjInstDestroy(pInst);
			}
		}

		if (Player->health == 0) {
			gGameStateNext = GS_DEATHSCREEN;
		}

		switch (Player->health)
		{
		case 0:
			Health[2]->TextureMap = TEXTURE_DEADHEART;
			break;
		case 1:
			Health[1]->TextureMap = TEXTURE_DEADHEART;
			break;
		case 2:
			Health[0]->TextureMap = TEXTURE_DEADHEART;
		}
	}

	int flag = CheckInstanceBinaryMapCollision(Player->posCurr.x, -Player->posCurr.y, 1.0f, 1.0f, binaryMap);

	snapCollision(*Player, flag);

	for (int i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++) {
		staticObjInst* pInst = sStaticObjInstList + i;
		if (pInst->flag != 1 || pInst->pObject->type != TYPE_SPIKE) {
			continue;
		}

		pInst->spikeUpdate(); // Updates alpha of spikes

		if (Player->calculateDistance(*pInst) <= 0.8f && (pInst->Alpha == 0) && playerHitTime == 0) {

			Player->deducthealth();
			playerHitTime = DAMAGE_COODLDOWN_t;
		}

	}

	// ===================================
	// update active game object instances
	// Example:
	//		-- Removing effects after certain time
	//		-- Removing dead objects
	// ===================================
	for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
	{
		staticObjInst* pInst = sStaticObjInstList + i;
		if (pInst->flag != FLAG_ACTIVE) {
			continue;
		}

		if (pInst->pObject->type == TYPE_SLASH) {
			pInst->playerSlashUpdate();
		}

		if (pInst->pObject->type == TYPE_TOWER) {
			utilities::decreaseTime(pInst->timetracker);

			if (pInst->timetracker == 0) {
				pInst->timetracker = TOWER_REFRESH;
				pInst->shootBullet();
			}

		}

	}

	for (int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;
		if (pInst->flag != FLAG_ACTIVE) {
			continue;
		}

		if (pInst->pObject->type == TYPE_ENEMY)
		{
			if (pInst->health == 0)
			{
				pInst->mobsKilled();
				CURRENT_MOBS -= 1;
			}
		}

		if (pInst->pObject->type == TYPE_CHARACTER) {
			pInst->timetracker += g_dt;
		}
	}

	if (MAX_MOBS - CURRENT_MOBS == 2) { //If first 2 tutorial monsters are killed,unlock gate 1
		utilities::unlockGate(gatesNum / 2 - 1, *MapObjInstList, *binaryMap, Gates, MAP_CELL_HEIGHT); //Tutorial gate is last gate in list
	}


	// Clear condition


	if (utilities::inRange(Player,levelClearMin,levelClearMax)) {
		utilities::completeLevel(tower, Player, Backpack);
	}


	// =====================================
	// calculate the matrix for all objects
	// =====================================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		pInst->calculateTransMatrix();
	}

	for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
	{
		staticObjInst* pInst = sStaticObjInstList + i;


		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		pInst->calculateTransMatrix();
	}

	// Camera position and UI items


	NumObj[0]->TextureMap = TEXTURE_NUMBERS[Backpack.Potion];
	NumObj[1]->TextureMap = TEXTURE_NUMBERS[Backpack.Key];


	utilities::snapCamPos(Player->posCurr, camX, camY, MAP_CELL_WIDTH, MAP_CELL_HEIGHT);

	//BUG NOT WORKING
	//Player->dustParticles();

	ParticleSystemUpdate();
	AEGfxSetCamPosition(static_cast<f32>(static_cast<int>(camX * (float)SPRITE_SCALE)), static_cast<f32>(static_cast<int> (camY * (float)SPRITE_SCALE)));

}

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_Tower_Draw(void) {


	// Tell the engine to get ready to draw something with texture. 
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	for (unsigned long i = 0; i < MAP_CELL_WIDTH; i++) {
		for (long j = 0; j < MAP_CELL_HEIGHT; j++) {
			AEVec2 Pos = { i + 0.5f, -j - 0.5f };

			if (utilities::checkWithinCam(Pos, camX, camY)) {
				continue;
			}
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

			if (mapeditor == 1 && (int)(mouseX + camX) == (int)Pos.x && (int)(mouseY + camY) == (int)Pos.y) {
				AEGfxSetTintColor(1.0f, 0.0f, 0.0f, 0.8f);
			}

			AEGfxSetTransparency(1.0f);

			AEMtx33 Translate, Scale, Transform;
			AEMtx33Trans(&Translate, Pos.x, Pos.y);
			AEMtx33Scale(&Scale, (f32)SPRITE_SCALE, (f32)SPRITE_SCALE);
			AEMtx33Concat(&Transform, &Scale, &Translate);

			AEGfxTextureSet(Player->pObject->pTexture,
				TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH * MapObjInstList[i][j].x,
				TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT * MapObjInstList[i][j].y);

			AEGfxSetTransform(Transform.m);

			AEGfxMeshDraw(Player->pObject->pMesh, AE_GFX_MDM_TRIANGLES);

		}
	}

	// Spawn Static entities
	for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
	{
		staticObjInst* pInst = sStaticObjInstList + i;

		// skip non-active object and reference boxes
		if (pInst->flag != FLAG_ACTIVE)
			continue;
		if (pInst->pObject->type == TYPE_REF) {
			continue;
		}
		if (utilities::checkWithinCam(pInst->posCurr, camX, camY)) {
			continue;
		}
		// for any transparent textures
		if (pInst->pObject->type == TYPE_SLASH || pInst->pObject->type == TYPE_SPIKE) {
			AEGfxSetTransparency(1.0f - pInst->Alpha);
		}
		else {
			AEGfxSetTransparency(1.0f);
		}
		// For any types using spritesheet
		if (pInst->pObject->type != TYPE_SLASH)
		{
			AEGfxTextureSet(pInst->pObject->pTexture,
				pInst->TextureMap.x * TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH,
				pInst->TextureMap.y * TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT);
		}
		else {
			AEGfxTextureSet(pInst->pObject->pTexture, 0, 0);
		}
		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		AEGfxSetTransform(pInst->transform.m);
		// Draw the shape used by the current object instance using "AEGfxMeshDraw"
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}



	AEGfxSetTransparency(1.0f);



	// Spawn dynamic entities
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if (pInst->flag != FLAG_ACTIVE)
			continue;
		if (utilities::checkWithinCam(pInst->posCurr, camX, camY)) {

			continue;
		}
		// for any sprite textures
		if (pInst->pObject->type != TYPE_NUM) {
			AEGfxTextureSet(pInst->pObject->pTexture,
				pInst->TextureMap.x * TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH,
				pInst->TextureMap.y * TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT);
		}


		else {
			AEGfxTextureSet(pInst->pObject->pTexture, 0, 0);
		}
		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		AEGfxSetTransform(pInst->transform.m);
		// Draw the shape used by the current object instance using "AEGfxMeshDraw"
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}


	if (state == 1)
	{
		char debug[20] = "Debug Screen";
		char input[20] = "Input";
		char a[20] = "A";
		char w[20] = "W";
		char s[20] = "S";
		char d[20] = "D";
		char playerpos[100] = { (char)Player->posCurr.x,(char)Player->posCurr.y };
		char mouse_xy_buffer[50] = " "; // buffer
		char camxy_buffer[50] = " ";
		AEGfxPrint(1, debug, -0.99f, 0.90f, 1.5f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(1, input, -0.99f, 0.65f, 1.5f, 1.0f, 1.0f, 1.0f);

		sprintf_s(mouse_xy_buffer, "Mouse Position X: %.4f", mouseX + camX);
		AEGfxPrint(1, mouse_xy_buffer, -0.99f, 0.76f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(mouse_xy_buffer, "Mouse Position Y: %.4f", mouseY + camY);
		AEGfxPrint(1, mouse_xy_buffer, -0.99f, 0.71f, 1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(1, a, -0.99f, 0.55f, 1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(1, w, -0.99f, 0.60f, 1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(1, s, -0.99f, 0.50f, 1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(1, d, -0.99f, 0.45f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(playerpos, "Player Position X: %.4f", (Player->posCurr.x));
		AEGfxPrint(1, playerpos, -0.99f, 0.40f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(playerpos, "Player Position Y: %.4f", (Player->posCurr.y));
		AEGfxPrint(1, playerpos, -0.99f, 0.35f, 1.0f, 1.0f, 1.0f, 1.0f);

		sprintf_s(camxy_buffer, "Camera Position X: %.4f", camX);
		AEGfxPrint(1, camxy_buffer, -0.99f, 0.30f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(camxy_buffer, "Camera Position Y: %.4f", camY);
		AEGfxPrint(1, camxy_buffer, -0.99f, 0.25f, 1.0f, 1.0f, 1.0f, 1.0f);

		char Collision[2400] = " ";
		sprintf_s(Collision, "Collided: %d", binaryMap[((int(Player->posCurr.x)))][abs((int(Player->posCurr.y)))]);
		AEGfxPrint(1, Collision, -0.99f, 0.20f, 1.0f, 1.0f, 1.0f, 1.0f);




		if (AEInputCheckCurr(AEVK_W))
		{
			sprintf_s(w, "W Pressed");
			AEGfxPrint(1, w, -0.99f, 0.60f, 1.0f, 0.0f, 1.0f, 0.0f);
		}
		if (AEInputCheckCurr(AEVK_A))
		{
			sprintf_s(a, "A Pressed");
			AEGfxPrint(1, a, -0.99f, 0.55f, 1.0f, 0.0f, 1.0f, 0.0f);
		}
		if (AEInputCheckCurr(AEVK_S))
		{
			sprintf_s(s, "S Pressed");
			AEGfxPrint(1, s, -0.99f, 0.50f, 1.0f, 0.0f, 1.0f, 0.0f);
		}
		if (AEInputCheckCurr(AEVK_D))
		{
			sprintf_s(s, "D Pressed");
			AEGfxPrint(1, s, -0.99f, 0.45f, 1.0f, 0.0f, 1.0f, 0.0f);
		}
	}

	ParticleSystemDraw(&Player->transform);   //localtransform

}

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the World level.
*/
/******************************************************************************/
void GS_Tower_Free(void) {
	// kill all object instances in the array using "gameObjInstDestroy"
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;
		if (pInst) {
			gameObjInstDestroy(pInst);
		}
	}

	for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++) {
		staticObjInst* pInst = sStaticObjInstList + i;
		if (pInst) {
			staticObjInstDestroy(pInst);
		}
	}
	deletenodes();

	utilities::unloadObjs(Gates);
}

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	World level.
*/
/******************************************************************************/
void GS_Tower_Unload(void) {
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned int i = 0; i < sGameObjNum; i++) {
		if ((sGameObjList + i)->refMesh == false)
			AEGfxMeshFree((sGameObjList + i)->pMesh);
		if ((sGameObjList + i)->refTexture == false)
			AEGfxTextureUnload((sGameObjList + i)->pTexture);
	}

	//BUGGY CODE, IF UANBLE TO LOAD, CANNOT USE DEBUGGING MODE
	AEGfxSetCamPosition(0, 0);
}

// ---------------------------------------------------------------------------
