/******************************************************************************/
/*!
\file		Bosslevel.cpp
\author 	Javier Low Zhi Liang
\par    	email: zhiliangjavier.low@digipen.edu
\date   	February 02, 2023
\brief		This source file contains the functions for the level of Boss Level.

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
static const int			MAP_CELL_WIDTH = 24;				// Total number of cell widths
static const int			MAP_CELL_HEIGHT = 42;				// Total number of cell heights

static const AEVec2			mapMin{ 4.5, -7 };					// Minimal coordinate of map
static const AEVec2			mapRange{ 15, 6 };					// Range of available coordinates from min of map

static const int			maxLevelNum = 3;					// Total number of challenge levels

// Boss Attributes
static const float			darkRoom_duration = 6.f;			// Duration for dark room to exist
static const float			bossCircle_duration = 0.25f;		// Duration for boss circle to toggle
static const float			bossCircleSize = 4.0f;				// Size of boss circle attack
static const float			bossAttackRange = 1.0f;				// Range of boss attack
static const float			bossSearchRange = 1.5f;				// Range of boss to stop patrolling
static const float			bossStillDuration = 0.5f;			// Duration for boss to stand still when activating skill
static const float			aoeREFRESH = 3.0f;					// Time for AOE attack to be refreshed
static const float			challengeATKREFRESH = 5.f;			// Time for Challenges attack to be refreshed

static const AEVec2 playerStartPoint{ 4,-10 };					// Starting coordinate of new game player
static const AEVec2 BossStartPoint{ 17,-10 };					// Starting coordinate of new game player

// -----------------------------------------------------------------------------

static AEVec2				MapObjInstList[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];	// 2D array of each map tile object
static int					binaryMap[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];	// 2D array of binary collision mapping

// pointer to the objects
static GameObjInst* Player;								// Pointer to the "Player" game object instance
static GameObjInst* Boss;								// Pointer to the "Boss" game object instance
static staticObjInst* mapEditorObj;						// Pointer to the reference map editor object instance
static staticObjInst* Health[3];						// Pointer to the player health statc object instance
static staticObjInst* MenuObj[2];						// Pointer to each menu UI object
static staticObjInst* NumObj[2];						// Pointer to each number UI object
static staticObjInst* bossHeart;						// Pointer to bossHeart object
static staticObjInst* PauseObj;							// Pointer to Pause Obj
static staticObjInst* StartScreenbj;					// Pointer to start screen Obj

// State Variables
static bool dark;										// Dark room state
static bool	debugging;									// Debugging state
static bool	mapeditor;									// Map edtior state

// Time Variables
static float darkTimer;									// Stores time left for dark room state
static float playerHitTime;								// Stores time left for player's invulnerability upon attacking
static float slashCD;									// Stores time left before player can slash again
static float walkCD;									// Stores time left before player can move after slashing


// HP bar variables
static bosshp bossHP;									// Struct for more info on bosshp for drawing purpose
static AEMtx33 hpbartransform;							// Transformation matrix of bosshp bar
static GameObj hpbar;									// Global obj of boss hp bar

static std::vector<int> stageList;						// List of stages uncleared, which boss attack is available
static Inventory Backpack;								// Inventoy of player

static AEAudio BossBGM;
static AEAudioGroup BossGroup;

// ---------------------------------------------------------------------------

// Function for BossLevel.cpp Only

/******************************************************************/
/*!
\brief Boss State machine that toggles between states for boss 
		behavior
\param[in] pInst
		Pointer to the boss game obj inst
*/
/*******************************************************************/
void BossStateMachine(GameObjInst* pInst);

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
	// Setting initial numbers to 0
	sGameObjNum = 0;
	sGameObjInstNum = 0;
	sStaticObjInstNum = 0;
	Player = nullptr;

	// List of Unique Game Objs
	GameObj* Character = 0, * Item = 0, * Map = 0, * Slash = 0,
		*RefLine = 0, *Health = 0, *Enemy = 0, *Boss = 0, *Key = 0,
		*Bullet = 0, *BossCircle = 0, *BossCircleAttack = 0, *BossSlash=0,
		*Pause = 0, *Start = 0;

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

	// Mesh for hpbar - 2
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF880808, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFF880808, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFF880808, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF880808, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFF880808, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFF880808, 0.0f, 0.0f);
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

	// =====================================
	//	Load Textures
	// =====================================

	textureList.push_back(AEGfxTextureLoad("Assets/slash.png")); // 0
	textureList.push_back(AEGfxTextureLoad("Assets/Tilemap/RefBox.png")); // 1
	textureList.push_back(AEGfxTextureLoad("Assets/Tilemap/tilemap_packed.png")); // 2
	textureList.push_back(AEGfxTextureLoad("Assets/Darkroom.png")); // 3
	textureList.push_back(AEGfxTextureLoad("Assets/PauseScreen.png")); // 4
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_1.png")); //5
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_2.png")); //6
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_3.png")); //7
	textureList.push_back(AEGfxTextureLoad("Assets/Boss_Obj.png")); // 8
	textureList.push_back(AEGfxTextureLoad("Assets/slashBoss.png")); // 9

	//Texture Alias
	AEGfxTexture*& slashTex = textureList[0];
	AEGfxTexture*& refBox = textureList[1];
	AEGfxTexture*& spriteSheet = textureList[2];
	AEGfxTexture*& darkRoom = textureList[3];
	AEGfxTexture*& PauseTex = textureList[4];
	AEGfxTexture*& startTex = textureList[8];
	AEGfxTexture*& slashBosstex = textureList[9];

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
	utilities::loadMeshNTexture(Boss, spriteMesh, spriteSheet, TYPE_BOSS);
	utilities::loadMeshNTexture(BossCircle, spriteMesh, spriteSheet, TYPE_BOSSCIRCLE);
	utilities::loadMeshNTexture(BossCircleAttack, spriteMesh, spriteSheet, TYPE_BOSSCIRCLEATTACK);
	utilities::loadMeshNTexture(BossSlash, fullSizeMesh, slashBosstex, TYPE_BOSSSLASH);
	utilities::loadMeshNTexture(Pause, fullSizeMesh, PauseTex, TYPE_PAUSE);
	utilities::loadMeshNTexture(Start, fullSizeMesh, startTex, TYPE_START);

	ParticleSystemLoad();

	BossBGM = AEAudioLoadMusic("Assets/Music/DEEP WATERS - DanceTechno MSCDNT2_57.mp3");
	BossGroup = AEAudioCreateGroup();
	HeroDamaged = AEAudioLoadMusic("Assets/Music/HUMAN-GRUNT_GEN-HDF-15047.wav");
	Damage = AEAudioCreateGroup();
	HeroSlash = AEAudioLoadMusic("Assets/Music/METAL-HIT_GEN-HDF-17085.wav");
	Movement = AEAudioLoadMusic("Assets/Music/FOOTSTEPS-OUTDOOR_GEN-HDF-12363.mp3");
	MovementGroup = AEAudioCreateGroup();
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Boss Level state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_BossLevel_Init(void) {
	srand(static_cast<unsigned int>(time(NULL))); //Random seed for boss attack pattern

	//Create objects for pause and start screen
	PauseObj = staticObjInstCreate(TYPE_PAUSE, 1, nullptr, 0);
	StartScreenbj = staticObjInstCreate(TYPE_START, 1, nullptr, 0);

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
		AEVec2 PlayerPos = playerStartPoint;
		Player = gameObjInstCreate(TYPE_CHARACTER, 1, &PlayerPos, 0, 0);

		std::ifstream ifs{ "Assets/save.txt" };
		ifs >> Player->health;
		ifs >> Backpack.Key; //set to player number of current potion
		ifs >> Backpack.Potion; //set to player number of current key
		ifs.close();

		//Initialise player health.
		for (int i = 0; i < MAX_PLAYER_HEALTH; i++) {
			Health[i] = staticObjInstCreate(TYPE_HEALTH, 0.75f, nullptr, 0);
		}

	//Init pathfinding nodes
	NodesInit(*binaryMap, MAP_CELL_WIDTH, MAP_CELL_HEIGHT);

	//init Boss
	AEVec2 BossPos = BossStartPoint; // TXT
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

	// Initialise bosshp struct
	bossHP.maxhp = Boss->health;
	bossHP.currenthp = &Boss->health;
	// Initialise heart object for boss health bar
	bossHeart = staticObjInstCreate(TYPE_HEALTH, 1, nullptr, 0);

	ParticleSystemInit();

	// Initialise available attack list
	for (int i = 0; i < maxLevelNum; i++) {
		if (levelCleared[i] == false) {
			stageList.push_back(i);
		}
	}

	// Initialise in-game states
	levelstart = true;
	pause = true;
	dark = false;
	debugging = false;
	mapeditor = false;

	darkTimer = 0;
	playerHitTime = 0;
	slashCD = 0;
	walkCD = 0;
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
		//Debugging mode - Developer Use
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
			utilities::exportMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureBoss.txt");

			utilities::exportMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "binaryBoss.txt");
		}

		// ======================================================
		// update physics of all active game object instances
		//  -- Get the AABB bounding rectangle of every active instance:
		//		boundingRect_min = -(BOUNDING_RECT_SIZE/2.0f) * instance->scale + instance->pos
		//		boundingRect_max = +(BOUNDING_RECT_SIZE/2.0f) * instance->scale + instance->pos

		// Boss state machine
		BossStateMachine(Boss);

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

			if (pInst->pObject->type == TYPE_ENEMY || pInst->pObject->type == TYPE_BOSS) {

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
				if (pInst->pObject->type == TYPE_BOSS) {
					for (int j = 0; j < STATIC_OBJ_INST_NUM_MAX; j++) {
						staticObjInst* jInst = sStaticObjInstList + j;
						if (jInst->flag != FLAG_ACTIVE || jInst->pObject->type != TYPE_SLASH) {
							continue;
						}

						if (pInst->calculateDistance(*jInst) < slashRange
							&& jInst->Alpha == 0) {
							// Between Boss and Slashes
							pInst->deducthealth(Player->damage);
							if (pInst->health <= 0) {
								gGameStateNext = GS_WIN; // Win condition
							}
						}
					}
				}
			}

			if (pInst->pObject->type == TYPE_BULLET) {
				int flag = CheckInstanceBinaryMapCollision(pInst->posCurr.x, -pInst->posCurr.y, binaryMap, pInst->scale, pInst->scale);
				if (CollisionIntersection_RectRect(Player->boundingBox, Player->velCurr, pInst->boundingBox, pInst->velCurr)) {
					// Between Bullet and Player
					if (playerHitTime == 0) {
						Player->deducthealth();
						playerHitTime = DAMAGE_COODLDOWN_t;
						AEAudioPlay(HeroDamaged, Damage, 0.3f, 1, 0);
						gameObjInstDestroy(pInst);
					}
				}
				if (snapCollision(*pInst, flag)) {
					// Between Bullet and Wall
					gameObjInstDestroy(pInst);
				}
			}
		}



		int flag = CheckInstanceBinaryMapCollision(Player->posCurr.x, -Player->posCurr.y, binaryMap);
		// Binary collision for player
		snapCollision(*Player, flag);

		for (int i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++) {
			staticObjInst* pInst = sStaticObjInstList + i;
			if (pInst->flag != FLAG_ACTIVE || (pInst->pObject->type != TYPE_BOSSCIRCLEATTACK && pInst->pObject->type != TYPE_BOSSSLASH)) {
				continue;
			}

			// Between boss circle attack and player
			if (pInst->pObject->type == TYPE_BOSSCIRCLEATTACK) {
				if (Player->calculateDistance(*pInst) < bossCircleSize/2 && playerHitTime == 0) {
					playerHitTime = DAMAGE_COODLDOWN_t;
					Player->deducthealth();
					AEAudioPlay(HeroDamaged, Damage, 0.3f, 1, 0);
				}
			}

			if (pInst->pObject->type == TYPE_BOSSSLASH) {
				if (Player->calculateDistance(*pInst) < bossAttackRange && playerHitTime == 0) {
					playerHitTime = DAMAGE_COODLDOWN_t;
					Player->deducthealth();
					AEAudioPlay(HeroDamaged, Damage, 0.3f, 1, 0);
				}
			}

		}
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
		// Dark room timer
		if (dark == true) {
			darkTimer += g_dt;
		}

		if (darkTimer > darkRoom_duration) {
			darkTimer = 0;
			dark = false;
		}

		for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
		{
			staticObjInst* pInst = sStaticObjInstList + i;
			if (pInst->flag != FLAG_ACTIVE) {
				continue;
			}
			// Update slashes
			if (pInst->pObject->type == TYPE_SLASH || pInst->pObject->type == TYPE_BOSSSLASH) {
				pInst->playerSlashUpdate();
			}

			// Update boss circle attacks
			if (pInst->pObject->type == TYPE_BOSSCIRCLE || pInst->pObject->type == TYPE_BOSSCIRCLEATTACK) {
				pInst->timetracker += g_dt;
				if (pInst->timetracker > bossCircle_duration) {
					staticObjInstDestroy(pInst);
				}
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
				}
			}
			// Increment global time tracker for character
			if (pInst->pObject->type == TYPE_CHARACTER) {
				pInst->timetracker += g_dt;
			}
		}

		// Set Camera and the UI objects
		utilities::snapCamPos(Player->posCurr, camX, camY, MAP_CELL_WIDTH, 18);
		AEGfxSetCamPosition(static_cast<f32>(static_cast<int>(camX * (float)SPRITE_SCALE)), static_cast<f32>(static_cast<int> (camY * (float)SPRITE_SCALE)));
		utilities::updatePlayerUI(Health, MenuObj, NumObj, Backpack, Player->health, camX, camY);

		utilities::bossBarTransMatrix(bossHP, hpbartransform, *bossHeart, camX, camY);

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
void GS_BossLevel_Draw(void) {

	// Set modes for rendering
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
			AEMtx33Trans(&trans, 0, 0);
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


	}
	else if (pause == false) {

		// Drawing map tiles
		for (unsigned long i = 0; i < MAP_CELL_WIDTH; i++) {
			for (long j = 0; j < MAP_CELL_HEIGHT; j++) {
				AEVec2 Pos = { i + 0.5f, -j - 0.5f };

				if (utilities::checkWithinCam(Pos, camX, camY)) {
					continue;
				}
				AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

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
			// Only draw within viewport
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
			// Only draw within viewport
			if (utilities::checkWithinCam(pInst->posCurr, camX, camY)) {

				continue;
			}
			// for any sprite textures

				AEGfxTextureSet(pInst->pObject->pTexture,
					pInst->TextureMap.x * TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH,
					pInst->TextureMap.y * TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT);
			AEGfxSetTintColor(pInst->damagetint.red, pInst->damagetint.green, pInst->damagetint.blue, 1.0f);
			// Set the current object instance's transform matrix using "AEGfxSetTransform"
			AEGfxSetTransform(pInst->transform.m);
			// Draw the shape used by the current object instance using "AEGfxMeshDraw"
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

		if (dark == true) {
			AEMtx33 lscale, lrotate, ltranslate, ltransform;

			AEGfxSetTransparency(1.0f);
			AEGfxTextureSet(textureList[3], 0, 0);

			AEMtx33Scale(&lscale, 20, 20);
			AEMtx33Rot(&lrotate, 0);
			AEMtx33Trans(&ltranslate, Player->posCurr.x * SPRITE_SCALE, Player->posCurr.y* SPRITE_SCALE);
			// Concat the matrices (TRS) 
			AEMtx33Concat(&ltransform, &lrotate, &lscale);
			AEMtx33Concat(&ltransform, &ltranslate, &ltransform);

			// Choose the transform to use 
			AEGfxSetTransform(ltransform.m);
			// Actually drawing the mesh
			AEGfxMeshDraw(meshList[3], AE_GFX_MDM_TRIANGLES);

			// Draw menu UI on top of dark room
			for (int i = 0; i < MAX_PLAYER_HEALTH; i++) {
				AEGfxSetTransform(Health[i]->transform.m);
				AEGfxTextureSet(Health[i]->pObject->pTexture, Health[i]->TextureMap.x * TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH,
					Health[i]->TextureMap.y * TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT);
				AEGfxMeshDraw(Health[i]->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			}
			
			for (staticObjInst* i : NumObj) {
				AEGfxSetTransform(i->transform.m);
				AEGfxTextureSet(i->pObject->pTexture, i->TextureMap.x * TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH,
					i->TextureMap.y * TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT);
				AEGfxMeshDraw(i->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			}

			for (staticObjInst* i : MenuObj) {
				AEGfxSetTransform(i->transform.m);
				AEGfxTextureSet(i->pObject->pTexture, i->TextureMap.x * TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH,
					i->TextureMap.y * TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT);
				AEGfxMeshDraw(i->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

		//drawing of health bar
		AEGfxSetTransparency(1.0f);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetTransform(hpbartransform.m);
		AEGfxMeshDraw(meshList[2], AE_GFX_MDM_TRIANGLES);

		// Drawing of boss heart
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetTransform(bossHeart->transform.m);
		AEGfxTextureSet(bossHeart->pObject->pTexture, bossHeart->TextureMap.x* TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH, 
						bossHeart->TextureMap.y* TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT);
		AEGfxMeshDraw(bossHeart->pObject->pMesh, AE_GFX_MDM_TRIANGLES);

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

	// Stops all audio
	AEAudioStopGroup(BossGroup);
	AEAudioStopGroup(Damage);
	AEAudioStopGroup(MovementGroup);

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


	// Reset camera position
	AEGfxSetCamPosition(0, 0);

	// Clear all vectors
	meshList.clear();
	textureList.clear();
	stageList.clear();

	ParticleSystemUnload();
}


/******************************************************************/
/*!
\brief Boss State machine that toggles between states for boss
		behavior
\param[in] pInst
		Pointer to the boss game obj inst
*/
/*******************************************************************/
void BossStateMachine(GameObjInst* pInst)
{
	AEVec2 enemyPos1 = Player->posCurr;
	AEVec2 enemyPos2 = Player->posCurr;
	static AEVec2 playerPosition{ 0,0 };
	static int stage = 0;
	//states declared at GameObjs.h
	switch (pInst->state)
	{

	/**********************************************************************************
	
		Patrol state - Pathfinding for Boss until timer for AOE/Challenge attack

	***********************************************************************************/

	case STATE_PATROL:
		switch (pInst->innerState) { 
		case INNER_STATE_ON_ENTER: /////////////////////////////////////////////////

			std::cout << "entering state 0 " << std::endl;
			pInst->timetracker += g_dt;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			
			break; /////////////////////////////////////////////////////////////////

		case INNER_STATE_ON_UPDATE: ////////////////////////////////////////////////

			std::cout << "updating state 0" << std::endl;
			pInst->timetracker += g_dt;
			pInst->mobsPathFind(*Player);

			if (static_cast<int>(pInst->timetracker * 10) % static_cast<int>(challengeATKREFRESH * 10) == 0) {
				//CHALLENGE ATTACK
				// random between 0, 1 and 2
				pInst->innerState = INNER_STATE_ON_EXIT;
				if (Backpack.Key != maxLevelNum) {
					int random = stageList[rand() % maxLevelNum-Backpack.Key];
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

			break; /////////////////////////////////////////////////////////////////

		case INNER_STATE_ON_EXIT: //////////////////////////////////////////////////

			std::cout << "exiting state 0" << std::endl;
			pInst->timetracker += g_dt;
			pInst->state = pInst->stateFlag;
			pInst->innerState = INNER_STATE_ON_ENTER;
			
			break; /////////////////////////////////////////////////////////////////
		} break;


	/**********************************************************************************
	
		Basic Attack state - Slash at player

	***********************************************************************************/
	
	case STATE_BASIC:
		switch (pInst->innerState) {
		case INNER_STATE_ON_ENTER: ////////////////////////////////////////////////

			pInst->velCurr = { 0,0 };
			std::cout << "entering state 1" << std::endl;
			pInst->timetracker += g_dt;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			playerPosition = Player->posCurr;

			break; /////////////////////////////////////////////////////////////////

		case INNER_STATE_ON_UPDATE: ///////////////////////////////////////////////

			std::cout << "updating state 1" << std::endl;
			pInst->timetracker += g_dt;

			//stand still for 1 second
			pInst->timeCD += g_dt;
			if (pInst->timeCD > bossStillDuration) {
				pInst->timeCD = 0;

				float angle = utilities::getAngle(pInst->posCurr.x, pInst->posCurr.y, playerPosition.x, playerPosition.y);
				if (Player->posCurr.y > pInst->posCurr.y) {
					angle = -angle;
				}

				AEVec2 slashPosition = { pInst->posCurr.x -cos(angle) / 1.3f, pInst->posCurr.y -sin(angle) / 1.3f };
				staticObjInst* bossSlash = staticObjInstCreate(TYPE_BOSSSLASH, 2, &slashPosition, PI + angle);
				pInst->stateFlag = STATE_PATROL;
				pInst->innerState = INNER_STATE_ON_EXIT;
			}

			break; /////////////////////////////////////////////////////////////////

		case INNER_STATE_ON_EXIT:///////////////////////////////////////////////////
			
			std::cout << "exiting state 1" << std::endl;
			pInst->timetracker += g_dt;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->state = pInst->stateFlag;

			break; /////////////////////////////////////////////////////////////////
		}break;


	/**********************************************************************************
	
		AOE Attack state - Blink a circle at player position 3 times before detonating

	***********************************************************************************/
	case STATE_AOE:
		switch (pInst->innerState) {

		case INNER_STATE_ON_ENTER://///////////////////////////////////////////////
			
			pInst->velCurr = { 0,0 };
			std::cout << "entering state 2" << std::endl;
			playerPosition = Player->posCurr;
			pInst->innerState = INNER_STATE_ON_UPDATE;

			break;/////////////////////////////////////////////////////////////////

		case INNER_STATE_ON_UPDATE:////////////////////////////////////////////////
			
			std::cout << "updating state 2" << std::endl;
			//AOE Attack
			pInst->timeCD += g_dt;
			switch (stage) {
			case 0: // First Circle
					staticObjInstCreate(TYPE_BOSSCIRCLE, bossCircleSize, &playerPosition, 0);
					//Draw Circle
					pInst->timeCD = 0;
					stage = 1;
					break;
			case 1: // Second Circle
				if (pInst->timeCD > bossCircle_duration * 2) {
					//Draw circle
					staticObjInstCreate(TYPE_BOSSCIRCLE, bossCircleSize, &playerPosition, 0);
					pInst->timeCD = 0;
					stage = 2;
				} break;
			case 2: // Third Circle
				if (pInst->timeCD > bossCircle_duration * 2) {
					//Draw circle
					staticObjInstCreate(TYPE_BOSSCIRCLE, bossCircleSize, &playerPosition, 0);
					pInst->timeCD = 0;
					stage = 3;
				} break;
			case 3: // Actual Attack Circle
				if (pInst->timeCD > bossCircle_duration * 4) {
					staticObjInstCreate(TYPE_BOSSCIRCLEATTACK, 3, &playerPosition, 0);
					pInst->timeCD = 0;
					stage = 0;
					pInst->stateFlag = STATE_PATROL;
					pInst->innerState = INNER_STATE_ON_EXIT;
				}
				break;
			}

			break;/////////////////////////////////////////////////////////////////

		case INNER_STATE_ON_EXIT://///////////////////////////////////////////////

			std::cout << "exiting state 2" << std::endl;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->state = pInst->stateFlag;

			break;/////////////////////////////////////////////////////////////////
		} break;


	/**********************************************************************************
	
		Spawn Enemy state - Spawn 2 enemies at random positions

	***********************************************************************************/

	case STATE_SPAWN_ENEMIES: /////////////////////////////////////////////////////
		switch (pInst->innerState) {
		case INNER_STATE_ON_ENTER:
			pInst->velCurr = { 0,0 };
			std::cout << "entering state 3" << std::endl;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			break;/////////////////////////////////////////////////////////////////

		case INNER_STATE_ON_UPDATE: ///////////////////////////////////////////////

			std::cout << "updating state 3" << std::endl;
			//Spawn enemies
			pInst->timeCD += g_dt;
			if (pInst->timeCD > bossStillDuration) {
				pInst->timeCD = 0;
				// Set 2 random enemy positions within the map, ensuring that enemies don't spawn on the player
				do {
					enemyPos1 = { mapMin.x + AERandFloat() * mapRange.x ,mapMin.y - AERandFloat() * mapRange.y };
					enemyPos2 = { mapMin.x + AERandFloat() * mapRange.x ,mapMin.y - AERandFloat() * mapRange.y };
				} while (static_cast<int> (enemyPos1.x) == Player->posCurr.x || static_cast<int> (enemyPos2.y) == Player->posCurr.y);

				// Spawn 2 enemies
				gameObjInstCreate(TYPE_ENEMY, 1, &enemyPos1, nullptr, 0);
				gameObjInstCreate(TYPE_ENEMY, 1, &enemyPos2, nullptr, 0);

				pInst->stateFlag = STATE_PATROL;
				pInst->innerState = INNER_STATE_ON_EXIT;
			}
			break;/////////////////////////////////////////////////////////////////

		case INNER_STATE_ON_EXIT: /////////////////////////////////////////////////
			std::cout << "exiting state 3" << std::endl;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->state = pInst->stateFlag;
			break;/////////////////////////////////////////////////////////////////
		} break;

	/**********************************************************************************
	
		Spawn Bullet state - Shoots 3 bullets at player

	***********************************************************************************/

	case STATE_SPAWN_BULLETS://////////////////////////////////////////////////////
		switch (pInst->innerState) {
		case INNER_STATE_ON_ENTER:
			pInst->velCurr = { 0,0 };
			std::cout << "entering state 4" << std::endl;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			break;/////////////////////////////////////////////////////////////////

		case INNER_STATE_ON_UPDATE:////////////////////////////////////////////////
			pInst->timeCD += g_dt;
			std::cout << "updating state 4" << std::endl;

			//Spawn Bullets with normalized direction vector
			AEVec2 dirVec = Player->posCurr - Boss->posCurr;
			AEVec2 normDir;
			AEVec2Normalize(&normDir, &dirVec);

			switch (stage) {
			case 0:
				gameObjInstCreate(TYPE_BULLET, 0.5, &pInst->posCurr, &normDir, 0);
				stage = 1;
				break;
			case 1:
				if (pInst->timeCD > bossStillDuration) {
				gameObjInstCreate(TYPE_BULLET, 0.5, &pInst->posCurr, &normDir, 0);
				stage = 2;
				pInst->timeCD = 0;
				}
				break;
			case 2:
				if (pInst->timeCD > bossStillDuration) {
					gameObjInstCreate(TYPE_BULLET, 0.5, &pInst->posCurr, &normDir, 0);
					stage = 3;
					pInst->timeCD = 0;
				}
				break;
			case 3:
				if (pInst->timeCD > bossStillDuration) {
					pInst->timeCD = 0;
					pInst->stateFlag = STATE_PATROL;
					pInst->innerState = INNER_STATE_ON_EXIT;
					stage = 0;
				}
				break;
			}
			break;////////////////////////////////////////////////////////////////////

		case INNER_STATE_ON_EXIT://///////////////////////////////////////////////////
			std::cout << "exiting state 4" << std::endl;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->state = pInst->stateFlag;
			break;
		}break;///////////////////////////////////////////////////////////////////////

	/**********************************************************************************
	
		Spawn Dark room state - Darkens the room

	***********************************************************************************/
	case STATE_MAZE_DARKEN://///////////////////////////////////////////////////
		switch (pInst->innerState) {
		case INNER_STATE_ON_ENTER:
			// Reset velocity
			pInst->velCurr = { 0,0 };
			std::cout << "entering state 5" << std::endl;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			break;//////////////////////////////////////////////////////////////

		case INNER_STATE_ON_UPDATE: ////////////////////////////////////////////

			std::cout << "updating state 5" << std::endl;
			// Activate dark room
			dark = true;
			// Stay still for awhile
			pInst->timeCD += g_dt;
			if (pInst->timeCD > bossStillDuration) {
				pInst->timeCD = 0;

				pInst->stateFlag = STATE_PATROL;
				pInst->innerState = INNER_STATE_ON_EXIT;
			}
			break;///////////////////////////////////////////////////////////////

		case INNER_STATE_ON_EXIT:////////////////////////////////////////////////
			// Return to patrol
			std::cout << "exiting state 5" << std::endl;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->state = pInst->stateFlag;
			break;
		} break;/////////////////////////////////////////////////////////////////
	}
}

// ---------------------------------------------------------------------------



