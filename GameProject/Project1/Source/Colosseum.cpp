/******************************************************************************/
/*!
\file		Colosseum.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 02, 2023
\brief		This header file contains the functions for the level of Colosseum.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Main.h"
#include <fstream>
#include <iostream>
#include "Globals.h"
#include "Enemy.h"
#include "collision.h"


/*!
	Defines
*/
/******************************************************************************/
static saveData				data;
static Node* nodes{};
static const unsigned int	GAME_OBJ_NUM_MAX = 32;				// The total number of unique objects (Shapes)
static const unsigned int	TEXTURE_NUM_MAX = 32;				// The total number of Textures
static const unsigned int	GAME_OBJ_INST_NUM_MAX = 2048;		// The total number of dynamic game object instances
static const unsigned int	FONT_NUM_MAX = 10;					// The total number of fonts
static const unsigned int	STATIC_OBJ_INST_NUM_MAX = 1024;	// The total number of static game object instances


//static const unsigned int	MAX_CHESTS;							// The total number of chests
static const unsigned int	MAX_LEVERS = 3;						// The total number of levers
static const unsigned int	MAX_MOBS = 11;							// The total number of mobs
static unsigned int			CURRENT_MOBS = MAX_MOBS;

static const unsigned int	MAX_CHESTS = 1;						// The total number of chests

static bool					SLASH_ACTIVATE = false;				// Bool to run slash animation

static const int			MAP_CELL_WIDTH = 28;				// Total number of cell widths
static const int			MAP_CELL_HEIGHT = 29;				// Total number of cell heights

static const float MAX_ENEMY_DISTANCE = 1.0f;
static unsigned int			state = 0;							// Debugging state
static unsigned int			mapeditor = 0;						// Map edtior state

static						AEVec2 binaryPlayerPos;				// Position on Binary Map
// -----------------------------------------------------------------------------
//static Node* nodes{};

// -----------------------------------------------------------------------------
// object flag definition

static const unsigned long FLAG_ACTIVE = 0x00000001;			// For whether object instance is active

/******************************************************************************/
/*!
	Struct/Class Definitions
*/
/******************************************************************************/

// ---------------------------------------------------------------------------

// list of original object
static GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sGameObjNum;								// The number of defined game objects

// list of object instances
static GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a dynamic unique game object instance (sprite)
static unsigned long		sGameObjInstNum;							// The number of used dynamic game object instances

// list of static instances
static staticObjInst		sStaticObjInstList[STATIC_OBJ_INST_NUM_MAX];// Each element in this array represents a unique static game object instance (sprite)
static unsigned long		sStaticObjInstNum;							// The number of used static game object instances

static AEVec2				MapObjInstList[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];	// 2D array of each map tile object
static int					binaryMap[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];	// 2D array of binary collision mapping

static s8					FontList[FONT_NUM_MAX];						// Each element in this array represents a Font
static unsigned long		FontListNum;								// The number of used fonts

// pointer to the objects
static GameObjInst* Player;												// Pointer to the "Player" game object instance
static staticObjInst* mapEditorObj;										// Pointer to the reference map editor object instance
static staticObjInst* Health[3];										// Pointer to the player health statc object instance
static GameObjInst* enemy[2];
static staticObjInst* RefBox;
static staticObjInst* Chest[MAX_CHESTS];



float Timer = 0.f;

int waves = 0;
float wavestimer = 0.f;

// ---------------------------------------------------------------------------

/******************************************************************************/


//int CheckInstanceBinaryMapCollision(float PosX, float PosY,
//	float scaleX, float scaleY);


/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Colosseum level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_Colosseum_Load(void) {
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

	ParticleSystemLoad();
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Colosseum state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_Colosseum_Init(void) {
	//Initialise Player
	AEVec2 PlayerPos = { 14,-16 };
	Player = gameObjInstCreate(TYPE_CHARACTER, 1, &PlayerPos, 0, 0);
	Player->TextureMap = { 1,8 };

	//Initialise map textures
	std::ifstream mapInput{ "Assets/textureColosseum.txt" };
	//std::ifstream mapInput{ "../Assets/map1.txt" };
	for (int j = 0; j < MAP_CELL_HEIGHT; j++) {
		for (int i = 0; i < MAP_CELL_WIDTH; i++) {
			mapInput >> MapObjInstList[i][j].x;
			mapInput >> MapObjInstList[i][j].y;
		}
	}
	mapInput.close();


	// =====================================
	//	Initialize map binary
	// =====================================

	//utilities::importMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *binaryMap, "binaryWorld.txt");
	std::ifstream binInput{ "Assets/binaryColosseum.txt" };
	for (int i = 0; i < MAP_CELL_HEIGHT; i++) {
		for (int j = 0; j < MAP_CELL_WIDTH; j++) {
			binInput >> binaryMap[j][i];
		}
	}
	binInput.close();

	RefBox = staticObjInstCreate(TYPE_REF, 1, nullptr, 0);

	AEVec2 Pos = { 9.f , 3.f };
	mapEditorObj = staticObjInstCreate(TYPE_MAP, 0, &Pos, 0);

	//Initialise player health. Printing of hearts.
	Player->health = 3;
	for (int i = 0; i < Player->health; i++) {
		Health[i] = staticObjInstCreate(TYPE_HEALTH, 0.75, nullptr, 0);
		Health[i]->TextureMap = { 0,11 };
	}

	AEVec2 pos[3] = { {17.5f - (1.0f / 16),-13} ,{ 66.5f - (1.0f / 16), -11 } ,{ 43.5f - (1.0f / 16), -6} };

	// Initialise camera pos
	camX = Player->posCurr.x, camY = Player->posCurr.y;
	AEVec2 EnemyPos[2] = { {33.f, -40.f} ,{33.f, -45.f} };

	//Initialise enemy in level
	for (int i = 0; i < 2; i++) {
		enemy[i] = gameObjInstCreate(TYPE_ENEMY, 1, &EnemyPos[i], 0, 0);
		enemy[i]->TextureMap = { 0,9 };
	}


	//Initialise chest in level
	AEVec2 chestpos[1] = { {14,-12} };
	for (int i = 0; i < MAX_CHESTS; i++)
	{
		Chest[i] = staticObjInstCreate(TYPE_CHEST, 1, &chestpos[i], 0);
		Chest[i]->TextureMap = { 5, 7 };
	}

	//binaryMap[(int)(Player->posCurr.x+20)][(int)(Player->posCurr.y-58)] = test++;
	//{ 12,-31 };
	binaryPlayerPos = { 32,-89 };
	ParticleSystemInit();

	int* coloptr = *binaryMap;
	NodesInit(coloptr, MAP_CELL_WIDTH, MAP_CELL_HEIGHT);
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Colosseum state.
*/
/******************************************************************************/

void GS_Colosseum_Update(void) {

	// =====================================
	// User Input
	// =====================================
	//Debugging mode
	if (AEInputCheckTriggered(AEVK_F3)) {
		state ^= 1;
	}

	if (AEInputCheckTriggered(AEVK_9)) {
		mapeditor ^= 1;
	}

	/*if (AEInputCheckTriggered(AEVK_EQUAL))
	{

		AEVec2 Enemypos[2] = { {14.f, -16.f} ,{20.f, -16.f} };
		for (int i = 0; i < 2; i++) {
			GameObjInst* enemy = gameObjInstCreate(TYPE_ENEMY, 1, &Enemypos[i], 0, 0);
			enemy->TextureMap = { 0,9 };
			enemy->health = 3;
		}
	}*/
	Player->velCurr = { 0,0 };// set velocity to 0 initially, if key is released, velocity is set back to 0

	if (AEInputCheckCurr(AEVK_W) || AEInputCheckCurr(AEVK_UP)) // movement for W key 
	{
		Player->velCurr.y = 1;// this is direction , positive y direction
	}
	if (AEInputCheckCurr(AEVK_S) || AEInputCheckCurr(AEVK_DOWN))
	{
		Player->velCurr.y = -1;// this is direction , negative y direction
	}
	if (AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT))
	{
		Player->velCurr.x = -1;// this is direction , negative x direction
		Player->scale = -1;
	}
	if (AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT))
	{
		Player->velCurr.x = 1;// this is direction , positive x direction
		Player->scale = 1;
	}

	// Normalising mouse to 0,0 at the center
	s32 mouseIntX, mouseIntY;
	AEInputGetCursorPosition(&mouseIntX, &mouseIntY);
	mouseX = (float)(mouseIntX - AEGetWindowWidth() / 2) / SPRITE_SCALE;
	mouseY = (float)(-mouseIntY + AEGetWindowHeight() / 2) / SPRITE_SCALE;

	float angleMousetoPlayer = utilities::getAngle(Player->posCurr.x, Player->posCurr.y, mouseX + Player->posCurr.x, mouseY + Player->posCurr.y);

	if (mouseY + camY > Player->posCurr.y) {
		angleMousetoPlayer = -angleMousetoPlayer;
	}

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		SLASH_ACTIVATE = true;
	}

	if (mapeditor == 1) {
		mapEditorObj->scale = 0.7f;
		mapEditorObj->posCurr = { mouseX + camX + 0.3f, mouseY + camY + 0.3f };
		if (AEInputCheckTriggered(AEVK_K) && mapEditorObj->TextureMap.y < TEXTURE_MAXHEIGHT / TEXTURE_CELLSIZE) {
			mapEditorObj->TextureMap.y += 1;
		}
		if (AEInputCheckTriggered(AEVK_I) && mapEditorObj->TextureMap.y > 0) {
			mapEditorObj->TextureMap.y -= 1;
		}
		if (AEInputCheckTriggered(AEVK_J) && mapEditorObj->TextureMap.x > 0) {
			mapEditorObj->TextureMap.x -= 1;
		}
		if (AEInputCheckTriggered(AEVK_L) && mapEditorObj->TextureMap.x < TEXTURE_MAXWIDTH / TEXTURE_CELLSIZE) {
			mapEditorObj->TextureMap.x += 1;
		}
		for (int j = 0; j < MAP_CELL_HEIGHT; j++) {
			for (int i = 0; i < MAP_CELL_WIDTH; i++) {
				if (mouseX + camX >= i &&
					mouseX + camX <= i + 1 &&
					-mouseY - camY >= j &&
					-mouseY - camY <= j + 1
					&& AEInputCheckCurr(AEVK_LBUTTON)) {
					MapObjInstList[i][j] = mapEditorObj->TextureMap;
				}
			}
		}
	}
	else {
		mapEditorObj->scale = 0;
	}

	//Map editor printing
	if (AEInputCheckTriggered(AEVK_8)) {
		utilities::exportMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureColosseum.txt");

		utilities::exportMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "binaryColosseum.txt");
	}

	if (AEInputCheckTriggered(AEVK_7)) {
		//utilities::importMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *binaryMap, "binaryColosseum.txt");
		std::ifstream binInput{ "Assets/binaryColosseum.txt" };
		for (int i = 0; i < MAP_CELL_HEIGHT; i++) {
			for (int j = 0; j < MAP_CELL_WIDTH; j++) {
				binInput >> binaryMap[j][i];
			}
		}
		binInput.close();
	}

	// Pathfinding for Enemy AI
	for (int j = 0; j < GAME_OBJ_INST_NUM_MAX; j++)
	{
		GameObjInst* pEnemy = sGameObjInstList + j;

		// skip non-active object
		if (pEnemy->flag != FLAG_ACTIVE || pEnemy->pObject->type != TYPE_ENEMY)
			continue;

		if (Player->calculateDistance(*pEnemy) > 10)
			continue;

		// perform pathfinding for this enemy
		pEnemy->path = pathfind(pEnemy->posCurr.x, pEnemy->posCurr.y, Player->posCurr.x, Player->posCurr.y);

		// update enemy velocity based on path
		if (pEnemy->path.size() > 1)
		{
			Node* pNextNode = pEnemy->path[1];

			// calculate the distance between the enemy and player
			float distance = sqrtf(powf(Player->posCurr.x - pEnemy->posCurr.x, 2) + powf(Player->posCurr.y - pEnemy->posCurr.y, 2));

			// update enemy velocity only if it is farther than the maximum distance
			if (distance > MAX_ENEMY_DISTANCE)
			{
				// check if player is moving or the enemy is already stopped
				if (Player->velCurr.x != 0 || Player->velCurr.y != 0 || pEnemy->stopped)
				{
					// continue moving
					pEnemy->velCurr.x -= (g_dt * 5.0f * (pNextNode->parent->ae_NodePos.x - pNextNode->ae_NodePos.x));
					pEnemy->velCurr.y -= (g_dt * 5.0f * (pNextNode->parent->ae_NodePos.y - pNextNode->ae_NodePos.y));

					// set flag to indicate not stopped
					pEnemy->stopped = false;
				}
				else // player is not moving and enemy is not stopped
				{
					// stop moving
					pEnemy->velCurr.x = 0;
					pEnemy->velCurr.y = 0;

					// set flag to indicate stopped
					pEnemy->stopped = true;
				}
			}
			else
			{
				// stop moving if already close to the player
				pEnemy->velCurr.x = 0;
				pEnemy->velCurr.y = 0;

				// set flag to indicate stopped
				pEnemy->stopped = true;
			}
		}
	}

	// ======================================================
	// update physics of all active game object instances
	//  -- Get the AABB bounding rectangle of every active instance:
	//		boundingRect_min = -(BOUNDING_RECT_SIZE/2.0f) * instance->scale + instance->pos
	//		boundingRect_max = +(BOUNDING_RECT_SIZE/2.0f) * instance->scale + instance->pos
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;
		if (pInst->flag != FLAG_ACTIVE) {
			continue;
		}
		pInst->boundingBox.min.x = -(BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.x;
		pInst->boundingBox.min.y = -(BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.y;
		pInst->boundingBox.max.x = (BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.x;
		pInst->boundingBox.max.y = (BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.y;
	}

	for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++) {
		staticObjInst* pInst = sStaticObjInstList + i;
		if (pInst->flag != FLAG_ACTIVE) {
			continue;
		}
		if (pInst->pObject->type != TYPE_SLASH) {
			continue;
		}
		pInst->boundingBox.min.x = -(BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.x;
		pInst->boundingBox.min.y = -(BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.y;
		pInst->boundingBox.max.x = (BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.x;
		pInst->boundingBox.max.y = (BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.y;
	}

	// ======================================================
	//	-- Positions of the instances are updated here with the already computed velocity (above)
	// ======================================================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;
		if (pInst->velCurr.x != 0 || pInst->velCurr.y != 0) //if player direction is not 0, as you cannot normalize 0.
		{
			AEVec2 temp_velo{ pInst->velCurr.x, pInst->velCurr.y };
			AEVec2Normalize(&pInst->velCurr, &temp_velo); // normalize

			if (pInst->pObject->type == TYPE_CHARACTER) {
				pInst->velCurr.x *= PLAYER_SPEED; // magnitude/speed of velo.x
				pInst->velCurr.y *= PLAYER_SPEED; // magnitude/speed of velo.y
			}
			//invert movement for binary map
			if (pInst->velCurr.x != 0)
			{
				binaryPlayerPos.y += pInst->velCurr.x * g_dt;
			}
			if (pInst->velCurr.y != 0)
			{
				binaryPlayerPos.x -= pInst->velCurr.y * g_dt;
			}
			if (pInst->pObject->type == TYPE_NPCS) {
				pInst->velCurr.x *= NPC_SPEED; // magnitude/speed of velo.x
				pInst->velCurr.y *= NPC_SPEED; // magnitude/speed of velo.y
			}
		}

		pInst->posCurr.x += pInst->velCurr.x * g_dt;
		pInst->posCurr.y += pInst->velCurr.y * g_dt;
	}

	// Camera position, stops following character when at edge of Colosseum
	if (MAP_CELL_WIDTH - CAM_CELL_WIDTH / 2 - 0.5 > Player->posCurr.x &&
		CAM_CELL_WIDTH / 2 + 0.5 < Player->posCurr.x) {
		camX = Player->posCurr.x;
	}
	if (MAP_CELL_HEIGHT - CAM_CELL_HEIGHT / 2 - 0.5 > -Player->posCurr.y &&
		CAM_CELL_HEIGHT / 2 + 0.5 < -Player->posCurr.y) {
		camY = Player->posCurr.y;
	}
	//player health following viewport
	Health[0]->posCurr = { (float)camX + 7.0f , (float)camY + 5.0f };
	Health[1]->posCurr = { (float)camX + 8.0f , (float)camY + 5.0f };
	Health[2]->posCurr = { (float)camX + 9.0f , (float)camY + 5.0f };

	if (SLASH_ACTIVATE == true) {
		AEVec2 Pos = Player->posCurr;
		Pos.x += Player->velCurr.x * 0.25f - cos(angleMousetoPlayer) / 1.3f;
		Pos.y += Player->velCurr.y * 0.25f - sin(angleMousetoPlayer) / 1.3f;
		staticObjInst* slashObj = staticObjInstCreate(TYPE_SLASH, 1, &Pos, 0);
		slashObj->dirCurr = angleMousetoPlayer + PI;
		slashObj->timetracker = 0;
		SLASH_ACTIVATE = false;
	}


	// ====================
	// check for collision
	// ====================

	//if pickup potion then add player health
	if (AEInputCheckTriggered(AEVK_R))
	{
		Player->recoverhealth();
		switch (Player->health)
		{
		case 2:
			Health[1]->TextureMap = { 0, 11 };
			break;
		case 3:
			Health[0]->TextureMap = { 0, 11 };
			break;
		}
	}

	//if player receive damage from collision or from mob, player decrease health
	if (AEInputCheckTriggered(AEVK_T))
	{
		Player->deducthealth();
		switch (Player->health)
		{
		case 0:
			Health[2]->TextureMap = { 1, 11 };
			break;
		case 1:
			Health[1]->TextureMap = { 1, 11 };
			break;
		case 2:
			Health[0]->TextureMap = { 1, 11 };
		}
	}
	static float playerHitTime = 0;
	playerHitTime -= g_dt;
	if (playerHitTime < 0) {
		playerHitTime = 0;
	}

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

					// Hit cooldown
					playerHitTime = 0.5f;

					//knockback
					AEVec2 nil{ 0,0 };
					if (Player->velCurr == nil)
						Player->posCurr += pInst->velCurr / 4;
					else Player->posCurr -= Player->velCurr / 4;

				}
			}

			for (int j = 0; j < STATIC_OBJ_INST_NUM_MAX; j++) {
				staticObjInst* jInst = sStaticObjInstList + j;
				if (jInst->flag != FLAG_ACTIVE || jInst->pObject->type != TYPE_SLASH) {
					continue;
				}
				AEVec2 velNull = { 0,0 };
				if (pInst->calculateDistance(*jInst) < 0.6f
					&& jInst->Alpha == 1) {
					pInst->deducthealth(Player->damage);
					// Knockback
					printf("dmg mobs");
					AEVec2 slash2Mob = jInst->posCurr - pInst->posCurr;
					pInst->posCurr -= slash2Mob;
				}
			}
		}

		if (pInst->pObject->type == TYPE_BULLET) {
			int flag = CheckInstanceBinaryMapCollisionCollo(pInst->posCurr.x, -pInst->posCurr.y, pInst->scale, pInst->scale, binaryMap);
			if (CollisionIntersection_RectRect(Player->boundingBox, Player->velCurr, pInst->boundingBox, pInst->velCurr)) {
				Player->deducthealth();
				gameObjInstDestroy(pInst);
			}
			if (flag & COLLISION_TOP) {
				gameObjInstDestroy(pInst);
			}
			if (flag & COLLISION_BOTTOM) {
				gameObjInstDestroy(pInst);
			}
			if (flag & COLLISION_RIGHT) {
				gameObjInstDestroy(pInst);
			}
			if (flag & COLLISION_LEFT) {
				gameObjInstDestroy(pInst);
			}
		}

		switch (Player->health)
		{
		case 0:
			Health[2]->TextureMap = { 1, 11 };
			break;
		case 1:
			Health[1]->TextureMap = { 1, 11 };
			break;
		case 2:
			Health[0]->TextureMap = { 1, 11 };
		}
	}



	// ===================================
	// update active game object instances
	// Example:
	//		-- Removing effects after certain time
	// ===================================
	for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
	{
		staticObjInst* pInst = sStaticObjInstList + i;
		if (pInst->flag != FLAG_ACTIVE) {
			continue;
		}

		if (pInst->pObject->type == TYPE_SLASH) {
			pInst->timetracker += g_dt;
			if (pInst->timetracker >= 0.2) {
				pInst->Alpha = (pInst->timetracker - 0.2f) / 0.4f;
			}
		}

		if (pInst->timetracker >= 0.6) {
			staticObjInstDestroy(pInst);
		}

	}
	/*if (CheckInstanceBinaryMapCollision(Player->posCurr.x, Player->posCurr.y,
		SPRITE_SCALE, SPRITE_SCALE) == 1)
	{
		Player->posCurr.x = Player->posCurr.x;
		Player->posCurr.y = Player->posCurr.y;
		std::cout << "collided" << std::endl;
	}*/

	for (int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;
		if (pInst->flag != FLAG_ACTIVE) {
			continue;
		}

		if (pInst->pObject->type == TYPE_ENEMY)
		{
			if (pInst->health == 0)
			{
				gameObjInstDestroy(pInst);
				CURRENT_MOBS -= 1;
				//			//randomising potion drop rate when mobs are killed 
				//			srand(time(NULL));
				//			if (rand() % 2 == 0)
				//			{
				//				AEVec2 Pos = { pInst->posCurr.x, pInst->posCurr.y };
				//				staticObjInst* Potion = staticObjInstCreate(TYPE_ITEMS, 1, &Pos, 0);
				//				Potion->TextureMap = { 6,9 };
			}
		}
		//	}

		if (pInst->pObject->type == TYPE_CHARACTER) {
			pInst->timetracker += g_dt;
			if (pInst->health == 0) {
				gGameStateNext = GS_DEATHSCREEN;
			}
		}
	}
	// =====================================
	// calculate the matrix for all objects
	// =====================================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		AEMtx33		 trans = { 0 }, rot = { 0 }, scale = { 0 };


		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		float scaleY;
		// Compute the scaling matrix
		if (pInst->scale < 0) {
			scaleY = -pInst->scale;
		}
		else {
			scaleY = pInst->scale;
		}
		AEMtx33Scale(&scale, pInst->scale * SPRITE_SCALE, scaleY * SPRITE_SCALE);
		// Compute the rotation matrix 
		AEMtx33Rot(&rot, pInst->dirCurr);
		// Compute the translation matrix
		AEMtx33Trans(&trans, pInst->posCurr.x * SPRITE_SCALE, pInst->posCurr.y * SPRITE_SCALE);
		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
		AEMtx33Concat(&pInst->transform, &rot, &scale);
		AEMtx33Concat(&pInst->transform, &trans, &pInst->transform);

	}

	for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
	{
		staticObjInst* pInst = sStaticObjInstList + i;
		AEMtx33		 trans = { 0 }, rot = { 0 }, scale = { 0 };

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		AEMtx33Scale(&scale, pInst->scale * SPRITE_SCALE, pInst->scale * SPRITE_SCALE);
		// Compute the rotation matrix 
		AEMtx33Rot(&rot, pInst->dirCurr);
		// Compute the translation matrix
		AEMtx33Trans(&trans, pInst->posCurr.x * SPRITE_SCALE, pInst->posCurr.y * SPRITE_SCALE);
		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
		AEMtx33Concat(&pInst->transform, &rot, &scale);
		AEMtx33Concat(&pInst->transform, &trans, &pInst->transform);
	}

	AEGfxSetCamPosition(camX * SPRITE_SCALE, camY * SPRITE_SCALE);

	//CheckInstanceBinaryMapCollision(binaryPlayerPos.x, binaryPlayerPos.y, 1.0f, 1.0f);

	if (AEInputCheckTriggered(AEVK_F)) {
		static int test = 2;
		std::ofstream testfile{ "test.txt" };
		binaryMap[(int)binaryPlayerPos.x][(int)binaryPlayerPos.y] = test++;
		for (int i = 0; i < 42; i++) {
			for (int j = 0; j < 124; j++) {
				testfile << binaryMap[j][i];
			}
			testfile << std::endl;
		}
		testfile.close();
	}
	for (int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;
		AEVec2 reverse;
		if (pInst->pObject->type == TYPE_CHARACTER) {
			AEVec2Neg(&reverse, &pInst->velCurr);
			Timer += g_dt;
			if (Timer > 0.25f)
			{
				AEVec2 particlecoords = pInst->posCurr;
				particlecoords.y = pInst->posCurr.y - 0.48;
				Timer -= 0.25f;
				ParticleSystemRequest(0, 10.6f, &particlecoords,
					&reverse, 1.0f, 0.15f, 10);
			}
			else
			{
				Timer += g_dt;
			}
		}
		break;

	}


	if (AEInputCheckTriggered(AEVK_E)) {
		for (int i = 0; i < 1; i++)
		{
			//Interaction with Chest
			if (Player->calculateDistance(*Chest[i]) < 1 && Chest[i]->TextureMap.x != 8)
			{
				//change texture of chest
				Chest[i]->TextureMap = { 8, 7 };
				AEVec2 Pos = { Chest[i]->posCurr.x, Chest[i]->posCurr.y };
				staticObjInst* Potion = staticObjInstCreate(TYPE_ITEMS, 1, &Pos, 0);
				Potion->TextureMap = { 6,9 };
				waves = 1;
			}
		}
	}

	int flag = CheckInstanceBinaryMapCollisionCollo(Player->posCurr.x, -Player->posCurr.y, 1.0f, 1.0f, binaryMap);

	if (flag & COLLISION_TOP) {
		//Top collision
		std::cout << "collide top" << std::endl;
		snaptocellsub(&Player->posCurr.y);

		std::cout << Player->posCurr.y << std::endl;
		//Player->posCurr.y + 0.5;
	}

	if (flag & COLLISION_BOTTOM) {
		//bottom collision
		std::cout << "collide botton" << std::endl;
		snaptocellsub(&Player->posCurr.y);

		//Player->posCurr.y - 0.5;
	}

	if (flag & COLLISION_LEFT) {
		//Left collision
		std::cout << "collide left" << std::endl;
		snaptocelladd(&Player->posCurr.x);

		//Player->posCurr.x + 0.5;

	}
	if (flag & COLLISION_RIGHT) {
		//Right collision
		std::cout << "collide right" << std::endl;
		snaptocelladd(&Player->posCurr.x);

		//Player->posCurr.x - 0.5;
	}


	//WAVES///////////////////
	if (waves >= 1) {
		wavestimer += g_dt;
	}
	if (wavestimer >= 5.f) {
		if (waves == 15) {
			waves = 2;
		}
	}

	if (wavestimer >= 10.f) {
		if (waves == 17) {
			waves = 3;
		}
	}

	if (wavestimer >= 15.f) {
		waves = 99;
	}

	if (waves == 1) {
		//Initialise enemy in level
		AEVec2 Enemypos[2] = { {14.f, -14.f} ,{20.f, -14.f} };
		for (int i = 0; i < 2; i++) {
			GameObjInst* enemy = gameObjInstCreate(TYPE_ENEMY, 1, &Enemypos[i], 0, 0);
			enemy->TextureMap = { 0,9 };
			enemy->health = 3;
		}
		waves = 15;
	}

	if (waves == 2) {
		//Initialise enemy in level
		AEVec2 Enemypos[4] = { {14.f, -14.f} ,{20.f, -14.f}, {14.f, -17.f} ,{20.f, -17.f} };
		for (int i = 0; i < 4; i++) {
			GameObjInst* enemy = gameObjInstCreate(TYPE_ENEMY, 1, &Enemypos[i], 0, 0);
			enemy->TextureMap = { 0,9 };
			enemy->health = 3;
		}

		waves = 17;
	}

	if (waves == 3) {
		//Initialise enemy in level
		AEVec2 Enemypos[2] = { {14.f, -16.f} ,{20.f, -16.f} };
		for (int i = 0; i < 2; i++) {
			GameObjInst* enemy = gameObjInstCreate(TYPE_ENEMY, 1, &Enemypos[i], 0, 0);
			enemy->TextureMap = { 0,9 };
			enemy->health = 3;
		}

		waves = 99;
	}

	//WAVESSSSSSSSS//////////////////////////////////
	ParticleSystemUpdate();
	//ShittyCollisionMap((float)(Player->posCurr.x), (float)(Player->posCurr.y));

}

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_Colosseum_Draw(void) {
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

			AEGfxSetTransparency(1.0f);

			AEMtx33 Translate, Scale, Transform;
			AEMtx33Trans(&Translate, Pos.x, Pos.y);
			AEMtx33Scale(&Scale, SPRITE_SCALE, SPRITE_SCALE);
			AEMtx33Concat(&Transform, &Scale, &Translate);

			AEGfxTextureSet(Player->pObject->pTexture,
				TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH * MapObjInstList[i][j].x,
				TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT * MapObjInstList[i][j].y);

			AEGfxSetTransform(Transform.m);

			AEGfxMeshDraw(Player->pObject->pMesh, AE_GFX_MDM_TRIANGLES);

			if (mapeditor == 1) {


				AEGfxTextureSet(RefBox->pObject->pTexture, 0, 0);

				AEGfxSetTransform(Transform.m);

				AEGfxMeshDraw(RefBox->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}

	// map editor object
	AEGfxSetTransparency(0.4f);
	AEGfxTextureSet(mapEditorObj->pObject->pTexture,
		TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH * mapEditorObj->TextureMap.x,
		TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT * mapEditorObj->TextureMap.y);
	AEGfxSetTransform(mapEditorObj->transform.m);
	AEGfxMeshDraw(mapEditorObj->pObject->pMesh, AE_GFX_MDM_TRIANGLES);


	// Spawn Static entities
	for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
	{
		staticObjInst* pInst = sStaticObjInstList + i;

		// skip non-active object and reference boxes
		if (pInst->flag != FLAG_ACTIVE)
			continue;
		if ((pInst->pObject->type == TYPE_REF && mapeditor == 0) || pInst->pObject->type == TYPE_MAP) {
			continue;
		}
		if (utilities::checkWithinCam(pInst->posCurr, camX, camY)) {
			continue;
		}
		// for any transparent textures
		if (pInst->pObject->type == TYPE_SLASH) {
			AEGfxSetTransparency(1.0f - pInst->Alpha);
		}
		else {
			AEGfxSetTransparency(1.0f);
		}
		// For any types using spritesheet
		if (pInst->pObject->type == TYPE_HEALTH ||
			pInst->pObject->type == TYPE_LEVERS)
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
		if (pInst->pObject->type == TYPE_CHARACTER) {
			AEGfxTextureSet(pInst->pObject->pTexture,
				pInst->TextureMap.x * TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH,
				pInst->TextureMap.y * TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT);
		}

		else if (pInst->pObject->type == TYPE_ENEMY) {
			//std::cout << " ghost is spawnned near cam" << std::endl;
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

	GameObjInst* pchar;
	for (int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		pchar = sGameObjInstList + i;

		if (pchar->pObject->type == TYPE_CHARACTER) {
			break;
		}
	}
	ParticleSystemDraw(&pchar->transform);
}

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the Colosseum level.
*/
/******************************************************************************/
void GS_Colosseum_Free(void) {
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
	ParticleSystemFree();

}

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Colosseum level.
*/
/******************************************************************************/
void GS_Colosseum_Unload(void) {
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned int i = 0; i < sGameObjNum; i++) {
		if ((sGameObjList + i)->refMesh == false)
			AEGfxMeshFree((sGameObjList + i)->pMesh);
		if ((sGameObjList + i)->refTexture == false)
			AEGfxTextureUnload((sGameObjList + i)->pTexture);
	}

}

// ---------------------------------------------------------------------------





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
static GameObjInst* gameObjInstCreate(unsigned long type,
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
static void gameObjInstDestroy(GameObjInst* pInst)
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
static staticObjInst* staticObjInstCreate(unsigned long type, float scale, AEVec2* pPos, float dir)
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
static void staticObjInstDestroy(staticObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	sStaticObjInstNum--; //Decrement the number of game object instance
	pInst->flag = 0;
}

//int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY)
//{
//	int Flag = 0;
//	int x1, y1, x2, y2;
//
//	//-hotspot 1
//	x1 = PosX + scaleX / 2;	//To reach the right side
//	y1 = PosY + scaleY / 4;	//To go up 1 / 4 of the height
//
//	//- hotspot 2
//	x2 = PosX + scaleX / 2;	//To reach the right side
//	y2 = PosY - scaleY / 4;	//To go down 1 / 4 of the height
//
//	if (binaryMap[abs(y1)][abs(x1)] == 1 || binaryMap[abs(y2)][abs(x2)] == 1) {
//		Flag |= COLLISION_RIGHT;
//	}
//
//	//-hotspot 1
//	x1 = PosX - scaleX / 2;
//	y1 = PosY - scaleY / 4;
//
//	//- hotspot 2
//	x2 = PosX - scaleX / 2;
//	y2 = PosY + scaleY / 4;
//	if (binaryMap[abs(y1)][abs(x1)] == 1 || binaryMap[abs(y2)][abs(x2)] == 1) {
//		Flag |= COLLISION_LEFT;
//	}
//	//-hotspot 1
//	x1 = PosX + scaleX / 4;
//	y1 = PosY + scaleY / 2;
//
//	//- hotspot 2
//	x2 = PosX - scaleX / 4;
//	y2 = PosY + scaleY / 2;
//
//	if (binaryMap[abs(y1)][abs(x1)] == 1 || binaryMap[abs(y2)][abs(x2)] == 1) {
//		Flag |= COLLISION_TOP;
//	}
//	//-hotspot 1
//	x1 = PosX + scaleX / 4;
//	y1 = PosY - scaleY / 2;
//
//	//- hotspot 2
//	x2 = PosX - scaleX / 4;
//	y2 = PosY - scaleY / 2;
//
//	if (binaryMap[abs(y1)][abs(x1)] == 1 || binaryMap[abs(y2)][abs(x2)] == 1) {
//		Flag |= COLLISION_BOTTOM;
//	}
//	return Flag;
//}

