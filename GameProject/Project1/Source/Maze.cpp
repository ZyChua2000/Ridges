/******************************************************************************/
/*!
\file		Maze.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 02, 2023
\brief		This header file contains the functions for the level of Maze.

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


static const unsigned int	MAX_MOBS;							// The total number of mobs
static const unsigned int	MAX_CHESTS;							// The total number of chests
static const unsigned int	MAX_LEVERS = 3;						// The total number of levers

static bool					SLASH_ACTIVATE = false;				// Bool to run slash animation

static const int			MAP_CELL_WIDTH = 124;				// Total number of cell widths
static const int			MAP_CELL_HEIGHT = 42;				// Total number of cell heights


static unsigned int			state = 0;							// Debugging state
static unsigned int			mapeditor = 0;						// Map edtior state

static						AEVec2 binaryPlayerPos;				// Position on Binary Map
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

static AEVec2				MiniMapObjInstList[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];


// pointer to the objects
static GameObjInst* Player;												// Pointer to the "Player" game object instance
static staticObjInst* mapEditorObj;										// Pointer to the reference map editor object instance
static staticObjInst* Health[3];										// Pointer to the player health statc object instance
static staticObjInst* Levers[3];										// Pointer to each enemy object instance
static GameObjInst* enemy[2];
static GameObjInst* Mask;
static staticObjInst* RefBox;

AEGfxTexture* DarkRoom;
AEGfxVertexList* DarkMesh = 0;
AEGfxVertexList* MapMesh = 0;
AEGfxVertexList* MapChar = 0;

static int dark = 0;
float spiketimer = 0.f;
static int minimap = 0;
static int posx = 0;
static int posy = 0;
static int flag;
static float prevX = 0.0f;
static float prevY = 0.0f;
float currX = 0.0f;
float currY = 0.0f;
float angle = 0.0f;
static float walkCD = 0;

// ---------------------------------------------------------------------------

/******************************************************************************/


//int CheckInstanceBinaryMapCollision(float PosX, float PosY,
//	float scaleX, float scaleY);


/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Maze level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_Maze_Load(void) {
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

	AEGfxMeshStart();

	AEGfxTriAdd(80.0f, -45.f, 0xFFFF00FF, 1.0f, 1.0f,
		-80.0f, -45.f, 0xFFFFFF00, 0.0f, 1.0f,
		80.0f, 45.f, 0xFF00FFFF, 1.0f, 0.0f);

	AEGfxTriAdd(-80.0f, -45.f, 0xFFFFFFFF, 0.0f, 1.0f,
		-80.0f, 45.f, 0xFFFFFFFF, 0.0f, 0.0f,
		80.0f, 45.f, 0xFFFFFFFF, 1.0f, 0.0f);
	DarkMesh = AEGfxMeshEnd();
	DarkRoom = AEGfxTextureLoad("Assets/Darkroom.png");

	///////MAPMESH////////////
	AEGfxMeshStart();

	AEGfxTriAdd(-60.f, 35.f, 0xd0d4c700, 0.0f, 0.0f,
		-60.f, -35.f, 0xd0d4c700, 0.0f, 1.0f,
		60.f, 35.f, 0xd0d4c700, 1.0f, 0.0f);

	AEGfxTriAdd(60.f, -35.f, 0xd0d4c700, 1.0f, 1.0f,
		-60.f, -35.f, 0xd0d4c700, 0.0f, 1.0f,
		60.f, 35.f, 0xd0d4c700, 1.0f, 0.0f);

	MapMesh = AEGfxMeshEnd();


	
	AEGfxMeshStart();

	AEGfxTriAdd(-2.f, 2.f, 0xFFFF0000, 0.0f, 0.0f,
		-2.f, -2.f, 0xFFFF0000, 0.0f, 1.0f,
		2.f, 2.f, 0xFFFF0000, 1.0f, 0.0f);

	AEGfxTriAdd(2.f, -2.f, 0xFFFF0000, 1.0f, 1.0f,
		-2.f, -2.f, 0xFFFF0000, 0.0f, 1.0f,
		2.f, 2.f, 0xFFFF0000, 1.0f, 0.0f);

	MapChar = AEGfxMeshEnd();


}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Maze state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_Maze_Init(void) {
	//Initialise Player
	AEVec2 PlayerPos = { 12,-8 };
	Player = gameObjInstCreate(TYPE_CHARACTER, 1, &PlayerPos, 0, 0);
	Player->TextureMap = { 1,8 };

	//Initialise map textures
	std::ifstream mapInput{ "Assets/textureMaze.txt" };
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
	std::ifstream binInput{ "Assets/binaryMaze.txt" };
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

	//Initialise Levers in level
	for (int i = 0; i < 3; i++) {
		Levers[i] = staticObjInstCreate(TYPE_LEVERS, 1, &pos[i], 0);
		Levers[i]->TextureMap = { 2,11 };
	}

	// Initialise camera pos
	camX = Player->posCurr.x, camY = Player->posCurr.y;
	AEVec2 EnemyPos[2] = { {33.f, -40.f} ,{33.f, -45.f} };

	//Initialise enemy in level
	for (int i = 0; i < 2; i++) {
		enemy[i] = gameObjInstCreate(TYPE_ENEMY, 1, &EnemyPos[i], 0, 0);
		enemy[i]->TextureMap = { 0,9 };
	}

	//binaryMap[(int)(Player->posCurr.x+20)][(int)(Player->posCurr.y-58)] = test++;
	//{ 12,-31 };
	binaryPlayerPos = { 32,-89 };
	AEVec2 MapPos = { 0,0 };

	


	
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Maze state.
*/
/******************************************************************************/

void GS_Maze_Update(void) {
	
	//Debugging mode
	if (AEInputCheckTriggered(AEVK_F3)) {
		state ^= 1;
	}

	if (AEInputCheckTriggered(AEVK_9)) {
		mapeditor ^= 1;
	}

	Player->velCurr = { 0,0 };// set velocity to 0 initially, if key is released, velocity is set back to 0

	if (AEInputCheckReleased(AEVK_W) || AEInputCheckReleased(AEVK_UP) || AEInputCheckReleased(AEVK_S) || AEInputCheckReleased(AEVK_DOWN)
		|| AEInputCheckReleased(AEVK_A) || AEInputCheckReleased(AEVK_LEFT) || AEInputCheckReleased(AEVK_D) || AEInputCheckReleased(AEVK_RIGHT)) {
		Player->TextureMap = { 1,8 };
	}

	Player->playerWalk(walkCD);

	if (AEInputCheckTriggered(AEVK_E)) {

		//Interaction with levers
		for (int i = 0; i < 3; i++) {
			if (Player->calculateDistance(*Levers[i]) < 1) {
				//Switch lever to face down
				Levers[i]->TextureMap = { 3,11 };
				Levers[i]->posCurr.x -= 0.2f;
				//Remove gates: Change texture & Binary map
				switch (i) {
				case 0:
					for (int i = 17; i < 22; i++) {
						MapObjInstList[i][15] = { 0,4 };
						binaryMap[i][15] = 0;
					}
					break;
				case 1:
					for (int i = 32; i < 35; i++) {
						MapObjInstList[81][i] = { 0,4 };
						binaryMap[81][i] = 0;
					}
					MapObjInstList[81][56] = { 2,4 };
					break;
					//WIP for 3rd gate
				case 2:
					break;
				default:
					break;
				}
			}
		}

		//Interaction with chests
	}

	int slashDir{ 0 };
	// Normalising mouse to 0,0 at the center
	s32 mouseIntX, mouseIntY;
	AEInputGetCursorPosition(&mouseIntX, &mouseIntY);
	mouseX = (float)(mouseIntX - AEGetWindowWidth() / 2) / SPRITE_SCALE;
	mouseY = (float)(-mouseIntY + AEGetWindowHeight() / 2) / SPRITE_SCALE;

	float angleMousetoPlayer = utilities::getAngle(Player->posCurr.x, Player->posCurr.y, mouseX + Player->posCurr.x, mouseY + Player->posCurr.y);

	utilities::decreaseTime(walkCD);

	if (mouseY + camY > Player->posCurr.y) {
		angleMousetoPlayer = -angleMousetoPlayer;
	}
	if (angleMousetoPlayer <= -(PI * 3 / 4) || angleMousetoPlayer > (PI * 3 / 4)) {
		slashDir = 1;
	}
	if (angleMousetoPlayer > -(PI * 3 / 4) && angleMousetoPlayer <= -(PI * 1 / 4)) {
		slashDir = 2;
	}
	if (angleMousetoPlayer <= (PI * 1 / 4) && angleMousetoPlayer > -(PI * 1 / 4)) {
		slashDir = 3;
	}
	if (angleMousetoPlayer > (PI * 1 / 4) && angleMousetoPlayer <= (PI * 3 / 4)) {
		slashDir = 4;
	}

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		std::cout << angleMousetoPlayer << std::endl;
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
	/////////////////////////////////////////// MINIMAP///////////////////////////
	int playerx = Player->posCurr.x;
	int playery = Player->posCurr.y;
			MiniMapObjInstList[playerx][playery] = mapEditorObj->TextureMap;
		
	

	//Map editor printing
	if (AEInputCheckTriggered(AEVK_8)) {
		utilities::exportMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureMaze.txt");

		utilities::exportMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "binaryMaze.txt");
	}

	if (AEInputCheckTriggered(AEVK_7)) {
		//utilities::importMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *binaryMap, "binaryMaze.txt");
		std::ifstream binInput{ "Assets/binaryMaze.txt" };
		for (int i = 0; i < MAP_CELL_HEIGHT; i++) {
			for (int j = 0; j < MAP_CELL_WIDTH; j++) {
				binInput >> binaryMap[j][i];
			}
		}
		binInput.close();
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

	// Camera position, stops following character when at edge of Maze
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

	

	 flag = CheckInstanceBinaryMapCollision(Player->posCurr.x, -Player->posCurr.y, 1.0f, 1.0f, binaryMap);

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

	spiketimer += g_dt;

	for (int i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++) {
		staticObjInst* pInst = sStaticObjInstList + i;
		if (pInst->flag != 1 || pInst->pObject->type != TYPE_SPIKE) {
			continue;
		}
		if (Player->calculateDistance(*pInst) <= 1 && (pInst->Alpha == 0) && spiketimer >= 1) {

			--Player->health;
			spiketimer = 0.0f;
		}


		//will work for all spikes spawned, find a better way to do the timetracker

		if (pInst->timetracker2 == 0) {
			pInst->timetracker += g_dt;
		}
		if (pInst->timetracker > 2) {
			pInst->timetracker = 2;
		}

		if (pInst->timetracker == 2) {
			pInst->timetracker2 += g_dt;
		}

		if (pInst->timetracker2 > 2) {
			pInst->timetracker2 = 2;
		}

		if (pInst->timetracker2 == 2) {
			pInst->timetracker -= g_dt;
		}

		if (pInst->timetracker < 0) {
			pInst->timetracker = 0;
		}

		if (pInst->timetracker == 0) {
			pInst->timetracker2 -= g_dt;
		}
		if (pInst->timetracker2 < 0) {
			pInst->timetracker2 = 0;
		}
		pInst->Alpha = 1.0f - pInst->timetracker / 2;

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

	CheckInstanceBinaryMapCollision(Player->posCurr.x, -Player->posCurr.y, 1.0f, 1.0f, binaryMap);

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
	//ShittyCollisionMap((float)(Player->posCurr.x), (float)(Player->posCurr.y));
	//===================MINIMAP Position Update=================//
	if (flag & COLLISION_TOP || flag & COLLISION_BOTTOM || flag & COLLISION_LEFT || flag & COLLISION_RIGHT)
	{
		posx = posx;
		posy = posy;
	}
	else {
		posx += Player->velCurr.x;

		posy += Player->velCurr.y;

		
	}
	
	

	

}

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_Maze_Draw(void) {

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
			std::cout << " ghost is spawnned near cam" << std::endl;
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

	
	if (dark == 1) {
		AEGfxSetTransparency(1.0f);
		// Tell the engine to get ready to draw something with texture. 
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
		AEGfxSetTintColor(1.0f, 0.0f, 1.0f, 1.0f);
		// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxTextureSet(DarkRoom, 0, 0);
		// Create a scale matrix that scales by 100 x and y
		AEMtx33 lscale = { 0 };
		AEMtx33Scale(&lscale, 10, 10);
		// Create a rotation matrix that rotates by 45 degrees
		AEMtx33 lrotate = { 0, };

		AEMtx33Rot(&lrotate, 0);

		// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis
		AEMtx33 ltranslate = { 0 };
		AEMtx33Trans(&ltranslate, camX * SPRITE_SCALE, camY * SPRITE_SCALE);
		// Concat the matrices (TRS) 
		AEMtx33 ltransform = { 0 };
		AEMtx33Concat(&ltransform, &lrotate, &lscale);
		AEMtx33Concat(&ltransform, &ltranslate, &ltransform);
		// Choose the transform to use 
		AEGfxSetTransform(ltransform.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(DarkMesh, AE_GFX_MDM_TRIANGLES);
	}
	if (minimap == 1)
	{
		AEMtx33 lscale = { 0 };
		AEMtx33 lrotate = { 0, };
		AEMtx33 ltranslate = { 0 };
		AEMtx33 ltransform = { 0 };
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTransparency(1.0f);
		// Tell the engine to get ready to draw something with texture. 
		
		// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.5f);
		// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		
		if (MapMesh) {
			// Create a scale matrix that scales by 100 x and y
			
			AEMtx33Scale(&lscale, 10, 10);
			// Create a rotation matrix that rotates by 45 degrees
			

			AEMtx33Rot(&lrotate, 0);

			// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis
			
			AEMtx33Trans(&ltranslate, camX * SPRITE_SCALE, camY * SPRITE_SCALE);
			// Concat the matrices (TRS) 
			AEMtx33Concat(&ltransform, &lrotate, &lscale);
			AEMtx33Concat(&ltransform, &ltranslate, &ltransform);
			AEGfxSetTransform(ltransform.m);
			// Actually drawing the mesh
			AEGfxMeshDraw(MapMesh, AE_GFX_MDM_TRIANGLES);
		}
		if (MapChar)
		{
			

			//if (posx != prevX || posy != prevY)
			//{
			currX = (camX * SPRITE_SCALE) + posx / 10;
				AEMtx33Scale(&lscale, 3, 3);


				// Create a rotation matrix that rotates by 45 degrees


				AEMtx33Rot(&lrotate, 0);

				// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis

				//AEMtx33Trans(&ltranslate, (camX * SPRITE_SCALE) + posx / 10, (camY * SPRITE_SCALE) + posy / 10);
				AEMtx33Trans(&ltranslate, (camX * SPRITE_SCALE) + posx / 10, (camY * SPRITE_SCALE) + posy / 10);
				// Concat the matrices (TRS) 
				AEMtx33Concat(&ltransform, &lrotate, &lscale);
				AEMtx33Concat(&ltransform, &ltranslate, &ltransform);
				AEGfxSetTransform(ltransform.m);
				// Actually drawing the mesh
				AEGfxMeshDraw(MapChar, AE_GFX_MDM_TRIANGLES);
				
					
				

			//}
				
			
					
				
		}
		
			
		prevX = posx;
		prevY = posy;

		
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
}

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the Maze level.
*/
/******************************************************************************/
void GS_Maze_Free(void) {
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

}

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Maze level.
*/
/******************************************************************************/
void GS_Maze_Unload(void) {
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned int i = 0; i < sGameObjNum; i++) {
		if ((sGameObjList + i)->refMesh == false)
			AEGfxMeshFree((sGameObjList + i)->pMesh);
		if ((sGameObjList + i)->refTexture == false)
			AEGfxTextureUnload((sGameObjList + i)->pTexture);
	}

	//BUGGY CODE, IF UANBLE TO LOAD, CANNOT USE DEBUGGING MODE
	AEGfxMeshFree(DarkMesh);
	AEGfxTextureUnload(DarkRoom);
	AEGfxMeshFree(MapMesh);
	AEGfxMeshFree(MapChar);
}

// ---------------------------------------------------------------------------



