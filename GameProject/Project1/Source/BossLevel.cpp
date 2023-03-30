/******************************************************************************/
/*!
\file		Bosslevel.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 02, 2023
\brief		This header file contains the functions for the level of Boss Level.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Main.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <time.h>



/*!
	Defines
*/
/******************************************************************************/
static saveData				data;
static Node* nodes{};


static const unsigned int	MAX_MOBS = 11;						// The total number of mobs
static int					CURRENT_MOBS = MAX_MOBS;
static const unsigned int	MAX_CHESTS = 6;						// The total number of chests
static const unsigned int	MAX_LEVERS = 3;						// The total number of levers
static const unsigned int	MAX_KEYS;							// The total number of keys

static bool					SLASH_ACTIVATE = false;				// Bool to run slash animation

static const int			MAP_CELL_WIDTH = 24;				// Total number of cell widths
static const int			MAP_CELL_HEIGHT = 42;				// Total number of cell heights


static unsigned int			state = 0;							// Debugging state
static unsigned int			mapeditor = 0;						// Map edtior state

// -----------------------------------------------------------------------------

static AEVec2				MapObjInstList[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];	// 2D array of each map tile object
static int					binaryMap[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];	// 2D array of binary collision mapping



static float slashCD = 0;
static float walkCD = 0;

// pointer to the objects
static GameObjInst* Player;												// Pointer to the "Player" game object instance
static GameObjInst* Boss;
static staticObjInst* mapEditorObj;										// Pointer to the reference map editor object instance
static staticObjInst* Health[3];										// Pointer to the player health statc object instance
static staticObjInst* Levers[3];										// Pointer to each enemy object instance
static staticObjInst* MenuObj[3];										// Pointer to each enemy object instance
static staticObjInst* NumObj[3];
static staticObjInst* Chest[MAX_CHESTS];
static staticObjInst* Key;
static Inventory Backpack;
static staticObjInst* Spike;
static GameObj hpbar;

static AEGfxTexture* spriteSheet;
static AEGfxTexture* slashText;
static AEGfxTexture* refText;
static AEGfxVertexList* pMesh1;
static AEGfxVertexList* pMesh2;

static AEVec2* Gates;
static int gatesNum;
static int levNum;
static int chestNum;

static int levelclearedNum;

static std::vector<int> stageList;

static float spikedmgtimer = 0.f;
static float internalTimer = 0.f;

static float playerHitTime;
static staticObjInst* RefBox;

static AEMtx33 hpbartransform;

struct bosshp
{
	float maxhp;
	float currenthp;
	f32 width;
	f32 height;
	float damagetaken;
};
bosshp boss;

// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Boss Level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_BossLevel_Load(void) {
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

	

	GameObj* Character = 0, * Item = 0, * Map = 0, * Slash = 0,
		*RefLine = 0, *Health = 0, *Enemy = 0, *Boss = 0, *Key = 0,
		*Bullet = 0, *BossCircle = 0, *BossCircleAttack = 0;

	//Mesh for Sprite Sheet
	AEGfxMeshStart();

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFF00FF, 16.0f / TEXTURE_MAXWIDTH, 16.0f / TEXTURE_MAXHEIGHT,
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 16.0f / TEXTURE_MAXHEIGHT,
		0.5f, 0.5f, 0xFF00FFFF, 16.0f / TEXTURE_MAXWIDTH, 0.0f);

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 16.0f / TEXTURE_MAXHEIGHT,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 16.0f / TEXTURE_MAXWIDTH, 0.0f);

	pMesh1 = AEGfxMeshEnd();
	
	// Mesh for whole texture files
	AEGfxMeshStart();

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
		0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f);

	pMesh2 = AEGfxMeshEnd();

	//Load Textures
	slashText = AEGfxTextureLoad("Assets/slash.png");
	refText = AEGfxTextureLoad("Assets/Tilemap/RefBox.png");
	spriteSheet = AEGfxTextureLoad("Assets/Tilemap/tilemap_packed.png");

	// Load mesh and texture into game objects
	utilities::loadMeshNTexture(Character, pMesh1, spriteSheet, TYPE_CHARACTER);
	utilities::loadMeshNTexture(Item, pMesh1, spriteSheet, TYPE_ITEMS);
	utilities::loadMeshNTexture(Map, pMesh1, spriteSheet, TYPE_MAP);
	utilities::loadMeshNTexture(Slash, pMesh2, slashText, TYPE_SLASH);
	utilities::loadMeshNTexture(RefLine, pMesh2, slashText, TYPE_REF);
	utilities::loadMeshNTexture(Health, pMesh1, spriteSheet, TYPE_HEALTH);
	utilities::loadMeshNTexture(Enemy, pMesh1, spriteSheet, TYPE_ENEMY);
	utilities::loadMeshNTexture(Key, pMesh1, spriteSheet, TYPE_KEY);
	utilities::loadMeshNTexture(Bullet, pMesh1, spriteSheet, TYPE_BULLET);
	utilities::loadMeshNTexture(Boss, pMesh1, spriteSheet, TYPE_BOSS);
	utilities::loadMeshNTexture(BossCircle, pMesh1, spriteSheet, TYPE_BOSSCIRCLE);
	utilities::loadMeshNTexture(BossCircleAttack, pMesh1, spriteSheet, TYPE_BOSSCIRCLEATTACK);


	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x88880808, 0.0f, 0.0f,
		0.5f, -0.5f, 0x88880808, 0.0f, 0.0f,
		0.5f, 0.5f, 0x88880808, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, -0.5f, 0x88880808, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x88880808, 0.0f, 0.0f,
		0.5f, 0.5f, 0x88880808, 0.0f, 0.0f);
	hpbar.pMesh = AEGfxMeshEnd();

	ParticleSystemLoad();
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Boss Level state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_BossLevel_Init(void) {
	AEVec2* pos = nullptr;

	// =====================================
	//	Initialize map textures
	// =====================================

	utilities::importMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureBoss.txt");

	// =====================================
	//	Initialize map binary
	// =====================================

	utilities::importMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *binaryMap, "binaryBoss.txt");

	mapEditorObj = staticObjInstCreate(TYPE_MAP, 0, nullptr, 0);

	// =====================================
	//	Initialize objects for new game
	// =====================================
		//Initialise Player
		AEVec2 PlayerPos = { 4,-10 };
		Player = gameObjInstCreate(TYPE_CHARACTER, 1, &PlayerPos, 0, 0);

		Backpack.Potion = 0;
		Backpack.Key = 0;

		Player->health = 3;
		Player->damage = 1;

		//Initialise player health.
		for (int i = 0; i < Player->health; i++) {
			Health[i] = staticObjInstCreate(TYPE_HEALTH, 0.75, nullptr, 0);
		}


	//Init pathfinding nodes
	NodesInit(*binaryMap, MAP_CELL_WIDTH, MAP_CELL_HEIGHT);

	//init Boss
	AEVec2 BossPos = { 17,-10 }; // TXT
	Boss = gameObjInstCreate(TYPE_BOSS, 1, &BossPos, 0, 0);
	Boss->health = 3;
	Boss->pathfindtime = 0.25f;
	Boss->pathtimer = Boss->pathfindtime;

	// Initialise camera pos
	camX = 10, camY = -10;

	// =====================================
	//	Initialize UI objects
	// =====================================

	MenuObj[0] = staticObjInstCreate(TYPE_ITEMS, 1, nullptr, 0); // Potions
	MenuObj[1] = staticObjInstCreate(TYPE_KEY, 1, nullptr, 0); // Keys

	NumObj[0] = staticObjInstCreate(TYPE_ITEMS, 1, nullptr, 0); // Potions
	NumObj[1] = staticObjInstCreate(TYPE_KEY, 1, nullptr, 0); // Keys

	boss.maxhp = 100;
	boss.currenthp = 100;
	boss.width = SPRITE_SCALE * 9 * boss.currenthp/boss.maxhp;
	boss.height = SPRITE_SCALE * 0.4;


	ParticleSystemInit();

	playerHitTime = 0;
	stageList.clear();
	levelclearedNum = 0;
	if (levelCleared[0] == false) {
		stageList.push_back(0);
		levelclearedNum++;
	}
	if (levelCleared[1] == false) {
		stageList.push_back(1);
		levelclearedNum++;
	}
	if (levelCleared[2] == false) {
		stageList.push_back(2);
		levelclearedNum++;
	}
	
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Boss Level state.
*/
/******************************************************************************/
void GS_BossLevel_Update(void) {
	// Normalising mouse to 0,0 at the center
	s32 mouseIntX, mouseIntY;
	AEInputGetCursorPosition(&mouseIntX, &mouseIntY);
	mouseX = (float)(mouseIntX - AEGetWindowWidth() / 2) / SPRITE_SCALE;
	mouseY = (float)(-mouseIntY + AEGetWindowHeight() / 2) / SPRITE_SCALE;

	float angleMousetoPlayer = utilities::getAngle(Player->posCurr.x, Player->posCurr.y, mouseX + Player->posCurr.x, mouseY + Player->posCurr.y);
	if (mouseY + camY > Player->posCurr.y) {
		angleMousetoPlayer = -angleMousetoPlayer;
	}

	BossStateMachine(Boss);
	//Time-related variables
	utilities::decreaseTime(slashCD);
	utilities::decreaseTime(walkCD);
	utilities::decreaseTime(playerHitTime);
	Player->playerDamaged(playerHitTime);

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
		utilities::exportMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureBoss.txt");

		utilities::exportMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "binaryBoss.txt");
	}

	if (AEInputCheckTriggered(AEVK_M)) {
		gGameStateNext = GS_MAINMENU;
	}

	//simulating damage taken
	
	if (boss.currenthp > 0)
	{
		if (AEInputCheckTriggered(AEVK_Q))
		{
			boss.currenthp -= 10.f;
			boss.damagetaken = boss.maxhp - boss.currenthp;
			boss.width = SPRITE_SCALE * 9 * boss.currenthp / boss.maxhp;
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
			if (pInst->pObject->type == TYPE_BOSS) {
				pInst->velToPos(NPC_SPEED);
			}
		}
	}

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
		if (pInst->flag != 1 || (pInst->pObject->type != TYPE_SPIKE && pInst->pObject->type != TYPE_BOSSCIRCLEATTACK)) {
			continue;
		}

		if (pInst->pObject->type == TYPE_SPIKE) {
			pInst->spikeUpdate(); // Updates alpha of spikes

			if (Player->calculateDistance(*pInst) <= 1 && (pInst->Alpha == 0) && playerHitTime == 0) {

				Player->deducthealth();
				playerHitTime = DAMAGE_COODLDOWN_t;
			}
		}

		if (pInst->pObject->type == TYPE_BOSSCIRCLEATTACK) {
			if (Player->calculateDistance(*pInst) < 1.5f && playerHitTime == 0) {
				playerHitTime = DAMAGE_COODLDOWN_t;
				Player->deducthealth();
			}
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
		if (pInst->pObject->type == TYPE_BOSSCIRCLE || pInst->pObject->type == TYPE_BOSSCIRCLEATTACK) {
			pInst->timetracker += g_dt;
			if (pInst->timetracker > 0.5f) {
				staticObjInstDestroy(pInst);
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

	utilities::snapCamPos(Player->posCurr, camX, camY, MAP_CELL_WIDTH, 18);
	AEGfxSetCamPosition(static_cast<f32>(static_cast<int>(camX* (float)SPRITE_SCALE)), static_cast<f32>(static_cast<int> (camY* (float)SPRITE_SCALE)));

	MenuObj[0]->posCurr = { (float)camX - 9.0f, (float)camY + 5.0f };
	NumObj[0]->posCurr = { (float)camX - 8.0f, (float)camY + 5.0f };

	MenuObj[1]->posCurr = { (float)camX - 6.0f, (float)camY + 5.0f };
	NumObj[1]->posCurr = { (float)camX - 5.0f, (float)camY + 5.0f };

	//player health following viewport
	Health[0]->posCurr = { (float)camX + 7.0f , (float)camY + 5.0f };
	Health[1]->posCurr = { (float)camX + 8.0f , (float)camY + 5.0f };
	Health[2]->posCurr = { (float)camX + 9.0f , (float)camY + 5.0f };

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

	//scale, rot, trans for health bar
	AEMtx33 bar_scale, bar_trans, bar_rot;
	AEMtx33Scale(&bar_scale, boss.width, boss.height);
	AEMtx33Rot(&bar_rot, 0);
	AEMtx33Trans(&bar_trans, camX* SPRITE_SCALE + 80.f - (boss.damagetaken * SPRITE_SCALE * 4 / boss.maxhp), camY * SPRITE_SCALE + 350.f);
	AEMtx33Concat(&hpbartransform, &bar_scale, &bar_rot);
	AEMtx33Concat(&hpbartransform, &bar_trans, &hpbartransform);

	// Camera position and UI items

	NumObj[0]->TextureMap = TEXTURE_NUMBERS[Backpack.Potion];
	NumObj[1]->TextureMap = TEXTURE_NUMBERS[Backpack.Key];

	//Player->dustParticles();

	ParticleSystemUpdate();

}

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_BossLevel_Draw(void) {

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
		if (pInst->pObject->type == TYPE_SLASH || pInst->pObject->type == TYPE_BOSSCIRCLE) {
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
		AEGfxSetTintColor(pInst->damagetint.red, pInst->damagetint.green, pInst->damagetint.blue, 1.0f);
		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		AEGfxSetTransform(pInst->transform.m);
		// Draw the shape used by the current object instance using "AEGfxMeshDraw"
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}

	//drawing of health bar
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTintColor(0.7f, 0.7f, 0.7f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTransform(hpbartransform.m);
	AEGfxMeshDraw(hpbar.pMesh, AE_GFX_MDM_TRIANGLES);

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
	This function frees all the instances created for the Boss Level level.
*/
/******************************************************************************/
void GS_BossLevel_Free(void) {
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


	ParticleSystemFree();
}

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Boss Level level.
*/
/******************************************************************************/
void GS_BossLevel_Unload(void) {
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned int i = 0; i < sGameObjNum; i++) {
		if ((sGameObjList + i)->refMesh == false)
			AEGfxMeshFree((sGameObjList + i)->pMesh);
		if ((sGameObjList + i)->refTexture == false)
			AEGfxTextureUnload((sGameObjList + i)->pTexture);
	}

	AEGfxMeshFree(pMesh1);
	AEGfxMeshFree(pMesh2);
	AEGfxMeshFree(hpbar.pMesh);
	AEGfxTextureUnload(spriteSheet);
	AEGfxTextureUnload(slashText);
	AEGfxTextureUnload(refText);

	//BUGGY CODE, IF UANBLE TO LOAD, CANNOT USE DEBUGGING MODE
	AEGfxSetCamPosition(0, 0);


	ParticleSystemUnload();
}

/******************************************************************
function definition for boss finite state machine
*******************************************************************/
void BossStateMachine(GameObjInst* pInst)
{
	AEVec2 velDown = { 0,1 };
	AEVec2 velRight = { 0,1 };
	AEVec2 enemyPos1{ 7, -6.5 };
	AEVec2 enemyPos2{ 17, -13.5 };
	static AEVec2 playerPosition{ 0,0 };
	static int stage = 0;
	//states declared at GameObjs.h
	switch (pInst->state)
	{
	case STATE_PATROL:
		switch (pInst->innerState) { 
		case INNER_STATE_ON_ENTER: // INNER STATE ON ENTER OF PATROL
			std::cout << "entering state 0 " << std::endl;
			pInst->timetracker += g_dt;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			
			break;
		case INNER_STATE_ON_UPDATE: // INNER STATE UPDATE OF PATROL
			std::cout << "updating state 0" << std::endl;
			pInst->timetracker += g_dt;
			pInst->mobsPathFind(*Player);
			if (pInst->calculateDistance(*Player) < 1.5f) { // If found player, attack player
				pInst->innerState = INNER_STATE_ON_EXIT;
				pInst->stateFlag = STATE_BASIC;
				break;
			}

			if (static_cast<int>(pInst->timetracker * 10) % static_cast<int>(challengeATKREFRESH * 10) == 0) {
				//CHALLENGE ATTACK
				// random between 0, 1 and 2
				pInst->innerState = INNER_STATE_ON_EXIT;
				int random = rand() % 3;
				if (random == 0) {
					pInst->stateFlag = STATE_MAZE_DARKEN;
				}
				else if (random == 1) {
					pInst->stateFlag = STATE_SPAWN_BULLETS;
				}
				else if (random == 2) {
					pInst->stateFlag = STATE_SPAWN_ENEMIES;
				}
				break;
			}

			if (static_cast<int>(pInst->timetracker * 10) % static_cast<int>(aoeREFRESH*10) == 0) {
				//AOE ATTACK
				pInst->stateFlag = STATE_AOE;
				pInst->innerState = INNER_STATE_ON_EXIT; 
				
				break;
			}


			break;

		case INNER_STATE_ON_EXIT: // INNER STATE UPDATE OF PATROL
			std::cout << "exiting state 0" << std::endl;
			pInst->timetracker += g_dt;
			pInst->state = pInst->stateFlag;
			pInst->innerState = INNER_STATE_ON_ENTER;
			
			break;
		} break;


	case STATE_BASIC:
		switch (pInst->innerState) {
		case INNER_STATE_ON_ENTER: // INNER STATE ON ENTER OF BASIC
			pInst->velCurr = { 0,0 };
			std::cout << "entering state 1" << std::endl;
			pInst->timetracker += g_dt;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			playerPosition = Player->posCurr;

			break;
		case INNER_STATE_ON_UPDATE: // INNER STATE ON UPDATE OF BASIC
			std::cout << "updating state 1" << std::endl;
			pInst->timetracker += g_dt;

			//stand still for 1 second
			pInst->timeCD += g_dt;
			if (pInst->timeCD > 1.0f) {
				pInst->timeCD = 0;
				float angle = utilities::getAngle(pInst->posCurr.x, pInst->posCurr.y, playerPosition.x, playerPosition.y);
				AEVec2 slashPosition = { pInst->posCurr.x -cos(angle) / 1.3f, pInst->posCurr.y -sin(angle) / 1.3f };
				staticObjInst* bossSlash = staticObjInstCreate(TYPE_SLASH, 2, &slashPosition, PI + angle);
				if (Player->calculateDistance(*bossSlash) < 1.0f) {
					playerHitTime = DAMAGE_COODLDOWN_t;
					Player->deducthealth();
				}

				//Run slash command
				pInst->stateFlag = STATE_PATROL;
				pInst->innerState = INNER_STATE_ON_EXIT;

			}
			


			break;
		case INNER_STATE_ON_EXIT: // INNER STATE ON EXIT OF BASIC
			std::cout << "exiting state 1" << std::endl;
			pInst->timetracker += g_dt;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->state = pInst->stateFlag;

			break;
		}break;


	case STATE_AOE:
		switch (pInst->innerState) {
		case INNER_STATE_ON_ENTER:
			pInst->velCurr = { 0,0 };
			std::cout << "entering state 2" << std::endl;
			playerPosition = Player->posCurr;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			break;
		case INNER_STATE_ON_UPDATE:
			std::cout << "updating state 2" << std::endl;
			//AOE Attack
			pInst->timeCD += g_dt;
			switch (stage) {
			case 0:

					staticObjInstCreate(TYPE_BOSSCIRCLE, 3, &playerPosition, 0);
					//Draw Circle
					pInst->timeCD = 0;
					stage = 1;
					break;
			case 1:
				if (pInst->timeCD > 1.0f) {
					//Draw circle
					staticObjInstCreate(TYPE_BOSSCIRCLE, 3, &playerPosition, 0);
					pInst->timeCD = 0;
					stage = 2;
				} break;
			case 2:
				if (pInst->timeCD > 1.0f) {
					//Draw circle
					staticObjInstCreate(TYPE_BOSSCIRCLE, 3, &playerPosition, 0);
					pInst->timeCD = 0;
					stage = 3;
				} break;
			case 3:
				if (pInst->timeCD > 3.0f) {
					//Damage player
					staticObjInstCreate(TYPE_BOSSCIRCLEATTACK, 3, &playerPosition, 0);
					pInst->timeCD = 0;
					stage = 0;
					pInst->stateFlag = STATE_PATROL;
					pInst->innerState = INNER_STATE_ON_EXIT;
				}
				break;
			}
			break;
		case INNER_STATE_ON_EXIT:
			std::cout << "exiting state 2" << std::endl;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->state = pInst->stateFlag;

			break;
		} break;


	case STATE_SPAWN_ENEMIES:
		switch (pInst->innerState) {
		case INNER_STATE_ON_ENTER:
			pInst->velCurr = { 0,0 };
			std::cout << "entering state 3" << std::endl;
			//stand still for 1 second
			pInst->timeCD += g_dt;

			if (pInst->timeCD > 1.0f) {
				pInst->innerState = INNER_STATE_ON_UPDATE;
				pInst->timeCD = 0;
			}
			break;
		case INNER_STATE_ON_UPDATE:

			std::cout << "updating state 3" << std::endl;
			//Spawn enemies
			// Stay still for awhile
			pInst->timeCD += g_dt;
			if (pInst->timeCD > 2.0f) {
				pInst->timeCD = 0;
				GameObjInst* first = gameObjInstCreate(TYPE_ENEMY, 1, &enemyPos1, nullptr, 0);
				GameObjInst* second = gameObjInstCreate(TYPE_ENEMY, 1, &enemyPos2, nullptr, 0);
				first->health = 3;
				first->pathfindtime = 0.25f;
				first->pathtimer = first->pathfindtime;
				second->health = 3;
				second->pathfindtime = 0.25f;
				second->pathtimer = second->pathfindtime;
				pInst->stateFlag = STATE_PATROL;
				pInst->innerState = INNER_STATE_ON_EXIT;
			}
			break;
		case INNER_STATE_ON_EXIT:
			std::cout << "exiting state 3" << std::endl;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->state = pInst->stateFlag;
			break;
		} break;

	case STATE_SPAWN_BULLETS:
		switch (pInst->innerState) {
		case INNER_STATE_ON_ENTER:
			pInst->velCurr = { 0,0 };
			std::cout << "entering state 4" << std::endl;
			//stand still for 1 second
			pInst->timeCD += g_dt;

			if (pInst->timeCD > 1.0f) {
				pInst->innerState = INNER_STATE_ON_UPDATE;
				pInst->timeCD = 0;
			}
			break;
		case INNER_STATE_ON_UPDATE:

			std::cout << "updating state 4" << std::endl;
			//Spawn Bullets

			// Stay still for awhile
			pInst->timeCD += g_dt;
			if (pInst->timeCD > 2.0f) {
				pInst->timeCD = 0;
				pInst->stateFlag = STATE_PATROL;
				pInst->innerState = INNER_STATE_ON_EXIT;
			}
			break;
		case INNER_STATE_ON_EXIT:
			std::cout << "exiting state 4" << std::endl;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->state = pInst->stateFlag;
			break;
		}break;

	case STATE_MAZE_DARKEN:
		switch (pInst->innerState) {
		case INNER_STATE_ON_ENTER:
			pInst->velCurr = { 0,0 };
			std::cout << "entering state 5" << std::endl;
			//stand still for 1 second
			pInst->timeCD += g_dt;

			if (pInst->timeCD > 1.0f) {
				pInst->innerState = INNER_STATE_ON_UPDATE;
				pInst->timeCD = 0;
			}
			break;
		case INNER_STATE_ON_UPDATE:

			std::cout << "updating state 5" << std::endl;
			//Spawn darken

			// Stay still for awhile
			pInst->timeCD += g_dt;
			if (pInst->timeCD > 2.0f) {
				pInst->timeCD = 0;
				pInst->stateFlag = STATE_PATROL;
				pInst->innerState = INNER_STATE_ON_EXIT;
			}
			break;
		case INNER_STATE_ON_EXIT:
			std::cout << "exiting state 5" << std::endl;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->state = pInst->stateFlag;
			break;
		} break;




		/*this will be the last 4 states
		 pinst->state = (srand(3,6));*/





	}
}

// ---------------------------------------------------------------------------



