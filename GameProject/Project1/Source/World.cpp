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

static int					MAX_MOBS;
static int					CURRENT_MOBS;
static const unsigned int	MAX_CHESTS = 6;						// The total number of chests
static const unsigned int	MAX_LEVERS = 3;						// The total number of levers
static const unsigned int	MAX_KEYS;							// The total number of keys

static bool					SLASH_ACTIVATE = false;				// Bool to run slash animation

static const int			MAP_CELL_WIDTH = 124;				// Total number of cell widths
static const int			MAP_CELL_HEIGHT = 42;				// Total number of cell heights


static unsigned int			state = 0;							// Debugging state
static unsigned int			mapeditor = 0;						// Map edtior state

bool loadState;

static const AEVec2 WarpPts[8]{ {102, -34.5f}, {104, -33.5f},
							{109, -34.5f}, {111, -33.5f},
						  {121, -28.5f}, {122, -25.5f},
						  {106, -20.f}, {108, -19.f} };

// -----------------------------------------------------------------------------


void saveGame(saveData data, GameObjInst* gameObjList, staticObjInst* staticObjList, int gameObjMax, int staticObjMax);

void loadData(saveData data);


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
static staticObjInst* PauseObj;
static staticObjInst* StartScreenbj;

static AEVec2* Gates;
static int gatesNum;
static int levNum;
static int chestNum;

static bool pause;
static bool levelstart;
static bool directionShown;
static bool directionDraw;
static int cycle;


static float spikedmgtimer = 0.f;
static float internalTimer = 0.f;
static float playerHitTime;

static staticObjInst* RefBox;


static std::vector<AEGfxTexture*> textureList;
static std::vector<AEGfxVertexList*> meshList;

 


// ---------------------------------------------------------------------------

/******************************************************************************/


/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the World level.
	It should be called once before the start of the level.clic
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_World_Load(void) {
	
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

	GameObj* Character = 0, * Item = 0, * Map = 0, * Slash = 0,
		* RefLine = 0, * Health = 0, * Enemy = 0, * Key = 0,
		* Bullet = 0, * Lever = 0, * Chest = 0, * Spike = 0,
		* Spike_nonfade = 0, * Tower = 0, * Pause = 0, * Start = 0;

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
	textureList.push_back(AEGfxTextureLoad("Assets/World_Obj.png")); // 3
	textureList.push_back(AEGfxTextureLoad("Assets/PauseScreen.png")); // 4
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_1.png")); //5
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_2.png")); //6
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_3.png")); //7
	textureList.push_back(AEGfxTextureLoad("Assets/Direction.png")); //8


	//Texture Alias
	AEGfxTexture*& slashTex = textureList[0];
	AEGfxTexture*& refBox = textureList[1];
	AEGfxTexture*& spriteSheet = textureList[2];
	AEGfxTexture*& startTex = textureList[3];
	AEGfxTexture*& PauseTex = textureList[4];

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
	utilities::loadMeshNTexture(Lever, spriteMesh, spriteSheet, TYPE_LEVERS);
	utilities::loadMeshNTexture(Chest, spriteMesh, spriteSheet, TYPE_CHEST);
	utilities::loadMeshNTexture(Spike, spriteMesh, spriteSheet, TYPE_SPIKE);
	utilities::loadMeshNTexture(Spike_nonfade, spriteMesh, spriteSheet, TYPE_SPIKE_NONFADE);
	utilities::loadMeshNTexture(Tower, spriteMesh, spriteSheet, TYPE_TOWER);
	utilities::loadMeshNTexture(Pause, fullSizeMesh, PauseTex, TYPE_PAUSE);
	utilities::loadMeshNTexture(Start, fullSizeMesh, startTex, TYPE_START);

	ParticleSystemLoad();

	HeroDamaged = AEAudioLoadMusic("Assets/Music/HUMAN-GRUNT_GEN-HDF-15047.wav");
	Damage = AEAudioCreateGroup();
	HeroSlash = AEAudioLoadMusic("Assets/Music/METAL-HIT_GEN-HDF-17085.wav");
	Interact = AEAudioLoadMusic("Assets/Music/SWITCH-LEVER_GEN-HDF-22196.wav");
	InteractGroup = AEAudioCreateGroup();
	
	Movement = AEAudioLoadMusic("Assets/Music/FOOTSTEPS-OUTDOOR_GEN-HDF-12363.mp3");
	MovementGroup = AEAudioCreateGroup();
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the World state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_World_Init(void) {

	PauseObj = staticObjInstCreate(TYPE_PAUSE, 1, nullptr, 0);
	StartScreenbj = staticObjInstCreate(TYPE_START, 1, nullptr, 0);

	AEVec2* pos = nullptr;
	int num;

	utilities::loadObjs(Gates, gatesNum, "worldGates.txt");

	// =====================================
	//	Initialize map textures
	// =====================================

	utilities::importMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureWorld.txt");

	// =====================================
	//	Initialize map binary
	// =====================================

	utilities::importMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *binaryMap, "binaryWorld.txt");

	mapEditorObj = staticObjInstCreate(TYPE_MAP, 0, nullptr, 0);

	MAX_MOBS = 14;

	// =====================================
	//	Initialize objects for new game
	// =====================================
	if (loadState == false) {
		//Initialise Player
		AEVec2 PlayerPos = { 12,-8 };
		//AEVec2 PlayerPos = { 106, -25 };
		Player = gameObjInstCreate(TYPE_CHARACTER, 1, &PlayerPos, 0, 0);

		Backpack.Potion = 0;
		Backpack.Key = 0;

		Player->health = 3;
		Player->damage = 1;


		//Initialise Levers in level
		utilities::loadObjs(pos, levNum, "worldLevers.txt");

		for (int i = 0; i < levNum; i++) {
			Levers[i] = staticObjInstCreate(TYPE_LEVERS, 1, &pos[i], 0);
		}

		utilities::unloadObjs(pos);

		//Initialise enemy in level
		utilities::loadObjs(pos, CURRENT_MOBS, "worldEnemy.txt");
		MAX_MOBS = CURRENT_MOBS;
		for (int i = 0; i < CURRENT_MOBS; i++) {
			GameObjInst* enemy = gameObjInstCreate(TYPE_ENEMY, 1, &pos[i], 0, 0);
		}
		utilities::unloadObjs(pos);

		//Initialise chest in level
		utilities::loadObjs(pos, chestNum, "worldChest.txt");
		for (int i = 0; i < chestNum; i++)
		{
			Chest[i] = staticObjInstCreate(TYPE_CHEST, 1, &pos[i], 0);
		}

		utilities::unloadObjs(pos);
	}

	// =====================================
	//	Initialize objects for loaded game
	// =====================================
	if (loadState == true) {
		loadData(data);
		Player->damage = 1;
		// Changing fence textures & binary collision depending on
		// lever texture
		for (int lev = 0; lev < levNum; lev++) {
				//Switch lever to face down
				if (Levers[lev]->dirCurr != 0)
				//Remove gates: Change texture & Binary map
				utilities::unlockGate(lev, *MapObjInstList, *binaryMap, Gates, MAP_CELL_HEIGHT);
		}
	}

	// Initialise Towers
	utilities::loadObjs(pos, num, "worldTowers.txt");

	float towerRot[] = {
		TOWER_DOWN,
		TOWER_DOWN
	};

	for (int i = 0; i < num; i++) {
		staticObjInst* jInst = staticObjInstCreate(TYPE_TOWER, 1, &pos[i], towerRot[i]);
		binaryMap[(int)pos[i].x][(int)-pos[i].y] = 1;
	}
	utilities::unloadObjs(pos);

	// Initialise fading Spikes
	utilities::loadObjs(pos, num, "worldSpikes.txt");
	for (int i = 0; i < num; i++) {
		staticObjInst* jInst = staticObjInstCreate(TYPE_SPIKE, 1, &pos[i], 0);
	}
	utilities::unloadObjs(pos);

	// Initialise non Spikes
	utilities::loadObjs(pos, num, "worldSpikes_Nonfade.txt");
	for (int i = 0; i < num; i++) {
		staticObjInst* jInst = staticObjInstCreate(TYPE_SPIKE_NONFADE, 1, &pos[i], 0);
	}
	utilities::unloadObjs(pos);

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
	
	//Initialise player health.
	for (int i = 0; i < 3; i++) {
		Health[i] = staticObjInstCreate(TYPE_HEALTH, 0.75, nullptr, 0);
	}

	if (levelCleared[colosseum] == true) {
		MapObjInstList[102][28] = TEXTURE_FENCE;
		binaryMap[102][28] = 1;
		MapObjInstList[103][28] = TEXTURE_FENCE;
		binaryMap[103][28] = 1;
	}
	if (levelCleared[maze] == true) {
		MapObjInstList[114][26] = TEXTURE_FENCE;
		binaryMap[114][26] = 1;
		MapObjInstList[114][27] = TEXTURE_FENCE;
		binaryMap[114][27] = 1;
	}
	if (levelCleared[tower] == true) {
		MapObjInstList[109][28] = TEXTURE_FENCE;
		binaryMap[109][28] = 1;
		MapObjInstList[110][28] = TEXTURE_FENCE;
		binaryMap[110][28] = 1;
	}
	ParticleSystemInit();
	playerHitTime = 0;

	levelstart = true;
	pause = true;
	cycle = 0;
	directionShown = false;
	directionDraw = false;
}
/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the World state.
*/
/******************************************************************************/

void GS_World_Update(void) {

	
	if (directionShown == false && Player->posCurr.x > 93) {
		pause = true;
		directionDraw = true;

		if (AEInputCheckTriggered(AEVK_ESCAPE)) {
			directionShown = true;
			directionDraw = false;
		}
	}

	if (AEInputCheckTriggered(AEVK_ESCAPE) && cycle == 0) {
		pause = !pause;
		levelstart = false;
	}

	if (pause == true) {
		if (AEInputCheckTriggered(AEVK_H) && cycle == 0) {
			cycle = 1;
		}
		if (cycle != 0 && AEInputCheckTriggered(AEVK_RIGHT)) {
			cycle++;
		}
		if (cycle == 4) {
			cycle = 0;
		}
		PauseObj->pObject->pTexture = textureList[4 + cycle];

		if (cycle == 0) {
			if (AEInputCheckReleased(AEVK_LBUTTON)) {
				if (utilities::rectbuttonClicked_AlignCtr(800.f, 445.f, 245.f, 85.f) == 1)//width 245 height 85
				{
					pause = false;
				}

				if (utilities::rectbuttonClicked_AlignCtr(800.f, 585.f, 245.f, 85.f) == 1)//width 245 height 85
				{
					gGameStateNext = GS_MAINMENU;
				}
			}
		}

	}
	if (pause == false) {

		// Normalising mouse to 0,0 at the center
		s32 mouseIntX, mouseIntY;
		AEInputGetCursorPosition(&mouseIntX, &mouseIntY);
		mouseX = (float)(mouseIntX - AEGetWindowWidth() / 2) / SPRITE_SCALE;
		mouseY = (float)(-mouseIntY + AEGetWindowHeight() / 2) / SPRITE_SCALE;



		float angleMousetoPlayer = utilities::getAngle(Player->posCurr.x, Player->posCurr.y, mouseX + Player->posCurr.x, mouseY + Player->posCurr.y);
		if (mouseY + camY > Player->posCurr.y) {
			angleMousetoPlayer = -angleMousetoPlayer;
		}


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

		if (AEInputCheckTriggered(AEVK_N)) {
			saveGame(data, sGameObjInstList, sStaticObjInstList, GAME_OBJ_INST_NUM_MAX, STATIC_OBJ_INST_NUM_MAX);
		}

		Player->playerStand();

		if (AEInputCheckCurr(AEVK_W) || AEInputCheckCurr(AEVK_UP) || AEInputCheckCurr(AEVK_S) || AEInputCheckCurr(AEVK_DOWN)
			|| AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT) || AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT)) {
			Player->playerWalk(walkCD);
		}
		else {
			Player->TextureMap = TEXTURE_PLAYER;
			AEAudioStopGroup(MovementGroup);
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
					AEAudioPlay(Interact, InteractGroup, 0.3, 1, 0);
				}
			}


			for (int i = 0; i < chestNum; i++)
			{
				//Interaction with Chest
				if (Player->calculateDistance(*Chest[i]) < 1 && Chest[i]->TextureMap.x != 8)
				{
					AEAudioPlay(Interact, InteractGroup, 0.3, 0.5, 0);
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
			utilities::exportMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureWorld.txt");

			utilities::exportMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "binaryWorld.txt");
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
			if (Player->calculateDistance(*pInst) < pickUpRange)
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
			if (pInst->pObject->type != TYPE_SLASH && pInst->pObject->type != TYPE_SPIKE && pInst->pObject->type != TYPE_SPIKE_NONFADE) {
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

			if (pInst->pObject->type == TYPE_BULLET) {
				int flag = CheckInstanceBinaryMapCollision(pInst->posCurr.x, -pInst->posCurr.y, binaryMap, pInst->scale, pInst->scale);
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

		}


		int flag = CheckInstanceBinaryMapCollision(Player->posCurr.x, -Player->posCurr.y, binaryMap);

		snapCollision(*Player, flag);

		for (int i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++) {
			staticObjInst* pInst = sStaticObjInstList + i;
			if (pInst->flag != 1 || (pInst->pObject->type != TYPE_SPIKE && pInst->pObject->type != TYPE_SPIKE_NONFADE)) {
				continue;
			}

			if (pInst->pObject->type == TYPE_SPIKE)
			{
				pInst->spikeUpdate(); // Updates alpha of spikes
			}

			if (Player->calculateDistance(*pInst) <= 1 && (pInst->Alpha == 0) && playerHitTime == 0) {

				Player->deducthealth();
				playerHitTime = DAMAGE_COODLDOWN_t;
			}

		}


		//Change stage conditions
		if (utilities::inRange(Player, WarpPts[0], WarpPts[1])) {
			gGameStateNext = GS_COLOSSEUM;
			Player->posCurr = { 106, -22 };
			saveGame(data, sGameObjInstList, sStaticObjInstList, GAME_OBJ_INST_NUM_MAX, STATIC_OBJ_INST_NUM_MAX);
		}
		if (utilities::inRange(Player, WarpPts[2], WarpPts[3])) {
				gGameStateNext = GS_TOWER;
				Player->posCurr = { 106, -22 };
				saveGame(data, sGameObjInstList, sStaticObjInstList, GAME_OBJ_INST_NUM_MAX, STATIC_OBJ_INST_NUM_MAX);
		}
		if (utilities::inRange(Player, WarpPts[4], WarpPts[5])) {
				gGameStateNext = GS_MAZE;
				Player->posCurr = { 106, -22 };
				saveGame(data, sGameObjInstList, sStaticObjInstList, GAME_OBJ_INST_NUM_MAX, STATIC_OBJ_INST_NUM_MAX);
		}
		if (utilities::inRange(Player, WarpPts[6], WarpPts[7])) {
				gGameStateNext = GS_BOSSLEVEL;
				Player->posCurr = { 106, -22 };
				saveGame(data, sGameObjInstList, sStaticObjInstList, GAME_OBJ_INST_NUM_MAX, STATIC_OBJ_INST_NUM_MAX);
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

		Player->dustParticles();

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
void GS_World_Draw(void) {


	if (pause == true) {

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
			AEMtx33Trans(&trans, 0, 0);
			AEMtx33Scale(&scale, 1600, 900);
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

		if (directionDraw) {
			AEMtx33 rot, scale, trans, transform;
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxTextureSet(textureList[8], 0, 0);

			AEMtx33Rot(&rot, 0);
			AEMtx33Trans(&trans, camX * SPRITE_SCALE, camY * SPRITE_SCALE);
			AEMtx33Scale(&scale, 1600, 900);
			AEMtx33Concat(&transform, &rot, &scale);
			AEMtx33Concat(&transform, &trans, &transform);

			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(meshList[1], AE_GFX_MDM_TRIANGLES);
		}


	}
	else if (pause == false) {
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

			AEGfxSetTintColor(pInst->damagetint.red, pInst->damagetint.green, pInst->damagetint.blue, 1.0f);
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
	This function frees all the instances created for the World level.
*/
/******************************************************************************/
void GS_World_Free(void) {
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

	ParticleSystemFree();
}

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	World level.
*/
/******************************************************************************/
void GS_World_Unload(void) {
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (AEGfxVertexList* Mesh : meshList) {
		AEGfxMeshFree(Mesh);
	}

	for (AEGfxTexture* texture : textureList) {
		AEGfxTextureUnload(texture);
	}

	AEGfxSetCamPosition(0, 0);

	meshList.clear();
	textureList.clear();


	ParticleSystemUnload();
}

// ---------------------------------------------------------------------------



void saveGame(saveData data, GameObjInst* gameObjList, staticObjInst* staticObjList, int gameObjMax, int staticObjMax) {
	// Put data into struct
	for (int i = 0; i < gameObjMax; i++) {
		GameObjInst* pInst = gameObjList + i;

		if (pInst->flag == 0) {
			continue;
		}
		if (pInst->pObject->type == TYPE_CHARACTER) {
			data.playerHealth = pInst->health;
			data.playerPosition = pInst->posCurr;
		}

		if (pInst->pObject->type == TYPE_ENEMY) {
			data.mobsNum++;
		}
	}

	for (int i = 0; i < staticObjMax; i++) {
		staticObjInst* pInst = staticObjList + i;
		if (pInst->flag == 0) {
			continue;
		}
		if (pInst->pObject->type == TYPE_CHEST) {
			data.chestNum++;
		}

		if (pInst->pObject->type == TYPE_LEVERS) {
			data.leverNum++;
		}
	}
	GameObjInst* Mobs = new GameObjInst[data.mobsNum];
	staticObjInst* Chests = new staticObjInst[data.chestNum];
	staticObjInst* Levers = new staticObjInst[data.leverNum];

	int k = 0, j = 0;
	for (int i = 0; i < gameObjMax; i++) {
		GameObjInst* pInst = gameObjList + i;
		if (pInst->flag == 0) {
			continue;
		}
		if (pInst->pObject->type == TYPE_ENEMY) {
			Mobs[k].health = pInst->health;
			Mobs[k].posCurr = pInst->posCurr;
			k++;
		}
	}

	k = 0;

	for (int i = 0; i < staticObjMax; i++) {
		staticObjInst* pInst = staticObjList + i;
		if (pInst->flag == 0) {
			continue;
		}
		if (pInst->pObject->type == TYPE_CHEST) {
			Chests[k].posCurr = pInst->posCurr;
			Chests[k].TextureMap = pInst->TextureMap;
			k++;
		}

		if (pInst->pObject->type == TYPE_LEVERS) {
			Levers[j].posCurr = pInst->posCurr;
			Levers[j].TextureMap = pInst->TextureMap;
			j++;
		}
	}

	std::ofstream saveText{ "Assets/save.txt" };

	saveText << data.playerHealth << std::endl;
	saveText << Backpack.Key << std::endl;
	saveText << Backpack.Potion << std::endl;
	saveText << data.playerPosition.x << std::endl;
	saveText << data.playerPosition.y << std::endl;


	saveText << data.mobsNum << std::endl;
	if (data.mobsNum != 0) {
		for (int i = 0; i < data.mobsNum; i++) {
			saveText << Mobs[i].posCurr.x << std::endl;
			saveText << Mobs[i].posCurr.y << std::endl;
			saveText << Mobs[i].health << std::endl;
		}
	}

	saveText <<data.chestNum << std::endl;
	if (data.chestNum != 0) {
		for (int i = 0; i < data.chestNum; i++) {
			saveText << Chests[i].posCurr.x << std::endl;
			saveText << Chests[i].posCurr.y << std::endl;
			saveText << Chests[i].TextureMap.x << std::endl;
			saveText << Chests[i].TextureMap.y << std::endl;
		}
	}

	saveText << data.leverNum << std::endl;
	if (data.leverNum != 0) {
		for (int i = 0; i < data.leverNum; i++) {
			saveText << Levers[i].posCurr.x << std::endl;
			saveText << Levers[i].posCurr.y << std::endl;
			saveText << Levers[i].TextureMap.x << std::endl;
			saveText << Levers[i].TextureMap.y << std::endl;
		}
	}

	//for (int i = 0; i < 4; i++) {
	//	saveText << data.puzzleCompleted[i] << std::endl;
	//}

	//saveText << data.elapsedTime << std::endl;

	delete[] Mobs;
	delete[] Chests;
	delete[] Levers;
}

void loadData(saveData data) {
	std::ifstream saveText{ "Assets/save.txt" };

	saveText >> data.playerHealth;
	saveText >> Backpack.Key;
	saveText >> Backpack.Potion;
	saveText >> data.playerPosition.x;
	saveText >> data.playerPosition.y;

	AEVec2 PlayerPos = { data.playerPosition.x,data.playerPosition.y};
	Player = gameObjInstCreate(TYPE_CHARACTER, 1, &PlayerPos, 0, 0);
	Player->TextureMap = TEXTURE_PLAYER;

	for (int i = 0; i < Player->health; i++) {
		Health[i] = staticObjInstCreate(TYPE_HEALTH, 0.75, nullptr, 0);
		Health[i]->TextureMap = TEXTURE_FULLHEART;
	}

	saveText >> data.mobsNum;
	for (int i = 0; i < data.mobsNum; i++) {
		//Create mob objs
		AEVec2 pos = { 0,0 };
		saveText >> pos.x;
		saveText >> pos.y;
		GameObjInst* pInst = gameObjInstCreate(TYPE_ENEMY, 1, &pos, nullptr, 0);
		saveText >> pInst->health;
	}
	CURRENT_MOBS = data.mobsNum;

	saveText >> data.chestNum;
	for (int i = 0; i < data.chestNum; i++) {
		AEVec2 pos = { 0,0 };
		AEVec2 tex = { 0,0 };
		saveText >> pos.x;
		saveText >> pos.y;
		saveText >> tex.x;
		saveText >> tex.y;
		Chest[i] = staticObjInstCreate(TYPE_CHEST, 1, &pos, 0);
		Chest[i]->TextureMap = tex;
	}
	chestNum = data.chestNum;

	saveText >> data.leverNum;
	for (int i = 0; i < data.leverNum; i++) {
		AEVec2 pos = { 0,0 };
		AEVec2 tex = { 0,0 };
		saveText >> pos.x;
		saveText >> pos.y;
		saveText >> tex.x;
		saveText >> tex.y;
		Levers[i] = staticObjInstCreate(TYPE_LEVERS, 1, &pos, 0);
		Levers[i]->TextureMap = tex;
	}
	levNum = data.leverNum;
}


