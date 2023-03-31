/******************************************************************************/
/*!
\file		Bosslevel.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 02, 2023
\brief		This source file contains the functions for the level of Boss Level.

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

static const float			darkRoom_duration = 3.f;
static const float			bossCircle_duration = 0.5f;
static const float			bossAttackRange = 1.0f;
static const float			bossSearchRange = 1.5f;


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
static staticObjInst* MenuObj[3];										// Pointer to each enemy object instance
static staticObjInst* NumObj[3];
static Inventory Backpack;
static GameObj hpbar;

static staticObjInst* PauseObj;
static staticObjInst* StartScreenbj;

static int dark;
static float darkTimer;
static bool pause;
static bool levelstart;
static int cycle;

static int levelclearedNum;

static std::vector<int> stageList;
static std::vector<AEGfxTexture*> textureList;
static std::vector<AEGfxVertexList*> meshList;

static float playerHitTime;
static staticObjInst* RefBox;

static AEMtx33 hpbartransform;

static bosshp boss;

static AEAudio BossBGM;
static AEAudioGroup BossGroup;

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

	GameObj* Character = 0, * Item = 0, * Map = 0, * Slash = 0,
		*RefLine = 0, *Health = 0, *Enemy = 0, *Boss = 0, *Key = 0,
		*Bullet = 0, *BossCircle = 0, *BossCircleAttack = 0, *BossSlash=0,
		*Pause = 0, *Start = 0;

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

	// Mesh for hpbar - 2
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x88880808, 0.0f, 0.0f,
		0.5f, -0.5f, 0x88880808, 0.0f, 0.0f,
		0.5f, 0.5f, 0x88880808, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, -0.5f, 0x88880808, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x88880808, 0.0f, 0.0f,
		0.5f, 0.5f, 0x88880808, 0.0f, 0.0f);
	meshList.push_back(AEGfxMeshEnd());

	// Mesh for dark room - 3
	AEGfxMeshStart();

	AEGfxTriAdd(80.0f, -45.f, 0xFFFF00FF, 1.0f, 1.0f,
		-80.0f, -45.f, 0xFFFFFF00, 0.0f, 1.0f,
		80.0f, 45.f, 0xFF00FFFF, 1.0f, 0.0f);

	AEGfxTriAdd(-80.0f, -45.f, 0xFFFFFFFF, 0.0f, 1.0f,
		-80.0f, 45.f, 0xFFFFFFFF, 0.0f, 0.0f,
		80.0f, 45.f, 0xFFFFFFFF, 1.0f, 0.0f);
	meshList.push_back(AEGfxMeshEnd());

	//Mesh Alias
	AEGfxVertexList*& spriteMesh = meshList[0];
	AEGfxVertexList*& fullSizeMesh = meshList[1];
	AEGfxVertexList*& healthBarMesh = meshList[2];
	AEGfxVertexList*& darkRoomMesh = meshList[3];


	//Load Textures
	textureList.push_back(AEGfxTextureLoad("Assets/slash.png")); // 0
	textureList.push_back(AEGfxTextureLoad("Assets/Tilemap/RefBox.png")); // 1
	textureList.push_back(AEGfxTextureLoad("Assets/Tilemap/tilemap_packed.png")); // 2
	textureList.push_back(AEGfxTextureLoad("Assets/Darkroom.png")); // 3
	textureList.push_back(AEGfxTextureLoad("Assets/PauseScreen.png")); // 4
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_1.png")); //5
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_2.png")); //6
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_3.png")); //7
	textureList.push_back(AEGfxTextureLoad("Assets/Boss_Obj.png")); // 8

	//Texture Alias
	AEGfxTexture*& slashTex = textureList[0];
	AEGfxTexture*& refBox = textureList[1];
	AEGfxTexture*& spriteSheet = textureList[2];
	AEGfxTexture*& darkRoom = textureList[3];
	AEGfxTexture*& PauseTex = textureList[4];
	AEGfxTexture*& startTex = textureList[8];
	
	
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
	utilities::loadMeshNTexture(Boss, spriteMesh, spriteSheet, TYPE_BOSS);
	utilities::loadMeshNTexture(BossCircle, spriteMesh, spriteSheet, TYPE_BOSSCIRCLE);
	utilities::loadMeshNTexture(BossCircleAttack, spriteMesh, spriteSheet, TYPE_BOSSCIRCLEATTACK);
	utilities::loadMeshNTexture(BossSlash, fullSizeMesh, slashTex, TYPE_BOSSSLASH);
	utilities::loadMeshNTexture(Pause, fullSizeMesh, PauseTex, TYPE_PAUSE);
	utilities::loadMeshNTexture(Start, fullSizeMesh, startTex, TYPE_START);

	

	ParticleSystemLoad();

	BossBGM = AEAudioLoadMusic("Assets/Music/DEEP WATERS - DanceTechno MSCDNT2_57.wav");
	BossGroup = AEAudioCreateGroup();
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Boss Level state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_BossLevel_Init(void) {
	srand(static_cast<unsigned int>(time(NULL))); //Random seed
	PauseObj = staticObjInstCreate(TYPE_PAUSE, 1, nullptr, 0);
	StartScreenbj = staticObjInstCreate(TYPE_START, 1, nullptr, 0);


	dark = notActivated;
	darkTimer = 0; // Reset dark timer
	playerHitTime = 0; // Reset hit cooldown

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

		std::ifstream ifs{ "Assets/save.txt" };
		ifs >> Player->health;
		ifs >> Backpack.Key; //set to player number of current potion
		ifs >> Backpack.Potion; //set to player number of current key
		ifs.close();

		Player->damage = 1;

		//Initialise player health.
		for (int i = 0; i < MAX_PLAYER_HEALTH; i++) {
			Health[i] = staticObjInstCreate(TYPE_HEALTH, 0.75f, nullptr, 0);
		}


	//Init pathfinding nodes
	NodesInit(*binaryMap, MAP_CELL_WIDTH, MAP_CELL_HEIGHT);

	//init Boss
	AEVec2 BossPos = { 17,-10 }; // TXT
	Boss = gameObjInstCreate(TYPE_BOSS, 1, &BossPos, 0, 0);

	// Initialise camera pos
	camX = 10, camY = -10;

	// =====================================
	//	Initialize UI objects
	// =====================================

	MenuObj[0] = staticObjInstCreate(TYPE_ITEMS, 1, nullptr, 0); // Potions
	MenuObj[1] = staticObjInstCreate(TYPE_KEY, 1, nullptr, 0); // Keys

	NumObj[0] = staticObjInstCreate(TYPE_ITEMS, 1, nullptr, 0); // Potions
	NumObj[1] = staticObjInstCreate(TYPE_KEY, 1, nullptr, 0); // Keys

	boss.maxhp = Boss->health;
	boss.currenthp = &Boss->health;

	ParticleSystemInit();


	for (int i = 0; i < 3; i++) {
		if (levelCleared[i] == false) {
			stageList.push_back(i);
		}
	}
	levelstart = 1;
	pause = 0;
	cycle = 0;

	AEAudioPlay(BossBGM, BossGroup, 0.2f, 1, 1);
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Boss Level state.
*/
/******************************************************************************/
void GS_BossLevel_Update(void) {
	if (AEInputCheckTriggered(AEVK_ESCAPE) && cycle == 0) {
		pause = !pause;
		levelstart = 0;
	}

	if (pause == 0) {
		if (AEInputCheckTriggered(AEVK_H) && cycle == 0) {
			cycle = 1;
		}
		if (cycle != 0 && AEInputCheckTriggered(AEVK_RIGHT)) {
			AEAudioStopGroup(BossGroup);
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
					AEAudioStopGroup(BossGroup);
					pause = 1;
				}

				if (utilities::rectbuttonClicked_AlignCtr(800.f, 585.f, 245.f, 85.f) == 1)//width 245 height 85
				{
					AEAudioStopGroup(BossGroup);
					gGameStateNext = GS_MAINMENU;
				}
			}
		}
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
			AEAudioStopGroup(BossGroup);
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

			if (pInst->pObject->type == TYPE_ENEMY || pInst->pObject->type == TYPE_BOSS) {

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
				if (pInst->pObject->type == TYPE_BOSS) {
					for (int j = 0; j < STATIC_OBJ_INST_NUM_MAX; j++) {
						staticObjInst* jInst = sStaticObjInstList + j;
						if (jInst->flag != FLAG_ACTIVE || jInst->pObject->type != TYPE_SLASH) {
							continue;
						}

						if (pInst->calculateDistance(*jInst) < slashRange
							&& jInst->Alpha == 0) {
							pInst->deducthealth(Player->damage);
							if (pInst->health <= 0) {
								AEAudioStopGroup(BossGroup);
								gGameStateNext = GS_WIN; // Win condition
							}
						}
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
		if (dark == 0) {
			darkTimer += g_dt;
		}

		if (darkTimer > darkRoom_duration) {
			darkTimer = 0;
			dark = notActivated;
		}

		for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
		{
			staticObjInst* pInst = sStaticObjInstList + i;
			if (pInst->flag != FLAG_ACTIVE) {
				continue;
			}

			if (pInst->pObject->type == TYPE_SLASH || pInst->pObject->type == TYPE_BOSSSLASH) {
				pInst->playerSlashUpdate();
			}

			if (pInst->pObject->type == TYPE_BOSSCIRCLE || pInst->pObject->type == TYPE_BOSSCIRCLEATTACK) {
				pInst->timetracker += g_dt;
				if (pInst->timetracker > bossCircle_duration) {
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
				}
			}

			if (pInst->pObject->type == TYPE_CHARACTER) {
				pInst->timetracker += g_dt;
			}
		}

		utilities::snapCamPos(Player->posCurr, camX, camY, MAP_CELL_WIDTH, 18);
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

		utilities::bossBarTransMatrix(boss, hpbartransform);

		//Player->dustParticles();

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
void GS_BossLevel_Draw(void) {
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


	}
	else if (pause) {

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

		if (dark == Activated) {
			AEGfxSetTransparency(1.0f);
			AEGfxTextureSet(textureList[3], 0, 0);
			// Create a scale matrix that scales by 100 x and y
			AEMtx33 lscale = { 0 };
			AEMtx33Scale(&lscale, 20, 20);
			// Create a rotation matrix that rotates by 45 degrees
			AEMtx33 lrotate = { 0, };

			AEMtx33Rot(&lrotate, 0);

			// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis
			AEMtx33 ltranslate = { 0 };
			AEMtx33Trans(&ltranslate, Player->posCurr.x * SPRITE_SCALE, Player->posCurr.y * SPRITE_SCALE);
			// Concat the matrices (TRS) 
			AEMtx33 ltransform = { 0 };
			AEMtx33Concat(&ltransform, &lrotate, &lscale);
			AEMtx33Concat(&ltransform, &ltranslate, &ltransform);
			// Choose the transform to use 
			AEGfxSetTransform(ltransform.m);
			// Actually drawing the mesh
			AEGfxMeshDraw(meshList[3], AE_GFX_MDM_TRIANGLES);
		}

		//drawing of health bar
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTintColor(0.7f, 0.7f, 0.7f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxSetTransform(hpbartransform.m);
		AEGfxMeshDraw(meshList[2], AE_GFX_MDM_TRIANGLES);

		if (state == Activated)
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

	for (AEGfxVertexList* Mesh : meshList) {
		AEGfxMeshFree(Mesh);
	}

	for (AEGfxTexture* texture : textureList) {
		AEGfxTextureUnload(texture);
	}


	//BUGGY CODE, IF UANBLE TO LOAD, CANNOT USE DEBUGGING MODE
	AEGfxSetCamPosition(0, 0);
	meshList.clear();
	textureList.clear();
	stageList.clear();

	ParticleSystemUnload();
}

/******************************************************************
function definition for boss finite state machine
*******************************************************************/
void BossStateMachine(GameObjInst* pInst)
{
	AEVec2 enemyPos1 = Player->posCurr;
	AEVec2 enemyPos2 = Player->posCurr;
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

			if (static_cast<int>(pInst->timetracker * 10) % static_cast<int>(challengeATKREFRESH * 10) == 0) {
				//CHALLENGE ATTACK
				// random between 0, 1 and 2
				pInst->innerState = INNER_STATE_ON_EXIT;
				if (Backpack.Key != 0) {
					int random = stageList[rand() % 3-Backpack.Key];
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
			}

			if (static_cast<int>(pInst->timetracker * 10) % static_cast<int>(aoeREFRESH*10) == 0) {
				//AOE ATTACK
				pInst->stateFlag = STATE_AOE;
				pInst->innerState = INNER_STATE_ON_EXIT; 
				
				break;
			}

			if (pInst->calculateDistance(*Player) < bossSearchRange) { // If found player, attack player
				pInst->innerState = INNER_STATE_ON_EXIT;
				pInst->stateFlag = STATE_BASIC;
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
				staticObjInst* bossSlash = staticObjInstCreate(TYPE_BOSSSLASH, 2, &slashPosition, PI + angle);
				if (Player->calculateDistance(*bossSlash) < bossAttackRange) {
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
			pInst->innerState = INNER_STATE_ON_UPDATE;
			break;
		case INNER_STATE_ON_UPDATE:

			std::cout << "updating state 3" << std::endl;
			//Spawn enemies
			// Stay still for awhile


			pInst->timeCD += g_dt;

			if (pInst->timeCD > 2.0f) {
				pInst->timeCD = 0;
				do {
					enemyPos1 = { 3.5f + AERandFloat() * 17 ,-6 - AERandFloat() * 8 };
					enemyPos2 = { 3.5f + AERandFloat() * 17 ,-6 - AERandFloat() * 8 };
				} while (static_cast<int> (enemyPos1.x) == Player->posCurr.x || static_cast<int> (enemyPos2.y) == Player->posCurr.y);

				GameObjInst* first = gameObjInstCreate(TYPE_ENEMY, 1, &enemyPos1, nullptr, 0);
				GameObjInst* second = gameObjInstCreate(TYPE_ENEMY, 1, &enemyPos2, nullptr, 0);
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
			pInst->innerState = INNER_STATE_ON_UPDATE;
			break;
		case INNER_STATE_ON_UPDATE:
			pInst->timeCD += g_dt;
			std::cout << "updating state 4" << std::endl;
			//Spawn Bullets
			AEVec2 dirVec = Player->posCurr - Boss->posCurr;
			AEVec2 normDir;
			AEVec2Normalize(&normDir, &dirVec);
			switch (stage) {
			case 0:
				gameObjInstCreate(TYPE_BULLET, 0.5, &pInst->posCurr, &normDir, 0);
				stage = 1;
				break;
			case 1:
				if (pInst->timeCD == 0.5f) {
				gameObjInstCreate(TYPE_BULLET, 0.5, &pInst->posCurr, &normDir, 0);
				stage = 2;
				pInst->timeCD = 0;
				}
				break;
			case 2:
				if (pInst->timeCD == 0.5f) {
					gameObjInstCreate(TYPE_BULLET, 0.5, &pInst->posCurr, &normDir, 0);
					stage = 3;
					pInst->timeCD = 0;
				}
				break;
			case 3:
				if (pInst->timeCD > 2.0f) {
					pInst->timeCD = 0;
					pInst->stateFlag = STATE_PATROL;
					pInst->innerState = INNER_STATE_ON_EXIT;
					stage = 0;
				}
				break;
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
			pInst->innerState = INNER_STATE_ON_UPDATE;

			break;
		case INNER_STATE_ON_UPDATE:

			std::cout << "updating state 5" << std::endl;
			dark = Activated;
			// Stay still for awhile
			pInst->timeCD += g_dt;
			if (pInst->timeCD > 1.0f) {
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



