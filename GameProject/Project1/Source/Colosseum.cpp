/******************************************************************************/
/*!
\file		Colosseum.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 02, 2023
\brief		This source file contains the functions for the level of Colosseum.

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

static const unsigned int	MAX_MOBS = 11;						// The total number of mobs
static const int			totalWaves = 3;						// The total number of waves

static const unsigned int	MAX_CHESTS = 1;						// The total number of chests

static const int			MAP_CELL_WIDTH = 28;				// Total number of cell widths
static const int			MAP_CELL_HEIGHT = 29;				// Total number of cell heights

static const AEVec2 playerStartPoint{ 14,-16 };					// Starting coordinate of new game player

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

// Map variables
static AEVec2				MapObjInstList[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];	// 2D array of each map tile object
static int					binaryMap[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];			// 2D array of binary collision mapping


// pointer to the objects
static GameObjInst* Player;												// Pointer to the "Player" game object instance
static staticObjInst* mapEditorObj;										// Pointer to the reference map editor object instance
static staticObjInst* Health[3];										// Pointer to the player health statc object instance
static staticObjInst* Chest[MAX_CHESTS];								// Pointer to Chest objects
static staticObjInst* MenuObj[3];										// Pointer to each menu UI object
static staticObjInst* NumObj[3];										// Pointer to each number UI object
static staticObjInst* PauseObj;											// Pointer to Pause Obj
static staticObjInst* StartScreenbj;									// Pointer to start screen Obj

// Object Instance Generation variables
static int chestnum;					// Number of chests
static int waves;						// Wave number

// State Variables
static bool spawned;					// Wave spawning state
static bool	debugging;					// Debugging state
static bool	mapeditor;					// Map edtior state

// Time Variables
static float slashCD;					// Stores time left before player can slash again
static float walkCD;					// Stores time left before player can move after slashing
static float playerHitTime;				// Stores time left for player invulnerability after being hit

static Inventory Backpack;				// Inventory of Character

static int CURRENT_MOBS;				// Current number of mobs in game

static AEAudio ColosseumBGM;
static AEAudioGroup ColosseumGroup;


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
	// Setting initial numbers to 0
	sGameObjNum = 0;
	sGameObjInstNum = 0;
	sStaticObjInstNum = 0;
	Player = nullptr;

	// List of Unique Game Objs
	GameObj* Character = 0, * Item = 0, * Map = 0, * Slash = 0,
		* RefLine = 0, * Health = 0, * Enemy = 0, * Key = 0,
		* Bullet = 0, * Chest = 0, * Spike = 0, * Spike_nonfade = 0,
		* Pause = 0, *Start = 0;


	// =====================================
	//	Load Meshes
	// =====================================
	
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

	// =====================================
	//	Load Textures
	// =====================================

	textureList.push_back(AEGfxTextureLoad("Assets/slash.png")); // 0
	textureList.push_back(AEGfxTextureLoad("Assets/Tilemap/RefBox.png")); // 1
	textureList.push_back(AEGfxTextureLoad("Assets/Tilemap/tilemap_packed.png")); // 2
	textureList.push_back(AEGfxTextureLoad("Assets/ColloStart.png")); // 3
	textureList.push_back(AEGfxTextureLoad("Assets/PauseScreen.png")); // 4
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_1.png")); //5
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_2.png")); //6
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_3.png")); //7


	//Texture Alias
	AEGfxTexture*& slashTex = textureList[0];
	AEGfxTexture*& refBox = textureList[1];
	AEGfxTexture*& spriteSheet = textureList[2];
	AEGfxTexture*& startTex = textureList[3];
	AEGfxTexture*& PauseTex = textureList[4];



	// =====================================
	//	Load Unique Game Objs
	// =====================================
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

	// Load Audio
	HeroDamaged = AEAudioLoadMusic("Assets/Music/HUMAN-GRUNT_GEN-HDF-15047.wav");
	Damage = AEAudioCreateGroup();
	HeroSlash = AEAudioLoadMusic("Assets/Music/METAL-HIT_GEN-HDF-17085.wav");
	Movement = AEAudioLoadMusic("Assets/Music/FOOTSTEPS-OUTDOOR_GEN-HDF-12363.mp3");
	MovementGroup = AEAudioCreateGroup();
	ColosseumBGM = AEAudioLoadMusic("Assets/Music");
	ColosseumGroup = AEAudioCreateGroup();
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Colosseum state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_Colosseum_Init(void) {
	//Create objects for pause and start screen
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
	//	Initialize objects for laded game
	// =====================================
		//Initialise Player
	AEVec2 PlayerPos = playerStartPoint;
	Player = gameObjInstCreate(TYPE_CHARACTER, 1, &PlayerPos, 0, 0);

	std::ifstream ifs{ "Assets/save.txt" };
	ifs >> Player->health;
	ifs >> Backpack.Key; //set to player number of current potion
	ifs >> Backpack.Potion; //set to player number of current key
	ifs.close();

	Player->damage = 1;

	//Initialise player health UI.
	for (int i = 0; i < MAX_PLAYER_HEALTH; i++) {
		Health[i] = staticObjInstCreate(TYPE_HEALTH, 0.75, nullptr, 0);
	}

	//Init pathfinding nodes
	NodesInit(*binaryMap, MAP_CELL_WIDTH, MAP_CELL_HEIGHT);

	// Initialise camera pos
	camX = 0, camY = 0;
	AEGfxSetCamPosition(0, 0);

	// =====================================
	//	Initialize UI objects
	// =====================================


	MenuObj[0] = staticObjInstCreate(TYPE_ITEMS, 1, nullptr, 0); // Potions
	MenuObj[1] = staticObjInstCreate(TYPE_KEY, 1, nullptr, 0); // Keys


	NumObj[0] = staticObjInstCreate(TYPE_ITEMS, 1, nullptr, 0); // Potions
	NumObj[1] = staticObjInstCreate(TYPE_KEY, 1, nullptr, 0); // Keys

	AEAudioPlay(ColosseumBGM, ColosseumGroup, 0.1f, 1, 1);

	// Initialise in-game states
	spawned = false;					
	debugging = false;					
	mapeditor = false;					
	levelstart = true;
	pause = true;
	cycle = 0;
	waves = 0;

	// Time Variables
	slashCD = 0;				
	walkCD = 0;					
	playerHitTime = 0;			

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

	// Toggle Pause button
	if (AEInputCheckTriggered(AEVK_ESCAPE)) {
		pause = !pause;
		levelstart = false;
		cycle = 0;
	}

	// Toggle help screen
	if (pause == true && levelstart == false) {
		utilities::moveHelpScreen(*PauseObj, 4);
	}

	if (pause == false) {



		// Normalising mouse to 0,0 at the center
		s32 mouseIntX, mouseIntY;
		AEInputGetCursorPosition(&mouseIntX, &mouseIntY);
		mouseX = (float)(mouseIntX - AEGetWindowWidth() / 2) / SPRITE_SCALE;
		mouseY = (float)(-mouseIntY + AEGetWindowHeight() / 2) / SPRITE_SCALE;

		// Calculating Angle between mouse and Player for Slash purposes

		float angleMousetoPlayer = utilities::getAngle(Player->posCurr.x, Player->posCurr.y, mouseX + camX, mouseY + camY);
		if (mouseY + camY > Player->posCurr.y) {
			angleMousetoPlayer = -angleMousetoPlayer;
		}

		//Time-related variables
		utilities::decreaseTime(slashCD);
		utilities::decreaseTime(walkCD);
		utilities::decreaseTime(playerHitTime);

		// =====================================
		// User Input
		// =====================================
		//Debugging mode - Developer use
		if (AEInputCheckTriggered(AEVK_F3)) {
			debugging ^= true;
		}
		//Map editor mode - Developer Use
		if (AEInputCheckTriggered(AEVK_9)) {
			mapeditor ^= true;
		}

		Player->playerStand();

		// Movement Controls
		if (AEInputCheckCurr(AEVK_W) || AEInputCheckCurr(AEVK_UP) || AEInputCheckCurr(AEVK_S) || AEInputCheckCurr(AEVK_DOWN)
			|| AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT) || AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT)) {
			Player->playerWalk(walkCD);
		}
		else {
			Player->TextureMap = TEXTURE_PLAYER;
			AEAudioStopGroup(MovementGroup);
		}

		if (CURRENT_MOBS == 0 && waves >= 1) { // From wave 2 onwards, wave 1 activated by chest
			waves++;
			spawned = true;
			// Level complete condition
			if (waves > totalWaves) { // If complete last wave
				utilities::completeLevel(colosseum, Player, Backpack);
			}
		}

		if (AEInputCheckTriggered(AEVK_E)) {

			for (int i = 0; i < chestnum; i++)
			{
				//Interaction with Chest
				if (Player->calculateDistance(*Chest[i]) < chestRange && Chest[i]->TextureMap != TEXTURE_OPENEDCHEST)
				{
					//change texture of chest
					Chest[i]->chest2Potion();
					spawned = true;
					waves++;
				}
			}
		}

		// Healing from Potion
		if (AEInputCheckTriggered(AEVK_R))
		{
			Player->drinkPotion(Health, Backpack);
		}

		// Slashing Input
		if (AEInputCheckTriggered(AEVK_LBUTTON) && slashCD == 0) {
			Player->playerSlashCreate(angleMousetoPlayer);
			slashCD = SLASH_COOLDOWN_t;
			walkCD = WALK_COOLDOWN_t;
			Player->playerStand();
		}

		//Map editor selection - Developer Use
		if (mapeditor == true) {
			mapEditorObj->mapEditorObjectSpawn(mouseX, mouseY, camX, camY);

			utilities::changeMapObj(mouseX + camX, mouseY + camY, MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, *mapEditorObj);

		}
		else {
			mapEditorObj->scale = 0;
		}

		// Map editor printing - Developer Use
		if (AEInputCheckTriggered(AEVK_8)) {
			utilities::exportMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureTower.txt");

			utilities::exportMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "binaryTower.txt");
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

		// Calculate Bounding Box for Dynamic Objects
		for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
			GameObjInst* pInst = sGameObjInstList + i;
			if (pInst->flag != FLAG_ACTIVE) {
				continue;
			}
			pInst->calculateBB();
		}

		// Calculate Bounding Box for Static Objects
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
			if (pInst->flag != FLAG_ACTIVE) {
				continue;
			}

			if (pInst->velCurr.x != 0 || pInst->velCurr.y != 0) //if player direction is not 0, as you cannot normalize 0.
			{
				pInst->velToPos();
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
				// Between Enemy and Player
				if (CollisionIntersection_RectRect(Player->boundingBox, Player->velCurr, pInst->boundingBox, pInst->velCurr)
					&& playerHitTime == 0)
				{
					if (Player->health > 0)
					{
						Player->deducthealth();
						AEAudioPlay(HeroDamaged, Damage, 0.3f, 1, 0);
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
					// Between Enemy and Slashes
					if (pInst->calculateDistance(*jInst) < slashRange
						&& jInst->Alpha == 0) {
						pInst->deducthealth(Player->damage);
						// Knockback
						pInst->mobKnockback(*jInst);
					}
				}
			}
		}

		int flag = CheckInstanceBinaryMapCollisionCollo(Player->posCurr.x, -Player->posCurr.y, binaryMap);
		// Binary collision for player
		snapCollision(*Player, flag);

		// Tint of character if damaged
		Player->playerDamaged(playerHitTime);

		//Change level conditions
		if (Player->health == 0) {
			gGameStateNext = GS_DEATHSCREEN;
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
			// Updating slash effects
			if (pInst->pObject->type == TYPE_SLASH) {
				pInst->playerSlashUpdate();
			}

			// Picking up items
			if (pInst->pObject->type == TYPE_ITEMS) {
				if (Player->calculateDistance(*pInst) < pickUpRange)
				{
					Backpack.itemPickUp(pInst);
				}
			}
		}

		for (int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
			GameObjInst* pInst = sGameObjInstList + i;
			if (pInst->flag != FLAG_ACTIVE) {
				continue;
			}
			// Killing Mobs
			if (pInst->pObject->type == TYPE_ENEMY)
			{
				if (pInst->health == 0)
				{
					pInst->mobsKilled();
					CURRENT_MOBS -= 1;
				}
			}
			// Increment global time tracker for character
			if (pInst->pObject->type == TYPE_CHARACTER) {
				pInst->timetracker += g_dt;
			}
		}

		// Spawning mobs for waves
		if (spawned == true) {
			std::string input = "colosseumWave" + std::to_string(waves) + ".txt";

			AEVec2* pos = nullptr;
			utilities::loadObjs(pos, CURRENT_MOBS, input);
			for (int i = 0; i < CURRENT_MOBS; i++) {
				GameObjInst* enemy = gameObjInstCreate(TYPE_ENEMY, 1, &pos[i], 0, 0);
			}

			utilities::unloadObjs(pos);
			spawned = false;
		}

		// Dust particles for player
		Player->dustParticles();

		// Camera position and UI items
		utilities::snapCamPos(Player->posCurr, camX, camY, MAP_CELL_WIDTH, MAP_CELL_HEIGHT);
		AEGfxSetCamPosition(static_cast<f32>(static_cast<int>(camX * (float)SPRITE_SCALE)), static_cast<f32>(static_cast<int> (camY * (float)SPRITE_SCALE)));
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

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	if (pause == true) {
		// Draw starting instructions
		if (levelstart == true)
		{
			AEMtx33 rot, scale, trans;

			AEGfxTextureSet(StartScreenbj->pObject->pTexture, 0, 0);

			AEMtx33Rot(&rot, 0);
			AEMtx33Trans(&trans, camX * SPRITE_SCALE, camY * SPRITE_SCALE);
			AEMtx33Scale(&scale, static_cast<f32>(AEGetWindowWidth()), static_cast<f32>(AEGetWindowHeight()));
			AEMtx33Concat(&StartScreenbj->transform, &rot, &scale);
			AEMtx33Concat(&StartScreenbj->transform, &trans, &StartScreenbj->transform);


			// Set the current object instance's transform matrix using "AEGfxSetTransform"
			AEGfxSetTransform(StartScreenbj->transform.m);
			// Draw the shape used by the current object instance using "AEGfxMeshDraw"
			AEGfxMeshDraw(StartScreenbj->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}
		// Draw pause screen
		else
		{

			AEGfxTextureSet(PauseObj->pObject->pTexture, 0, 0);

			PauseObj->transform.m[0][2] = camX * SPRITE_SCALE;
			PauseObj->transform.m[1][2] = camY * SPRITE_SCALE;
			PauseObj->transform.m[0][0] = static_cast<f32>(AEGetWindowWidth());
			PauseObj->transform.m[1][1] = static_cast<f32>(AEGetWindowHeight());

			// Set the current object instance's transform matrix using "AEGfxSetTransform"
			AEGfxSetTransform(PauseObj->transform.m);
			// Draw the shape used by the current object instance using "AEGfxMeshDraw"
			AEGfxMeshDraw(PauseObj->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}


	} else if (pause == false) {

		// Drawing map tiles
		for (unsigned long i = 0; i < MAP_CELL_WIDTH; i++) {
			for (long j = 0; j < MAP_CELL_HEIGHT; j++) {
				AEVec2 Pos = { i + 0.5f, -j - 0.5f };

				// Only draw within viewport
				if (utilities::checkWithinCam(Pos, camX, camY)) {
					continue;
				}
				AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

				// Tint the tile mouse is hovering over when mapeditor mode is on
				if (mapeditor == true && (int)(mouseX + camX) == (int)Pos.x && (int)(mouseY + camY) == (int)Pos.y) {
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

		ParticleSystemDraw(&Player->transform);   //localtransform
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

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
			// Only draw within viewport

			if (utilities::checkWithinCam(pInst->posCurr, camX, camY)) {
				continue;
			}
			// for any sprite textures

			AEGfxTextureSet(pInst->pObject->pTexture,
				pInst->TextureMap.x* TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH,
				pInst->TextureMap.y* TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT);
			// For tint upon damage, all set to 1 by default
			AEGfxSetTintColor(pInst->damagetint.red, pInst->damagetint.green, pInst->damagetint.blue, 1.0f);
			// Set the current object instance's transform matrix using "AEGfxSetTransform"
			AEGfxSetTransform(pInst->transform.m);
			// Draw the shape used by the current object instance using "AEGfxMeshDraw"
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}


		if (debugging == true)
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

	AEAudioStopGroup(ColosseumGroup);

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

	// Reset camera position
	AEGfxSetCamPosition(0, 0);

	// Clear vectors
	meshList.clear();
	textureList.clear();

	ParticleSystemUnload();
}