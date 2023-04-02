/******************************************************************************/
/*!
\file		Maze.cpp
\author 	Alvin Woo Jia Hao
\par    	email: a.woo\@digipen.edu
\date   	February 02, 2023
\brief		This header file contains the functions for the level of Main Menu.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Main.h"
#include <fstream>
#include <iostream>

/*!
	Definess
*/
/******************************************************************************/


static const int			MAP_CELL_WIDTH = 124;				// Total number of cell widths
static const int			MAP_CELL_HEIGHT = 42;				// Total number of cell heights

static const AEVec2			levelClearMin = { 38,-33 };			// Min tile for level to be cleared
static const AEVec2			levelClearMax = { 40,-32 };			// Max tile for level to be cleared

static const float			pathingCDDuration = 10.f;			// Duration for pathing to complete cooldown
static const float			pathingStateDuration = 3.f;			// Duration for pathing state to remain on
static const int			pathingTrackingMax = 3000;			// Total number of tracking tiles drawn before looping back

static const AEVec2			playerStartPoint{ 12,-8 };			// Starting coordinate of new game player
// -----------------------------------------------------------------------------


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
static GameObjInst* Player;								// Pointer to the "Player" game object instance
static staticObjInst* mapEditorObj;						// Pointer to the reference map editor object instance
static staticObjInst* Health[3];						// Pointer to the player health statc object instance
static staticObjInst* Levers[3];						// Pointer to each lever object
static staticObjInst* MenuObj[3];						// Pointer to each menu UI object
static staticObjInst* NumObj[3];						// Pointer to each number UI object
static staticObjInst* PauseObj;							// Pointer to Pause Obj
static staticObjInst* StartScreenbj;					// Pointer to start screen Obj

// State Variables
static bool					debugging;					// Debugging state
static bool					mapeditor;					// Map edtior state
static bool					dark;						// Dark room state
static bool					pathing;					// Pathing mode state
static bool					pathingcd;					// Pathing cooldown state
static bool					movement;					// Player's movement state

// Time Variables
static float				pathingcdtime;				// Stores time left for pathing cooldown
static float				pathingtime;				// Stores time left for pathing mode
static float				playerHitTime;				// Stores time left for player's invulnerability upon attacking
static float				slashCD;					// Stores time left before player can slash again
static float				walkCD;						// Stores time left before player can move after slashing

// Path tracking variables
static AEVec2 mappingarr[pathingTrackingMax];			// 2D Array of Vectors to track position of pathing
static int arrin;										// Accessing value of Array, incremented per game loop

static Inventory Backpack;								// Inventory of Character

AEAudio MazeBG;
AEAudioGroup MazeBGG;
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
	
	// Setting initial numbers to 0
	sGameObjNum = 0;
	sGameObjInstNum = 0;
	sStaticObjInstNum = 0;
	Player = nullptr;

	// List of Unique Game Objs
	GameObj* Character = 0, * Item = 0, * Map = 0, * Slash = 0,
		* RefLine = 0, * Health = 0, * Key = 0,
		* Spike = 0, *Chest = 0, *Pause = 0, *Start = 0;

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

	// Mesh for Dark Room - 2
	AEGfxMeshStart();

	AEGfxTriAdd(80.0f, -45.f, 0xFFFF00FF, 1.0f, 1.0f,
		-80.0f, -45.f, 0xFFFFFF00, 0.0f, 1.0f,
		80.0f, 45.f, 0xFF00FFFF, 1.0f, 0.0f);

	AEGfxTriAdd(-80.0f, -45.f, 0xFFFFFFFF, 0.0f, 1.0f,
		-80.0f, 45.f, 0xFFFFFFFF, 0.0f, 0.0f,
		80.0f, 45.f, 0xFFFFFFFF, 1.0f, 0.0f);
	meshList.push_back(AEGfxMeshEnd());

	// Mesh for mapchar - 3
	AEGfxMeshStart();

	AEGfxTriAdd(-2.f, 2.f, 0xFFFF0000, 0.0f, 0.0f,
		-2.f, -2.f, 0xFFFF0000, 0.0f, 1.0f,
		2.f, 2.f, 0xFFFF0000, 1.0f, 0.0f);

	AEGfxTriAdd(2.f, -2.f, 0xFFFF0000, 1.0f, 1.0f,
		-2.f, -2.f, 0xFFFF0000, 0.0f, 1.0f,
		2.f, 2.f, 0xFFFF0000, 1.0f, 0.0f);

	meshList.push_back(AEGfxMeshEnd());

	//Mesh Alias
	AEGfxVertexList*& spriteMesh = meshList[0];
	AEGfxVertexList*& fullSizeMesh = meshList[1];

	// =====================================
	//	Load Textures
	// =====================================
	// 
	//Load Textures
	textureList.push_back(AEGfxTextureLoad("Assets/slash.png")); // 0
	textureList.push_back(AEGfxTextureLoad("Assets/Tilemap/RefBox.png")); // 1
	textureList.push_back(AEGfxTextureLoad("Assets/Tilemap/tilemap_packed.png")); // 2
	textureList.push_back(AEGfxTextureLoad("Assets/Darkroom.png")); // 3
	textureList.push_back(AEGfxTextureLoad("Assets/Maze_Obj.png")); // 4
	textureList.push_back(AEGfxTextureLoad("Assets/PauseScreen.png")); // 5
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_1.png")); //6
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_2.png")); //7
	textureList.push_back(AEGfxTextureLoad("Assets/MainMenu/Instruction_3.png")); //8

	//Texture Alias
	AEGfxTexture*& slashTex = textureList[0];
	AEGfxTexture*& refBox = textureList[1];
	AEGfxTexture*& spriteSheet = textureList[2];
	AEGfxTexture*& startTex = textureList[4];
	AEGfxTexture*& PauseTex = textureList[5];

	// =====================================
	//	Load Unique Game Objs
	// =====================================
	utilities::loadMeshNTexture(Character, spriteMesh, spriteSheet, TYPE_CHARACTER);
	utilities::loadMeshNTexture(Item, spriteMesh, spriteSheet, TYPE_ITEMS);
	utilities::loadMeshNTexture(Map, spriteMesh, spriteSheet, TYPE_MAP);
	utilities::loadMeshNTexture(Slash, fullSizeMesh, slashTex, TYPE_SLASH);
	utilities::loadMeshNTexture(RefLine, fullSizeMesh, refBox, TYPE_REF);
	utilities::loadMeshNTexture(Health, spriteMesh, spriteSheet, TYPE_HEALTH);
	utilities::loadMeshNTexture(Key, spriteMesh, spriteSheet, TYPE_KEY);
	utilities::loadMeshNTexture(Chest, spriteMesh, spriteSheet, TYPE_CHEST);
	utilities::loadMeshNTexture(Spike, spriteMesh, spriteSheet, TYPE_SPIKE);
	utilities::loadMeshNTexture(Pause, fullSizeMesh, PauseTex, TYPE_PAUSE);
	utilities::loadMeshNTexture(Start, fullSizeMesh, startTex, TYPE_START);

	// =====================================
	//	Load Audio
	// =====================================
	 MazeBG = AEAudioLoadMusic("Assets/Music/INPUT - NewAge MSCNEW2_01.mp3");
	 MazeBGG = AEAudioCreateGroup();

	 HeroDamaged = AEAudioLoadMusic("Assets/Music/HUMAN-GRUNT_GEN-HDF-15047.wav");
	 Damage = AEAudioCreateGroup();
	 HeroSlash = AEAudioLoadMusic("Assets/Music/METAL-HIT_GEN-HDF-17085.wav");
	 Movement = AEAudioLoadMusic("Assets/Music/FOOTSTEPS-OUTDOOR_GEN-HDF-12363.mp3");
	 MovementGroup = AEAudioCreateGroup();

	 ParticleSystemLoad();
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Maze state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_Maze_Init(void) {

	//Create objects for pause and start screen
	PauseObj = staticObjInstCreate(TYPE_PAUSE, 1, nullptr, 0);
	StartScreenbj = staticObjInstCreate(TYPE_START, 1, nullptr, 0);

	AEVec2* pos = nullptr;
	int num;

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
	//	Initialize objects for loaded game
	// =====================================
		//Initialise Player
	AEVec2 PlayerPos = playerStartPoint;
	Player = gameObjInstCreate(TYPE_CHARACTER, 1, &PlayerPos, 0, 0);

	std::ifstream ifs{ "Assets/save.txt" };
	ifs >> Player->health;
	ifs >> Backpack.Key; //set to player number of current potion
	ifs >> Backpack.Potion; //set to player number of current key
	ifs.close();

	utilities::loadObjs(pos, num, "mazeSpikes.txt");
	for (int i = 0; i < num; i++)
	{
		staticObjInstCreate(TYPE_SPIKE, 1, &pos[i], 0);
	}
	utilities::unloadObjs(pos);

	// Initialise camera pos
	camX = Player->posCurr.x, camY = Player->posCurr.y;
	AEGfxSetCamPosition(0, 0);

	MenuObj[0] = staticObjInstCreate(TYPE_ITEMS, 1, nullptr, 0); // Potions
	MenuObj[1] = staticObjInstCreate(TYPE_KEY, 1, nullptr, 0); // Keys


	NumObj[0] = staticObjInstCreate(TYPE_ITEMS, 1, nullptr, 0); // Potions
	NumObj[1] = staticObjInstCreate(TYPE_KEY, 1, nullptr, 0); // Keys

	//Initialise player health.
	for (int i = 0; i < MAX_PLAYER_HEALTH; i++) {
		Health[i] = staticObjInstCreate(TYPE_HEALTH, 0.75, nullptr, 0);
	}

	ParticleSystemInit();

	AEAudioPlay(MazeBG, MazeBGG, 0.25f, 1, 1);

	// Initialise states and timers
	debugging = false;
	mapeditor = false;
	dark = true;
	pathing = false;
	pathingcd = false;
	levelstart = true;
	pause = true;

	pathingcdtime = 0;
	pathingtime = 0;
	playerHitTime = 0;
	slashCD = 0;
	walkCD = 0;
	cycle = 0;

}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Maze state.
*/
/******************************************************************************/

void GS_Maze_Update(void) {
	
	// Toggle Pause button
	if (AEInputCheckTriggered(AEVK_ESCAPE)) {
		pause = !pause;
		levelstart = false;
		cycle = 0;
	}

	// Toggle help screen
	if (pause == true && levelstart == false) {
		utilities::moveHelpScreen(*PauseObj, 5);
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
		//Dark Mesh toggle
		if (AEInputCheckTriggered(AEVK_1))
		{
			dark ^= true;
		}

		//pathing toggle
		if (pathingcd == 0) {
			if (AEInputCheckTriggered(AEVK_M))
			{
				pathingcd = true;
				pathing ^= true;
			}
		}

		Player->playerStand();

		// Movement Controls
		if (AEInputCheckCurr(AEVK_W) || AEInputCheckCurr(AEVK_UP) || AEInputCheckCurr(AEVK_S) || AEInputCheckCurr(AEVK_DOWN)
			|| AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT) || AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT)) {
			Player->playerWalk(walkCD);
			movement = true;
		}
		else {
			Player->TextureMap = TEXTURE_PLAYER;
			AEAudioStopGroup(MovementGroup);
			movement = false;
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
			movement = false;
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
			utilities::exportMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "textureMaze.txt");

			utilities::exportMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *MapObjInstList, "binaryMaze.txt");
		}

		// NO BOUNDING BOX UPDATE - AABB NOT USED IN this LEVEL

		// ======================================================
		//	-- Positions of the instances are updated here with the already computed velocity (above)
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



		int flag = CheckInstanceBinaryMapCollision(Player->posCurr.x, -Player->posCurr.y, binaryMap);
		// Binary collision for player
		snapCollision(*Player, flag);

		for (int i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++) {
			staticObjInst* pInst = sStaticObjInstList + i;
			if (pInst->flag != FLAG_ACTIVE || pInst->pObject->type != TYPE_SPIKE) {
				continue;
			}

			pInst->spikeUpdate(); // Updates alpha of spikes

			// Between Spikes and Players
			if (Player->calculateDistance(*pInst) <= SPIKE_RANGE && (pInst->Alpha == 0) && playerHitTime == 0) {

				Player->deducthealth();
				AEAudioPlay(HeroDamaged, Damage, 0.3f, 1, 0);
				playerHitTime = DAMAGE_COODLDOWN_t;
			}
		}

		// Tint of character if damaged
		Player->playerDamaged(playerHitTime);

		// Clear condition for maze
		if (utilities::inRange(Player, levelClearMin, levelClearMax)) {
			AEAudioStopGroup(MazeBGG);
			utilities::completeLevel(maze, Player, Backpack);
		}
		// Death condition for maze
		if (Player->health == 0) {
			gGameStateNext = GS_DEATHSCREEN;
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

		// Update pathing instances
		if (pathing == true) // If pathing state is siwtched on
		{
			pathingtime += g_dt;
			if (pathingtime >= pathingStateDuration) // Increase time until duration
			{
				pathing = false;
				pathingtime = 0;
			}
		}

		if (pathingcd == true) // If pathing is on coooldown
		{
			pathingcdtime += g_dt;
			if (pathingcdtime >= pathingCDDuration) // Increase time until duration
			{
				pathingcd = false;
				pathingcdtime = 0;
			}
		}


		if (movement == true) // If player is moving
		{
			// Add to pathing drawing array
			mappingarr[arrin].x = (camX * SPRITE_SCALE);
			mappingarr[arrin].y = (camY * SPRITE_SCALE);
			arrin++;
		}

		if (arrin >= pathingTrackingMax) { // If player wlaked beyond tracking max
			arrin = 0; // Reset the buffer
		}

		// Set Camera and the UI objects
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
	} // End of Pause
	
} // End of Update

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_Maze_Draw(void) {

	// Set modes for rendering
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	if (pause == true) {

		// Draw starting instructions
		if (levelstart)
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

		// Draw pause menu
		else 
		{
			PauseObj;

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

		// Draw map objects
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

		// Spawn spikes
		for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
		{
			staticObjInst* pInst = sStaticObjInstList + i;

			// skip non-active object and reference boxes
			if (pInst->flag != FLAG_ACTIVE)
				continue;
			if (pInst->pObject->type != TYPE_SPIKE) {
				continue;
			}
			// Only draw within viewport
			if (utilities::checkWithinCam(pInst->posCurr, camX, camY)) {
				continue;
			}

			AEGfxSetTransparency(1.0f - pInst->Alpha);

			// For any types using spritesheet

			AEGfxTextureSet(pInst->pObject->pTexture,
				pInst->TextureMap.x * TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH,
				pInst->TextureMap.y * TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT);

			// Set the current object instance's transform matrix using "AEGfxSetTransform"
			AEGfxSetTransform(pInst->transform.m);
			// Draw the shape used by the current object instance using "AEGfxMeshDraw"
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

		// Draw dark room
		if (dark == true) {
			AEMtx33 lscale, lrotate, ltranslate, ltransform;

			AEGfxSetTransparency(1.0f);
			AEGfxTextureSet(textureList[3], 0, 0);

			AEMtx33Scale(&lscale, 20, 20);
			AEMtx33Rot(&lrotate, 0);
			AEMtx33Trans(&ltranslate, camX * SPRITE_SCALE, camY * SPRITE_SCALE);
			// Concat the matrices (TRS) 
			AEMtx33Concat(&ltransform, &lrotate, &lscale);
			AEMtx33Concat(&ltransform, &ltranslate, &ltransform);

			// Choose the transform to use 
			AEGfxSetTransform(ltransform.m);
			// Actually drawing the mesh
			AEGfxMeshDraw(meshList[2], AE_GFX_MDM_TRIANGLES);
		}

		// Spawn Static entities excluding spikes
		for (unsigned long i = 0; i < STATIC_OBJ_INST_NUM_MAX; i++)
		{
			staticObjInst* pInst = sStaticObjInstList + i;

			// skip non-active object and reference boxes
			if (pInst->flag != FLAG_ACTIVE)
				continue;
			if (pInst->pObject->type == TYPE_REF || pInst->pObject->type == TYPE_SPIKE) {
				continue;
			}
			// Only draw within viewport
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

		// Draw pathing
		if (pathing == true)
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			
			for (unsigned long i = 0; i < pathingTrackingMax; i++)
			{
				AEMtx33 lscale, lrotate, ltranslate, ltransform;

				AEMtx33Scale(&lscale, 3, 3);
				AEMtx33Rot(&lrotate, 0);
				AEMtx33Trans(&ltranslate, mappingarr[i].x, mappingarr[i].y);
				// Concat the matrices (TRS) 
				AEMtx33Concat(&ltransform, &lrotate, &lscale);
				AEMtx33Concat(&ltransform, &ltranslate, &ltransform);

				// Actually drawing the mesh

				AEGfxSetTransform(ltransform.m);
				AEGfxMeshDraw(meshList[3], AE_GFX_MDM_TRIANGLES);
			}
		}

		// Pathing texts
		if (pathing == false)
		{
			char tracker[50] = "Press M to Track Past Pathing";
			AEGfxPrint(1, tracker, 0.60f, 0.75f, 1.5f, 1.0f, 1.0f, 1.0f);
		}

		char pathtimerdisplay[50];
		if (10 - pathingcdtime == 10)
		{
			sprintf_s(pathtimerdisplay, "Pathing Cooldown: %.d", 0);
			AEGfxPrint(1, pathtimerdisplay, 0.60f, 0.65f, 1.5f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			sprintf_s(pathtimerdisplay, "Pathing Cooldown: %.f", 10 - pathingcdtime);
			AEGfxPrint(1, pathtimerdisplay, 0.60f, 0.65f, 1.5f, 1.0f, 1.0f, 1.0f);
		}

		// Debugging mode - for developer use
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

	// Stops BG music
	AEAudioStopGroup(MazeBGG);

	ParticleSystemFree();
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
	for (AEGfxVertexList* Mesh : meshList) {
		AEGfxMeshFree(Mesh);
	}

	for (AEGfxTexture* texture : textureList) {
		AEGfxTextureUnload(texture);
	}

	// Clears vector list
	meshList.clear();
	textureList.clear();

	// Reset camera position
	AEGfxSetCamPosition(0, 0);

	ParticleSystemUnload();
}

// ---------------------------------------------------------------------------

