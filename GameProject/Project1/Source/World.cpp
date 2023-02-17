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

/*!
	Defines
*/
/******************************************************************************/
const unsigned int	GAME_OBJ_NUM_MAX = 32;		// The total number of different objects (Shapes)
const unsigned int	TEXTURE_NUM_MAX = 32;		// The total number of Textures
const unsigned int	GAME_OBJ_INST_NUM_MAX = 2048;		// The total number of different game object instances
const unsigned int	FONT_NUM_MAX = 10;					// The total number of different game object instances
const unsigned int	STATIC_OBJ_INST_NUM_MAX = 100000;		// The total number of different game object instances

const float         BOUNDING_RECT_SIZE = 1.0f;     // this is the normalized bounding rectangle (width and height) sizes - AABB collision data
const float			PLAYER_SPEED = 5.f;
const float			NPC_SPEED = 2.5f;

const int			TEXTURE_MAXWIDTH = 192;
const int			TEXTURE_MAXHEIGHT = 192;
const float			TEXTURE_CELLSIZE = 16;

const int			SPRITE_SCALE = 80;
bool				SLASH_ACTIVATE = false;

const int			MAP_CELL_WIDTH = 200;
const int			MAP_CELL_HEIGHT = 100;

const int			CAM_CELL_WIDTH = 20;
const int			CAM_CELL_HEIGHT = 12;

extern unsigned int		state = 0;
unsigned int		mapeditor = 0;

float				mouseX;
float				mouseY;

f32					camX;
f32					camY;

int				pHealth = 3;


// -----------------------------------------------------------------------------ma


// -----------------------------------------------------------------------------
// object flag definition

const unsigned long FLAG_ACTIVE = 0x00000001;

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
static GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sGameObjInstNum;							// The number of used game object instances

// list of static instances
static staticObjInst		sStaticObjInstList[STATIC_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sStaticObjInstNum;								// The number of used game object instances

static staticObjInst* MapObjInstList[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];
static int					binaryMap[MAP_CELL_WIDTH][MAP_CELL_HEIGHT];

static s8					FontList[FONT_NUM_MAX];						// Each element in this array represents a Font
static unsigned long		FontListNum;								// The number of used fonts

// pointer to the ship object
static GameObjInst* Player;												// Pointer to the "Player" game object instance
static staticObjInst* mapEditorObj;
static staticObjInst* Health[3];										// Pointer to the player health statc object instance

// ---------------------------------------------------------------------------

/******************************************************************************/




/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the World level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_World_Load(void) {
	// zero the game object array
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	sGameObjNum = 0;

	// zero the game object instance array
	memset(sGameObjInstList, 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sGameObjInstNum = 0;

	// zero the game object instance array
	memset(sStaticObjInstList, 0, sizeof(staticObjInst) * STATIC_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sStaticObjInstNum = 0;

	// The ship object instance hasn't been created yet, so this "spShip" pointer is initialized to 0
	Player = nullptr;

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
	//Character->pTexture = AEGfxTextureLoad("../Assets/Tilemap/tilemap_packed.png");
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


	GameObj* Effects;
	Effects = sGameObjList + sGameObjNum++;
	AEGfxMeshStart();

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
		0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f);

	Effects->pMesh = AEGfxMeshEnd();
	Effects->pTexture = Character->pTexture;
	Effects->type = TYPE_EFFECTS;
	Effects->refTexture = true;

	GameObj* Slash;
	Slash = sGameObjList + sGameObjNum++;
	Slash->pMesh = Effects->pMesh;
	//Slash->pTexture = AEGfxTextureLoad("../Assets/slash.png");
	Slash->pTexture = AEGfxTextureLoad("Assets/slash.png");
	Slash->type = TYPE_SLASH;
	Slash->refMesh = true;

	GameObj* RefLine;
	RefLine = sGameObjList + sGameObjNum++;
	RefLine->pMesh = Effects->pMesh;
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
	//BUGGY CODE, IF UANBLE TO LOAD, CANNOT USE DEBUGGING MODE
	//s8 font = AEGfxCreateFont("Assets/OpenSans-Regular.ttf", 12);
	//FontList[FontListNum++] = font;
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the World state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_World_Init(void) {
	AEVec2 PlayerPos = { 12,-31 };
	Player = gameObjInstCreate(TYPE_CHARACTER, 1, &PlayerPos, 0, 0);
	Player->TextureMap = { 1,8 };

	//Initialise map texture numbers.

	std::ifstream mapInput{ "Assets/textureWorld.txt" };
	//std::ifstream mapInput{ "../Assets/map1.txt" };
	for (int j = 0; j < MAP_CELL_HEIGHT; j++) {
		for (int i = 0; i < MAP_CELL_WIDTH; i++) {
			AEVec2 Pos = { (float)i + 0.5f , -((float)j + 0.5f) };
			staticObjInstCreate(TYPE_MAP, 1, &Pos, 0);
			staticObjInst* pInst = sStaticObjInstList + i + j * MAP_CELL_WIDTH;
			MapObjInstList[i][j] = pInst;
			// input texture
			pInst->TextureMap = { 0,0 };
			mapInput >> MapObjInstList[i][j]->TextureMap.x;
			mapInput >> MapObjInstList[i][j]->TextureMap.y;
		}
	}
	mapInput.close();

	utilities::importMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *binaryMap, "binaryWorld.txt");


	for (int j = 0; j < MAP_CELL_HEIGHT; j++) {
		for (int i = 0; i < MAP_CELL_WIDTH; i++) {
			AEVec2 Pos = { (float)i + 0.5f , -((float)j + 0.5f) };
			staticObjInstCreate(TYPE_REF, 1, &Pos, 0);

		}
	}

	AEVec2 Pos = { 9.f , 3.f };
	mapEditorObj = staticObjInstCreate(TYPE_MAP, 0, &Pos, 0);


	//Initialise player health. Printing of hearts.
	Player->health = 3;
	Health[0] = staticObjInstCreate(TYPE_HEALTH, 1, nullptr, 0);
	Health[1] = staticObjInstCreate(TYPE_HEALTH, 1, nullptr, 0);
	Health[2] = staticObjInstCreate(TYPE_HEALTH, 1, nullptr, 0);
	Health[0]->TextureMap = { 0,11 };
	Health[1]->TextureMap = { 0,11 };
	Health[2]->TextureMap = { 0,11 };
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the World state.
*/
/******************************************************************************/

void GS_World_Update(void) {

	// =====================================
	// User Input
	// =====================================
	camX = Player->posCurr.x, camY = Player->posCurr.y;
	//Debugging mode
	if (AEInputCheckTriggered(AEVK_F3)) {
		state ^= 1;
	}

	if (AEInputCheckTriggered(AEVK_9)) {
		mapeditor ^= 1;
	}


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
		Player->velCurr.x = 1;// this is direction , positive y direction
		Player->scale = 1;
	}

	int slashDir{ 0 };
	// Normalising mouse to 0,0 at the center
	s32 mouseIntX, mouseIntY;
	AEInputGetCursorPosition(&mouseIntX, &mouseIntY);
	mouseX = (float)(mouseIntX - AEGetWindowWidth() / 2) / SPRITE_SCALE;
	mouseY = (float)(-mouseIntY + AEGetWindowHeight() / 2) / SPRITE_SCALE;

	float angleMousetoPlayer = utilities::getAngle(Player->posCurr.x, Player->posCurr.y, mouseX + Player->posCurr.x, mouseY + Player->posCurr.y);

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
					&& AEInputCheckTriggered(AEVK_LBUTTON)) {
					MapObjInstList[i][j]->TextureMap = mapEditorObj->TextureMap;
				}
			}
		}
	}
	else {
		mapEditorObj->scale = 0;
	}

	//Map editor printing
	if (AEInputCheckTriggered(AEVK_8)) {
		utilities::exportMapTexture(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, **MapObjInstList, "textureWorld.txt");
		utilities::exportMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, **MapObjInstList, "binaryWorld.txt");
	}

	if (AEInputCheckTriggered(AEVK_7)) {
		utilities::importMapBinary(MAP_CELL_HEIGHT, MAP_CELL_WIDTH, *binaryMap, "binaryWorld.txt");
	}

	// ======================================================
	// update physics of all active game object instances
	//  -- Get the AABB bounding rectangle of every active instance:
	//		boundingRect_min = -(BOUNDING_RECT_SIZE/2.0f) * instance->scale + instance->pos
	//		boundingRect_max = +(BOUNDING_RECT_SIZE/2.0f) * instance->scale + instance->pos
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;
		pInst->boundingBox.min.x = -(BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.x;
		pInst->boundingBox.min.y = -(BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.y;
		pInst->boundingBox.max.x = (BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.x;
		pInst->boundingBox.max.y = (BOUNDING_RECT_SIZE / 2.0f) * pInst->scale + pInst->posCurr.y;
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
	

	if ((Player->posCurr.y - SPRITE_SCALE / 2) <= 45 && (Player->posCurr.y + SPRITE_SCALE / 2) >= -65 && (Player->posCurr.x - SPRITE_SCALE / 2) <= -85 && (Player->posCurr.x + SPRITE_SCALE / 2) >= -215) {
		//player_direction.x = -player_direction.x;

		float player_bottom = Player->posCurr.y + 50;
		float tiles_bottom = 0 + 50;
		float player_right = Player->posCurr.x + 50;
		float tiles_right = -160 + 50;

		float b_collision = tiles_bottom - Player->posCurr.y;
		float t_collision = player_bottom - 0;
		float l_collision = player_right + 160;
		float r_collision = tiles_right - Player->posCurr.x;

		if (t_collision < b_collision && t_collision < l_collision && t_collision < r_collision) {
			//Top collision
			std::cout << "collide top" << std::endl;
			if (Player->velCurr.y == 1) {
				std::cout << "move top" << std::endl;
				Player->velCurr.y = 0;
			}
			else {
				std::cout << "move bot" << std::endl;
				Player->velCurr.y = -1;
				Player->posCurr.y += Player->velCurr.y;
			}
		}

		if (b_collision < t_collision && b_collision < l_collision && b_collision < r_collision) {
			//bottom collision
			std::cout << "collide botton" << std::endl;
			if (Player->velCurr.y == -1) {
				std::cout << "move top" << std::endl;
				Player->velCurr.y = 0;
			}
			else {
				std::cout << "move bot" << std::endl;
				Player->velCurr.y = 1;
				Player->posCurr.y += Player->velCurr.y;
			}
		}

		if (l_collision < r_collision && l_collision < t_collision && l_collision < b_collision) {
			//Left collision
			std::cout << "collide left" << std::endl;
			if (Player->velCurr.x == 1)
			{
				std::cout << "move top" << std::endl;
				Player->velCurr.x = 0;
			}
			else {
				std::cout << "move bot" << std::endl;
				Player->velCurr.x = -1;
				Player->posCurr.x += Player->velCurr.x;
			}

		}

		if (r_collision < l_collision && r_collision < t_collision && r_collision < b_collision) {
			//Right collision
			std::cout << "collide right" << std::endl;
			if (Player->velCurr.x == -1) {
				std::cout << "move top" << std::endl;
				Player->velCurr.x = 0;
			}
			else {
				std::cout << "move bot" << std::endl;
				Player->velCurr.x = 1;
				Player->posCurr.x += Player->velCurr.x;
			}
		}
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
			if (pInst->pObject->type == TYPE_NPCS) {
				pInst->velCurr.x *= NPC_SPEED; // magnitude/speed of velo.x
				pInst->velCurr.y *= NPC_SPEED; // magnitude/speed of velo.y
			}
		}

		pInst->posCurr.x += pInst->velCurr.x * g_dt;
		pInst->posCurr.y += pInst->velCurr.y * g_dt;
	}

	camX = Player->posCurr.x, camY = Player->posCurr.y;

	//player health following viewport
	Health[0]->posCurr = { (float)camX + 7.5f , (float)camY + 5.1f };
	Health[1]->posCurr = { (float)camX + 8.5f , (float)camY + 5.1f };
	Health[2]->posCurr = { (float)camX + 9.5f , (float)camY + 5.1f };

	if (SLASH_ACTIVATE == true) {
		AEVec2 Pos = Player->posCurr;
		Pos.x += Player->velCurr.x * 0.25f;
		Pos.y += Player->velCurr.y * 0.25f;
		staticObjInst* slashObj = staticObjInstCreate(TYPE_SLASH, 1, &Pos, 0);
		switch (slashDir) {
		case 1: //right
			slashObj->posCurr.x += 0.8f;
			slashObj->posCurr.y += 0;
			slashObj->dirCurr = 0;
			break;
		case 2: //up
			slashObj->posCurr.y += 0.8f;
			slashObj->posCurr.x += 0;
			slashObj->dirCurr = PI / 2;
			break;
		case 3: //left
			slashObj->posCurr.x += -0.8f;
			slashObj->posCurr.y += 0;
			slashObj->dirCurr = PI;
			break;
		case 4: //down
			slashObj->posCurr.y += -0.8f;
			slashObj->posCurr.x += 0;
			slashObj->dirCurr = -PI / 2;
			break;
		default:
			break;
		}
		slashObj->timetracker = 0;
		SLASH_ACTIVATE = false;
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


	// =====================================
	// calculate the matrix for all objects
	// =====================================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		AEMtx33		 trans = { 0 }, rot = { 0 }, scale = { 0 };

		UNREFERENCED_PARAMETER(trans);
		UNREFERENCED_PARAMETER(rot);
		UNREFERENCED_PARAMETER(scale);

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

		UNREFERENCED_PARAMETER(trans);
		UNREFERENCED_PARAMETER(rot);
		UNREFERENCED_PARAMETER(scale);

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

}

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_World_Draw(void) {
	// Tell the engine to get ready to draw something with texture. 
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	for (unsigned long i = 0; i < MAP_CELL_WIDTH; i++) {
		for (unsigned long j = 0; j < MAP_CELL_HEIGHT; j++) {
			if (utilities::checkWithinCam(MapObjInstList[i][j]->posCurr, camX, camY)) {
				continue;
			}
			AEGfxSetTransparency(1.0f);
			AEGfxTextureSet(MapObjInstList[i][j]->pObject->pTexture,
				TEXTURE_CELLSIZE / TEXTURE_MAXWIDTH * MapObjInstList[i][j]->TextureMap.x,
				TEXTURE_CELLSIZE / TEXTURE_MAXHEIGHT * MapObjInstList[i][j]->TextureMap.y);
			AEGfxSetTransform(MapObjInstList[i][j]->transform.m);
			AEGfxMeshDraw(MapObjInstList[i][j]->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
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
		// for any sprite textures
		if (pInst->pObject->type == TYPE_SLASH) {
			AEGfxSetTransparency(1.0f - pInst->Alpha);
		}
		else {
			AEGfxSetTransparency(1.0f);
		}
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
		AEGfxPrint(FontList[0], debug, -0.99f, 0.90f, 1.5f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(FontList[0], input, -0.99f, 0.65f, 1.5f, 1.0f, 1.0f, 1.0f);

		sprintf_s(mouse_xy_buffer, "Mouse Position X: %.4f", mouseX + camX);
		AEGfxPrint(FontList[0], mouse_xy_buffer, -0.99f, 0.76f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(mouse_xy_buffer, "Mouse Position Y: %.4f", mouseY + camY);
		AEGfxPrint(FontList[0], mouse_xy_buffer, -0.99f, 0.71f, 1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(FontList[0], a, -0.99f, 0.55f, 1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(FontList[0], w, -0.99f, 0.60f, 1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(FontList[0], s, -0.99f, 0.50f, 1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxPrint(FontList[0], d, -0.99f, 0.45f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(playerpos, "Player Position X: %.4f", (Player->posCurr.x));
		AEGfxPrint(FontList[0], playerpos, -0.99f, 0.40f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(playerpos, "Player Position Y: %.4f", (Player->posCurr.y));
		AEGfxPrint(FontList[0], playerpos, -0.99f, 0.35f, 1.0f, 1.0f, 1.0f, 1.0f);

		sprintf_s(camxy_buffer, "Camera Position X: %.4f", camX);
		AEGfxPrint(FontList[0], camxy_buffer, -0.99f, 0.30f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(camxy_buffer, "Camera Position Y: %.4f", camY);
		AEGfxPrint(FontList[0], camxy_buffer, -0.99f, 0.25f, 1.0f, 1.0f, 1.0f, 1.0f);




		if (AEInputCheckCurr(AEVK_W))
		{
			sprintf_s(w, "W Pressed");
			AEGfxPrint(FontList[0], w, -0.99f, 0.60f, 1.0f, 0.0f, 1.0f, 0.0f);
		}
		if (AEInputCheckCurr(AEVK_A))
		{
			sprintf_s(a, "A Pressed");
			AEGfxPrint(FontList[0], a, -0.99f, 0.55f, 1.0f, 0.0f, 1.0f, 0.0f);
		}
		if (AEInputCheckCurr(AEVK_S))
		{
			sprintf_s(s, "S Pressed");
			AEGfxPrint(FontList[0], s, -0.99f, 0.50f, 1.0f, 0.0f, 1.0f, 0.0f);
		}
		if (AEInputCheckCurr(AEVK_D))
		{
			sprintf_s(s, "D Pressed");
			AEGfxPrint(FontList[0], s, -0.99f, 0.45f, 1.0f, 0.0f, 1.0f, 0.0f);
		}
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
	for (unsigned int i = 0; i < sGameObjNum; i++) {
		if ((sGameObjList + i)->refMesh == false)
			AEGfxMeshFree((sGameObjList + i)->pMesh);
		if ((sGameObjList + i)->refTexture == false)
			AEGfxTextureUnload((sGameObjList + i)->pTexture);
	}

	//BUGGY CODE, IF UANBLE TO LOAD, CANNOT USE DEBUGGING MODE
		//AEGfxDestroyFont(FontList[0]);

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
GameObjInst* gameObjInstCreate(unsigned long type,
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
void gameObjInstDestroy(GameObjInst* pInst)
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
staticObjInst* staticObjInstCreate(unsigned long type, float scale, AEVec2* pPos, float dir)
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
void staticObjInstDestroy(staticObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	sStaticObjInstNum--; //Decrement the number of game object instance
	pInst->flag = 0;
}