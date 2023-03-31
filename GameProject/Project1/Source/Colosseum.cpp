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



/*!
	Defines
*/
/******************************************************************************/
static saveData				data;
static Node* nodes{};

bool pause;
//static const unsigned int	MAX_CHESTS;							// The total number of chests
static const unsigned int	MAX_LEVERS = 3;						// The total number of levers
static const unsigned int	MAX_MOBS = 11;							// The total number of mobs
static int					CURRENT_MOBS = MAX_MOBS;

static const unsigned int	MAX_CHESTS = 1;						// The total number of chests

static bool					SLASH_ACTIVATE = false;				// Bool to run slash animation

static const int			MAP_CELL_WIDTH = 28;				// Total number of cell widths
static const int			MAP_CELL_HEIGHT = 29;				// Total number of cell heights

static unsigned int			state = 0;							// Debugging state
static unsigned int			mapeditor = 0;						// Map edtior state

static						AEVec2 binaryPlayerPos;				// Position on Binary Map


static float slashCD = 0;
static float walkCD = 0;

bool levelstart;


// -----------------------------------------------------------------------------
//static Node* nodes{};

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


// pointer to the objects
static GameObjInst* Player;												// Pointer to the "Player" game object instance
static staticObjInst* mapEditorObj;										// Pointer to the reference map editor object instance
static staticObjInst* Health[3];										// Pointer to the player health statc object instance
static staticObjInst* RefBox;
static staticObjInst* Chest[MAX_CHESTS];
static staticObjInst* MenuObj[3];										// Pointer to each enemy object instance
static staticObjInst* NumObj[3];
static staticObjInst* PauseObj;
static staticObjInst* StartScreenbj;
static Inventory Backpack;
static int chestnum;

static std::vector<AEGfxTexture*> textureList;
static std::vector<AEGfxVertexList*> meshList;

float Timer = 0.f;
static float internalTimer = 0.f;
int waves = 0;
float wavestimer = 0.f;
bool spawned = false;

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
	meshList.clear();
	textureList.clear();
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


	GameObj* Character = 0, * Item = 0, * Map = 0, * Slash = 0,
		* RefLine = 0, * Health = 0, * Enemy = 0, * Key = 0,
		* Bullet = 0, * Chest = 0, * Spike = 0, * Spike_nonfade = 0,
		* Pause = 0, *Start = 0;

	//Mesh for Sprite Sheet - 0
	AEGfxMeshStart();

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFF00FF, 16.0f / TEXTURE_MAXWIDTH, 16.0f / TEXTURE_MAXHEIGHT,
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 16.0f / TEXTURE_MAXHEIGHT,
		0.5f, 0.5f, 0xFF00FFFF, 16.0f / TEXTURE_MAXWIDTH, 0.0f);

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 16.0f / TEXTURE_MAXHEIGHT,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 16.0f / TEXTURE_MAXWIDTH, 0.0f);

	meshList.push_back(AEGfxMeshEnd());

	// Mesh for whole texture files - 1
	AEGfxMeshStart();

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
		0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f);

	meshList.push_back(AEGfxMeshEnd());

	//Mesh Alias
	AEGfxVertexList*& spriteMesh = meshList[0];
	AEGfxVertexList*& fullSizeMesh = meshList[1];


	//Load Textures
	textureList.push_back(AEGfxTextureLoad("Assets/slash.png")); // 0
	textureList.push_back(AEGfxTextureLoad("Assets/Tilemap/RefBox.png")); // 1
	textureList.push_back(AEGfxTextureLoad("Assets/Tilemap/tilemap_packed.png")); // 2
	textureList.push_back(AEGfxTextureLoad("Assets/PauseScreen.png")); // 3
	textureList.push_back(AEGfxTextureLoad("Assets/ColloStart.png")); // 4

	//Texture Alias
	AEGfxTexture*& slashTex = textureList[0];
	AEGfxTexture*& refBox = textureList[1];
	AEGfxTexture*& spriteSheet = textureList[2];
	AEGfxTexture*& PauseTex = textureList[3];
	AEGfxTexture*& startTex = textureList[4];


	// Load mesh and texture into game objects
	utilities::loadMeshNTexture(Character, spriteMesh, spriteSheet, TYPE_CHARACTER);
	utilities::loadMeshNTexture(Item, spriteMesh, spriteSheet, TYPE_ITEMS);
	utilities::loadMeshNTexture(Map, spriteMesh, spriteSheet, TYPE_MAP);
	utilities::loadMeshNTexture(Slash, fullSizeMesh, slashTex, TYPE_SLASH);
	utilities::loadMeshNTexture(RefLine, fullSizeMesh, refBox, TYPE_REF);
	utilities::loadMeshNTexture(Health, spriteMesh, spriteSheet, TYPE_HEALTH);
	utilities::loadMeshNTexture(Enemy, spriteMesh, spriteSheet, TYPE_ENEMY);
	utilities::loadMeshNTexture(Key, spriteMesh, spriteSheet, TYPE_KEY);
	utilities::loadMeshNTexture(Bullet, spriteMesh, spriteSheet, TYPE_BULLET);
	utilities::loadMeshNTexture(Chest, spriteMesh, spriteSheet, TYPE_CHEST);
	utilities::loadMeshNTexture(Spike, spriteMesh, spriteSheet, TYPE_SPIKE);
	utilities::loadMeshNTexture(Spike_nonfade, spriteMesh, spriteSheet, TYPE_SPIKE_NONFADE);
	utilities::loadMeshNTexture(Pause, fullSizeMesh, PauseTex, TYPE_PAUSE);
	utilities::loadMeshNTexture(Start, fullSizeMesh, startTex, TYPE_START);

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
	PauseObj = staticObjInstCreate(TYPE_PAUSE, 1, nullptr, 0);
	StartScreenbj = staticObjInstCreate(TYPE_START, 1, nullptr, 0);

	AEVec2* pos = nullptr;

	// =====================================
	//	Initialize map textures
	// =====================================

	utilities::importMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureColosseum.txt");

	// =====================================
	//	Initialize map binary
	// =====================================

	utilities::importMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *binaryMap, "binaryColosseum.txt");

	mapEditorObj = staticObjInstCreate(TYPE_MAP, 0, nullptr, 0);

	utilities::loadObjs(pos, chestnum, "colosseumChest.txt");
	for (int i = 0; i < chestnum; i++) {
		Chest[i] = staticObjInstCreate(TYPE_CHEST, 1, &pos[i], 0);
	}
	utilities::unloadObjs(pos);
	// =====================================
	//	Initialize objects for new game
	// =====================================
		//Initialise Player
	AEVec2 PlayerPos = { 14.f,-16.f };
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

	//PauseObj = staticObjInstCreate(TYPE_PAUSE, 2, nullptr, 0);

	levelstart = 1;
	pause = 0;

	ParticleSystemInit();

}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Colosseum state.
*/
/******************************************************************************/

void GS_Colosseum_Update(void) {


	if (AEInputCheckTriggered(AEVK_ESCAPE)) {
		pause = !pause;
		levelstart = 0;
	}
		if (pause == 1) {


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

				for (int i = 0; i < chestnum; i++)
				{
					//Interaction with Chest
					if (Player->calculateDistance(*Chest[i]) < 1 && Chest[i]->TextureMap.x != 8)
					{
						//change texture of chest
						Chest[i]->chest2Potion();
						spawned = false;
						waves = 1;
					}
				}
			}


			if (CURRENT_MOBS == 0) {
				switch (waves) {
				case 1:
					waves = 2;
					break;
				case 2:
					waves = 3;
					break;
				case 3:
					//move to next level
					utilities::completeLevel(colosseum, Player, Backpack);
					waves = 4;
					break;
				default:
					break;
				}
				spawned = false;
			}



			if (waves == 1 && !spawned) {
				//Initialise enemy in level
				AEVec2* pos = nullptr;
				utilities::loadObjs(pos, CURRENT_MOBS, "colosseumWave1.txt");
				for (int i = 0; i < CURRENT_MOBS; i++) {
					GameObjInst* enemy = gameObjInstCreate(TYPE_ENEMY, 1, &pos[i], 0, 0);
				}
				utilities::unloadObjs(pos);
				spawned = true;
			}

			if (waves == 2 && !spawned) {
				//Initialise enemy in level
				AEVec2* pos = nullptr;
				utilities::loadObjs(pos, CURRENT_MOBS, "colosseumWave2.txt");
				for (int i = 0; i < CURRENT_MOBS; i++) {
					GameObjInst* enemy = gameObjInstCreate(TYPE_ENEMY, 1, &pos[i], 0, 0);
				}
				utilities::unloadObjs(pos);
				spawned = true;
			}

			if (waves == 3 && !spawned) {
				//Initialise enemy in level
				AEVec2* pos = nullptr;
				utilities::loadObjs(pos, CURRENT_MOBS, "colosseumWave3.txt");
				for (int i = 0; i < CURRENT_MOBS; i++) {
					GameObjInst* enemy = gameObjInstCreate(TYPE_ENEMY, 1, &pos[i], 0, 0);
				}
				utilities::unloadObjs(pos);
				spawned = true;
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

				if (Player->calculateDistance(*pEnemy) > enemySightRange)
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
				if (pInst->pObject->type != TYPE_SLASH) {
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
						if (pInst->calculateDistance(*jInst) < slashRange
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

			}

			int flag = CheckInstanceBinaryMapCollisionCollo(Player->posCurr.x, -Player->posCurr.y,binaryMap);

			snapCollision(*Player, flag);



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

			// Camera position and UI items

			AEGfxSetCamPosition(static_cast<f32>(static_cast<int>(camX * (float)SPRITE_SCALE)), static_cast<f32>(static_cast<int> (camY * (float)SPRITE_SCALE)));

			utilities::snapCamPos(Player->posCurr, camX, camY, MAP_CELL_WIDTH, MAP_CELL_HEIGHT);

			utilities::updatePlayerUI(Health, MenuObj, NumObj, Backpack, Player->health, camX, camY);

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


			//BUG NOT WORKING
			Player->dustParticles();

			for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
			{
				GameObjInst* pInst = sGameObjInstList + i;
				if (pInst->pObject && pInst->pObject->type == TYPE_ENEMY) {

					if (pInst->flag == 1) {
						pInst->dustParticles();
					}
				}
			}


			ParticleSystemUpdate();
			

		}


}

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_Colosseum_Draw(void) {

	if (!pause) {

		if (levelstart)
		{
			AEMtx33 rot, scale, trans;

			staticObjInst* pInst = StartScreenbj;
			// Tell the engine to get ready to draw something with texture. 
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);

			AEGfxTextureSet(pInst->pObject->pTexture, 0, 0);

			AEMtx33Rot(&rot, 0);
			AEMtx33Trans(&trans, camX * SPRITE_SCALE, camY * SPRITE_SCALE);
			AEMtx33Scale(&scale,  1600, 900);
			AEMtx33Concat(&pInst->transform, &rot, &scale);
			AEMtx33Concat(&pInst->transform, &trans, &pInst->transform);


			// Set the current object instance's transform matrix using "AEGfxSetTransform"
			AEGfxSetTransform(pInst->transform.m);
			// Draw the shape used by the current object instance using "AEGfxMeshDraw"
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

		else
		{

			staticObjInst* pInst = PauseObj;
			// Tell the engine to get ready to draw something with texture. 
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);

			AEGfxTextureSet(pInst->pObject->pTexture, 0, 0);

			pInst->transform.m[0][2] = camX * SPRITE_SCALE;
			pInst->transform.m[1][2] = camY * SPRITE_SCALE;
			pInst->transform.m[0][0] = 1600;
			pInst->transform.m[1][1] = 900;

			// Set the current object instance's transform matrix using "AEGfxSetTransform"
			AEGfxSetTransform(pInst->transform.m);
			// Draw the shape used by the current object instance using "AEGfxMeshDraw"
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}


	} else if (pause) {
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
	deletenodes();

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
	for (AEGfxVertexList* Mesh : meshList) {
		AEGfxMeshFree(Mesh);
	}

	for (AEGfxTexture* texture : textureList) {
		AEGfxTextureUnload(texture);
	}

	//BUGGY CODE, IF UANBLE TO LOAD, CANNOT USE DEBUGGING MODE
	AEGfxSetCamPosition(0, 0);

	ParticleSystemUnload();
}