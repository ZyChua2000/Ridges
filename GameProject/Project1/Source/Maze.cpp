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


/******************************************************************************/
/*!
	Struct/Class Definitions
*/
/******************************************************************************/

// ---------------------------------------------------------------------------


static AEVec2				MapObjInstList[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];	// 2D array of each map tile object
static int					binaryMap[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];	// 2D array of binary collision mapping

static AEVec2				MiniMapObjInstList[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];

static s8					FontList[FONT_NUM_MAX];						// Each element in this array represents a Font
static unsigned long		FontListNum;								// The number of used fonts

// pointer to the objects
static GameObjInst* Player;												// Pointer to the "Player" game object instance
static staticObjInst* mapEditorObj;										// Pointer to the reference map editor object instance
static staticObjInst* Health[3];										// Pointer to the player health statc object instance
static staticObjInst* Levers[3];										// Pointer to each enemy object instance
static GameObjInst* enemy[2];
static staticObjInst* MenuObj[3];										// Pointer to each enemy object instance
static staticObjInst* NumObj[3];
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

static int countx = 0;
static int county = 0;
static float mappingarrx[3000];
static float mappingarry[3000];
static int count = 0;
static int movement=0;
static int arrin = 0;

static int pathingcd = 0;
static float pathingcdtime = 0;
static float minimaptime = 0;

static float slashCD = 0;
static float walkCD = 0;
Inventory Backpack;
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

	AEGfxTriAdd(-60.f, 35.f, 0xd0d4c700, 0.0f, 0.0f,  //120 by 70 scale 10
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


	// =====================================
	//	Initialize map textures
	// =====================================

	utilities::importMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureMaze.txt");

	// =====================================
	//	Initialize map binary
	// =====================================

	utilities::importMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *binaryMap, "binaryMaze.txt");

	mapEditorObj = staticObjInstCreate(TYPE_MAP, 0, nullptr, 0);

	// =====================================
	//	Initialize objects for new game
	// =====================================
	//Initialise Player
	AEVec2 PlayerPos = { 12.f,-8.f };
	Player = gameObjInstCreate(TYPE_CHARACTER, 1, &PlayerPos, 0, 0);

	std::ifstream ifs{ "Assets/save.txt" };
	ifs >> Player->health;
	ifs >> Backpack.Key; //set to player number of current potion
	ifs >> Backpack.Potion; //set to player number of current key
	ifs.close();

	Player->damage = 1;

	//Initialise player health.
	for (int i = 0; i < 3; i++) {
		Health[i] = staticObjInstCreate(TYPE_HEALTH, 0.75, nullptr, 0);
	}

	// Initialise camera pos
	camX = Player->posCurr.x, camY = Player->posCurr.y;

	// =====================================
//	Initialize UI objects
// =====================================


	MenuObj[0] = staticObjInstCreate(TYPE_ITEMS, 1, nullptr, 0); // Potions
	MenuObj[1] = staticObjInstCreate(TYPE_KEY, 1, nullptr, 0); // Keys


	NumObj[0] = staticObjInstCreate(TYPE_ITEMS, 1, nullptr, 0); // Potions
	NumObj[1] = staticObjInstCreate(TYPE_KEY, 1, nullptr, 0); // Keys
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Maze state.
*/
/******************************************************************************/

void GS_Maze_Update(void) {
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

	if (AEInputCheckTriggered(AEVK_9)) {
		mapeditor ^= 1;
	}
	//Dark Mesh toggle
	if (AEInputCheckTriggered(AEVK_1))
	{
		dark ^= 1;
	}

	//Minimap toggle
	if (pathingcd == 0) {
		if (AEInputCheckTriggered(AEVK_M))
		{
			pathingcd = 1;
			minimap ^= 1;
		}
	}
	if (pathingcd == 1)
	{
		pathingcdtime += g_dt;
		if ((int)pathingcdtime >= 10)
		{
			pathingcd = 0;
			pathingcdtime = 0;
		}
	}
	if (minimap == 1)
	{
		minimaptime += g_dt;
		if ((int)minimaptime >= 3)
		{
			minimap = 0;
			minimaptime = 0;
		}


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
		utilities::exportMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureMaze.txt");

		utilities::exportMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "binaryMaze.txt");
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


	/////////////////////////////////////////// MINIMAP///////////////////////////
	int playerx = Player->posCurr.x;
	int playery = Player->posCurr.y;
			MiniMapObjInstList[playerx][playery] = mapEditorObj->TextureMap;
		
	

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
				pInst->calculateBB();
			}

			for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++) {
				staticObjInst* pInst = sStaticObjInstList + i;
				if (pInst->flag != FLAG_ACTIVE) {
					continue;
				}
				if (pInst->pObject->type != TYPE_SLASH) {
					continue;
				}
				pInst->calculateBB();
			}

	// ======================================================
	//	-- Positions of the instances are updated here with the already computed velocity (above)
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
					pInst->playerSlashUpdate();
				}
			}

			for (int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
				GameObjInst* pInst = sGameObjInstList + i;
				if (pInst->flag != FLAG_ACTIVE) {
					continue;
				}

				if (pInst->pObject->type == TYPE_CHARACTER) {
					pInst->timetracker += g_dt;
				}
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
	
	if (movement == 1)
	{
		
		
			mappingarrx[arrin] = (camX * SPRITE_SCALE)+posx/100;
		

		
			mappingarry[arrin] = (camY * SPRITE_SCALE)+posy/100;
			arrin++;
			
	}
	if (count >= 3000)
		count = 0;
	//printf("%d\n", arrin);
	//printf("%f\n", mappingarrx[3]);
	
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


	
	if (dark == 0) {
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
		
		//if (MapMesh) {
		//	// Create a scale matrix that scales by 100 x and y
		//	
		//	AEMtx33Scale(&lscale, 10, 10);
		//	// Create a rotation matrix that rotates by 45 degrees
		//	

		//	AEMtx33Rot(&lrotate, 0);

		//	// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis
		//	
		//	AEMtx33Trans(&ltranslate, camX * SPRITE_SCALE, camY * SPRITE_SCALE);
		//	// Concat the matrices (TRS) 
		//	AEMtx33Concat(&ltransform, &lrotate, &lscale);
		//	AEMtx33Concat(&ltransform, &ltranslate, &ltransform);
		//	AEGfxSetTransform(ltransform.m);
		//	// Actually drawing the mesh
		//	AEGfxMeshDraw(MapMesh, AE_GFX_MDM_TRIANGLES);
		//}
		if (MapChar)
		{
			

				//if (posx != prevX || posy != prevY)
				//{
				//currX = (camX * SPRITE_SCALE) + posx / 10;
				


				// Create a rotation matrix that rotates by 45 degrees
				for (unsigned long i = 0; i < 3000; i++)
				{
					AEMtx33Scale(&lscale, 3, 3);
					AEMtx33Rot(&lrotate, 0);

					AEMtx33Concat(&ltransform, &lrotate, &lscale);
					// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis

					//AEMtx33Trans(&ltranslate, (camX * SPRITE_SCALE) + posx / 10, (camY * SPRITE_SCALE) + posy / 10);
					
						AEMtx33Trans(&ltranslate, mappingarrx[i], mappingarry[i]);
					// Concat the matrices (TRS) 

					// Actually drawing the mesh
					AEMtx33Concat(&ltransform, &ltranslate, &ltransform);
					AEGfxSetTransform(ltransform.m);
				
						AEGfxMeshDraw(MapChar, AE_GFX_MDM_TRIANGLES);

					//count++;
				}	
		}	
	}
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
	{
		staticObjInst* pInst = sStaticObjInstList + i;

		// skip non-active object and reference boxes
		if (pInst->flag != FLAG_ACTIVE)
			continue;
		if ((pInst->pObject->type == TYPE_REF && mapeditor == 0) || pInst->pObject->type == TYPE_MAP || pInst->pObject->type == TYPE_LEVERS) {
			continue;
		}
		if (utilities::checkWithinCam(pInst->posCurr, camX, camY)) {
			continue;
		}
		// for any transparent textures
		// For any types using spritesheet
		if (pInst->pObject->type == TYPE_HEALTH)
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
	if (minimap == 0)
	{
		char tracker[50] = "Press M to Track Past Pathing";
		AEGfxPrint(1, tracker, 0.60f, 0.75f, 1.5f, 1.0f, 1.0f, 1.0f);
	}
	
	char pathtimerdisplay[50];
	if (10 - pathingcdtime == 10)
	{
		sprintf_s(pathtimerdisplay, "Pathing Cooldown: %.f", 0);
		AEGfxPrint(1, pathtimerdisplay, 0.60f, 0.65f, 1.5f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		sprintf_s(pathtimerdisplay, "Pathing Cooldown: %.f", 10 - pathingcdtime);
		AEGfxPrint(1, pathtimerdisplay, 0.60f, 0.65f, 1.5f, 1.0f, 1.0f, 1.0f);
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

