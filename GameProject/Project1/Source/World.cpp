#include "main.h"
#include <iostream>
#include <fstream>


/******************************************************************************/
/*!
\file		World.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 02, 2023
\brief		This source file contains the functions for the level of World.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/


/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
const unsigned int	GAME_OBJ_NUM_MAX				= 32;			// The total number of different objects (Shapes)
const unsigned int	TEXTURE_NUM_MAX					= 32;			// The total number of Textures
const unsigned int	GAME_OBJ_INST_NUM_MAX			= 2048;		// The total number of different game object instances
const unsigned int	FONT_NUM_MAX					= 10;		// The total number of different game object instances

const float         BOUNDING_RECT_SIZE				= 1.0f;         // this is the normalized bounding rectangle (width and height) sizes - AABB collision data


// -----------------------------------------------------------------------------
enum TYPE
{
	// list of game object types
	TYPE_CHARACTER = 0,
	TYPE_NPCS,

	TYPE_NUM
};

// -----------------------------------------------------------------------------
// object flag definition

const unsigned long FLAG_ACTIVE						= 0x00000001;

/******************************************************************************/
/*!
	Struct/Class Definitions
*/
/******************************************************************************/


//Game object structure
struct GameObj
{
	unsigned long	type;		// object type
	AEGfxVertexList* pMesh;		// This will hold the triangles which will form the shape of the object
	AEGfxTexture pTexture;	
};

struct GameObjInst
{
	GameObj* pObject;	// pointer to the 'original' shape
	unsigned long		flag;		// bit flag or-ed together
	float				scale;		// scaling value of the object instance
	AEVec2				posCurr;	// object current position
	AEVec2				velCurr;	// object current velocity
	float				dirCurr;	// object current direction
	//AABB				boundingBox;// object bouding box that encapsulates the object
	AEMtx33				transform;	// object transformation matrix: Each frame, 
	// calculate the object instance's transformation matrix and save it here
};

struct entity {
	double x;
	double y;
};


struct mapTiles {
	int TextureX;
	int TextureY;
};

/******************************************************************************/
/*!
	Static Variables
*/
/******************************************************************************/

// list of original object
static GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sGameObjNum;								// The number of defined game objects

// list of object instances
static GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sGameObjInstNum;							// The number of used game object instances

static AEGfxTexture*		TextureList[TEXTURE_NUM_MAX];				// Each element in this array represents a Texture
static unsigned long		TextureListNum;								// The number of used Textures

static s8					FontList[FONT_NUM_MAX];					// Each element in this array represents a Font
static unsigned long		FontListNum;								// The number of used fonts

// pointer to the ship object
static GameObjInst* Player;												// Pointer to the "Player" game object instance

int mapEditor = 0;
int mapX = 0;
int mapY = 0;
int translateX = 250, translateY = -250;
int max_x = 550, min_x = -550;
int max_y = 300, min_y = -300;
bool enable = 0;
int state = 0;
int blackholestate = 0;
float charScaleX = 60;
int width = 1600, height = 900, scaling = 50;
s32 x{ 0 }, y{ 0 }; //init xy pos
char strx[11] = "X position";
char stry[11] = "Y position";
s32 mx, my;
entity blackhole;
mapTiles maps[20][12];

int direction = 0, counter = 10, live = 3;
float cursorAngle;
//Rotate Init
float i{ 0 };
//Movement Init
AEVec2 player_pos{ 0, 0 }; // player position
AEVec2 player_direction{ 0,0 }; //player direction
float speed{ 400.f };
//Rotate Init
float slashX;
float slashY;
float slashRot;
int slashTrigger;

// ---------------------------------------------------------------------------

// functions to create/destroy a game object instance
GameObjInst*				gameObjInstCreate(unsigned long type, float scale,
							AEVec2* pPos, AEVec2* pVel, float dir);

void						gameObjInstDestroy(GameObjInst* pInst);

/******************************************************************************/


/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the World level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etcÅc
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

	// The ship object instance hasn't been created yet, so this "spShip" pointer is initialized to 0
	Player = nullptr;


	//Initialise map texture numbers.
	//std::ifstream mapInput{ "Assets/map1.txt" };
	std::ifstream mapInput{ "../Assets/map1.txt" };
	for (int j = 0; j < 12; j++) {
		for (int i = 0; i < 20; i++) {

			// input texture
			mapInput >> maps[i][j].TextureX;
			mapInput >> maps[i][j].TextureY;
		}
	}

	mapInput.close();


	GameObj* pObj;
	pObj = sGameObjList + sGameObjNum++;

	AEGfxMeshStart();

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

	pObj->pMesh = AEGfxMeshEnd();

	GameObj* sObj;
	sObj = sGameObjList + sGameObjNum++;


	AEGfxMeshStart();

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFF00FF, 16.0f / 192, 16.0f / 176,
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 16.0f / 176,
		0.5f, 0.5f, 0xFF00FFFF, 16.0f / 192, 0.0f);

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 16.0f / 176,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 16.0f / 192, 0.0f);

	sObj->pMesh = AEGfxMeshEnd();


	GameObj* uObj;
	uObj = sGameObjList + sGameObjNum++;

	AEGfxMeshStart();

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f);

	uObj->pMesh = AEGfxMeshEnd();

	GameObj* bObj;
	bObj = sGameObjList + sGameObjNum++;
	AEGfxMeshStart();

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

	bObj->pMesh = AEGfxMeshEnd();

	
	//AEGfxTexture* pTex = AEGfxTextureLoad("Assets/Tilemap/tilemap_packed.png");
	//TextureList[TextureListNum++] = pTex;
	//AEGfxTexture* pblacktex = AEGfxTextureLoad("Assets/blackhole.png");
	//TextureList[TextureListNum++] = pblacktex;
	//AEGfxTexture* sTex = AEGfxTextureLoad("Assets/slash.png");
	//TextureList[TextureListNum++] = sTex;
	//AEGfxTexture* cTex = AEGfxTextureLoad("Assets/bluee.jpg");
	//TextureList[TextureListNum++] = cTex;
	//AEGfxTexture* planetTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	//TextureList[TextureListNum++] = planetTex;
	//AEGfxTexture* fheart = AEGfxTextureLoad("Assets/full_heart.png");
	//TextureList[TextureListNum++] = fheart;
	//AEGfxTexture* eheart = AEGfxTextureLoad("Assets/empty_heart.png");
	//TextureList[TextureListNum++] = eheart;
	//s8 font = AEGfxCreateFont("Assets/OpenSans-Regular.ttf", 12);
	//FontList[FontListNum++] = font;
	//s8 counterfont = AEGfxCreateFont("Assets/OpenSans-Regular.ttf", 30);
	//FontList[FontListNum++] = counterfont;
	AEGfxTexture* pTex = AEGfxTextureLoad("../Assets/Tilemap/tilemap_packed.png");
	TextureList[TextureListNum++] = pTex;
	AEGfxTexture* pblacktex = AEGfxTextureLoad("../Assets/blackhole.png");
	TextureList[TextureListNum++] = pblacktex;
	AEGfxTexture* sTex = AEGfxTextureLoad("../Assets/slash.png");
	TextureList[TextureListNum++] = sTex;
	AEGfxTexture* cTex = AEGfxTextureLoad("../Assets/bluee.jpg");
	TextureList[TextureListNum++] = cTex;
	AEGfxTexture* planetTex = AEGfxTextureLoad("../Assets/PlanetTexture.png");
	TextureList[TextureListNum++] = planetTex;
	AEGfxTexture* fheart = AEGfxTextureLoad("../Assets/full_heart.png");
	TextureList[TextureListNum++] = fheart;
	AEGfxTexture* eheart = AEGfxTextureLoad("../Assets/empty_heart.png");
	TextureList[TextureListNum++] = eheart;
	s8 font = AEGfxCreateFont("../Assets/OpenSans-Regular.ttf", 12);
	FontList[FontListNum++] = font;
	s8 counterfont = AEGfxCreateFont("../Assets/OpenSans-Regular.ttf", 30);
	FontList[FontListNum++] = counterfont;
	
}


/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the World state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_World_Init(void) {
	blackhole.x = -200;
	blackhole.y = 100;
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the World state.
*/
/******************************************************************************/
void GS_World_Update(void) {
	// Your own update logic goes here
	if (AEInputCheckTriggered(AEVK_F3)) {

		state ^= 1;
	}

	if (AEInputCheckTriggered(AEVK_9)) {
		mapEditor = 1;
	}

	if (AEInputCheckTriggered(AEVK_1)) {
		blackholestate = 1;
	}

	if (AEInputCheckTriggered(AEVK_8)) {
		//std::ofstream mapOutput{ "../Assets/maptest.txt" };
		std::ofstream mapOutput{ "../Assets/maptest.txt" };
		for (int j = 0; j < 12; j++) {
			for (int i = 0; i < 20; i++) {

				// input texture
				mapOutput << maps[i][j].TextureX << " ";
				mapOutput << maps[i][j].TextureY << " ";

				if (i == 19) {
					mapOutput << "\n";
				}
			}
		}
		mapOutput.close();
	}


	float Angle = utilities::getAngle(blackhole.x, blackhole.y, player_pos.x, player_pos.y);
	if (blackholestate == 1) {
		blackhole.x -= cos(Angle) / 5;
		blackhole.y -= sin(Angle) / 5;
	}

	AEInputGetCursorPosition(&x, &y); // check cursor pos

	//std::cout << x << std::endl << std::endl << y;

	cursorAngle = utilities::getAngle(player_pos.x + 600, player_pos.y, x, -(y - 350));

	player_direction.x = 0;// set y direction to 0 initially, if key is released x direction is set back to 0
	player_direction.y = 0;// set y direction to 0 initially, if key is released y direction is set back to 0

	if (-(y - 350) > player_pos.y) {
		cursorAngle = -cursorAngle;
	}

	if (cursorAngle <= -(PI * 3 / 4) || cursorAngle > (PI * 3 / 4)) {
		direction = 1;
	}
	if (cursorAngle > -(PI * 3 / 4) && cursorAngle <= -(PI * 1 / 4)) {
		direction = 2;
	}
	if (cursorAngle <= (PI * 1 / 4) && cursorAngle > -(PI * 1 / 4)) {
		direction = 3;
	}
	if (cursorAngle > (PI * 1 / 4) && cursorAngle <= (PI * 3 / 4)) {
		direction = 4;
	}

	if (AEInputCheckCurr(AEVK_Q))
	{
		//std::cout << "diu" << std::endl;
		i -= 0.02;
		std::cout << " rotating" << std::endl;
	}

	if (AEInputCheckCurr(AEVK_W) || AEInputCheckCurr(AEVK_UP)) // movement for W key 
	{
		player_direction.y = 1;// this is direction , positive y direction
		std::cout << "W key" << std::endl;
	}
	if (AEInputCheckCurr(AEVK_S) || AEInputCheckCurr(AEVK_DOWN))
	{
		player_direction.y = -1;// this is direction , negative y direction
		std::cout << "S key" << std::endl;
	}
	if (AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT))
	{
		player_direction.x = -1;// this is direction , negative x direction
		std::cout << "A key" << std::endl;
		charScaleX = -60;
	}
	if (AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT))
	{
		player_direction.x = 1;// this is direction , positive y direction
		std::cout << "D key" << std::endl;
		charScaleX = 60;
	}



	// add velo to player_pos

	//if (player_direction.x != 0 || player_direction.y != 0) //if player direction is not 0, as you cannot normalize 0.
	//{
	//	AEVec2 temp_velo{ player_direction.x, player_direction.y };
	//	AEVec2Normalize(&player_direction, &temp_velo); // normalize
	//	player_direction.x *= speed; // magnitude/speed of velo.x
	//	player_direction.y *= speed; // magnitude/speed of velo.y
	//}

	//player_pos.x += player_direction.x * 0.016; // 0.016 is delta time temp value
	//player_pos.y += player_direction.y * 0.016;

	//COLLISION

	if ((player_pos.y - 40) <= 45 && (player_pos.y + 40) >= -65 && (player_pos.x - 40) <= -85 && (player_pos.x + 40) >= -215) {
		//player_direction.x = -player_direction.x;



		float player_bottom = player_pos.y + 50;
		float tiles_bottom = 0 + 50;
		float player_right = player_pos.x + 50;
		float tiles_right = -160 + 50;

		float b_collision = tiles_bottom - player_pos.y;
		float t_collision = player_bottom - 0;
		float l_collision = player_right + 160;
		float r_collision = tiles_right - player_pos.x;

		if (t_collision < b_collision && t_collision < l_collision && t_collision < r_collision) {
			//Top collision
			std::cout << "collide top" << std::endl;
			if (player_direction.y == 1) {
				std::cout << "move top" << std::endl;
				player_direction.y = 0;
			}
			else {
				std::cout << "move bot" << std::endl;
				player_direction.y = -1;
				player_pos.y += player_direction.y;
			}
		}

		if (b_collision < t_collision && b_collision < l_collision && b_collision < r_collision) {
			//bottom collision
			std::cout << "collide botton" << std::endl;
			if (player_direction.y == -1) {
				std::cout << "move top" << std::endl;
				player_direction.y = 0;
			}
			else {
				std::cout << "move bot" << std::endl;
				player_direction.y = 1;
				player_pos.y += player_direction.y;
			}
		}

		if (l_collision < r_collision && l_collision < t_collision && l_collision < b_collision) {
			//Left collision
			std::cout << "collide left" << std::endl;
			if (player_direction.x == 1)
			{
				std::cout << "move top" << std::endl;
				player_direction.x = 0;
			}
			else {
				std::cout << "move bot" << std::endl;
				player_direction.x = -1;
				player_pos.x += player_direction.x;
			}

		}

		if (r_collision < l_collision && r_collision < t_collision && r_collision < b_collision) {
			//Right collision
			std::cout << "collide right" << std::endl;
			if (player_direction.x == -1) {
				std::cout << "move top" << std::endl;
				player_direction.x = 0;
			}
			else {
				std::cout << "move bot" << std::endl;
				player_direction.x = 1;
				player_pos.x += player_direction.x;
			}

		}



	}

	//collision


	// add velo to player_pos
	if (player_direction.x != 0 || player_direction.y != 0) //if player direction is not 0, as you cannot normalize 0.
	{
		AEVec2 temp_velo{ player_direction.x, player_direction.y };
		AEVec2Normalize(&player_direction, &temp_velo); // normalize
		player_direction.x *= speed; // magnitude/speed of velo.x
		player_direction.y *= speed; // magnitude/speed of velo.y
	}

	player_pos.x += player_direction.x * 0.016; // 0.016 is delta time temp value
	player_pos.y += player_direction.y * 0.016;

	if (AEInputCheckTriggered(AEVK_E) == 1 && blackholestate == 1) {
		blackhole.x += 5 * cos(Angle);
		blackhole.y += 5 * sin(Angle);
	}

	if (mapEditor == 1) {
		if (AEInputCheckTriggered(AEVK_K) && mapY < 12) {
			mapY += 1;
		}
		if (AEInputCheckTriggered(AEVK_I) && mapY > 0) {
			mapY -= 1;
		}
		if (AEInputCheckTriggered(AEVK_J) && mapX > 0) {
			mapX -= 1;
		}
		if (AEInputCheckTriggered(AEVK_L) && mapX < 20) {
			mapX += 1;
		}

		for (int j = 0; j < 12; j++) {
			for (int i = 0; i < 20; i++) {
				if (x >= i * 80 && x <= (i + 1) * 80 && y >= j * 80 && y <= (j + 1) * 80 && AEInputCheckTriggered(AEVK_LBUTTON)) {
					maps[i][j].TextureX = mapX;
					maps[i][j].TextureY = mapY;
				}
			}
		}
	}

	if (AEInputCheckCurr(AEVK_LBUTTON)) {
		switch (direction) {
		case 1: //right
			slashX = 50;
			slashY = 0;
			slashRot = 0;
			break;
		case 2: //up
			slashY = 50;
			slashX = 0;
			slashRot = PI / 2;
			break;
		case 3: //left
			slashX = -50;
			slashY = 0;
			slashRot = PI;
			break;
		case 4: //down
			slashY = -50;
			slashX = 0;
			slashRot = -PI / 2;
			break;
		default:
			break;
		}
		slashTrigger = 1;
	}
	else {
		slashTrigger = 0;
	}

	if (AEInputCheckTriggered(AEVK_F3)) {
		state != state;
	}



		if (AEInputCheckTriggered(AEVK_0))
		{
			enable = 1;
			std::cout << "Generate planet earth" << '\n';
		}
		if (enable == 1)
		{
			if (counter > 0 && live > 0)
			{
				char count[20] = "Counter: ";
				sprintf_s(count, "Counter: %d", counter);
				AEGfxPrint(FontList[1], count, 0.15f, 0.90f, 1.0f, 1.0f, 1.0f, 1.0f);
			}

			if (AEInputCheckTriggered(AEVK_LBUTTON))
			{
				if (live > 0 && counter != 0)
				{
					if (utilities::WithinCircle(scaling / 2, x, y, translateX + width / 2, translateY + height / 2) == 1)
					{
						translateX = min_x + rand() % (max_x - min_x + 1);
						translateY = min_y + rand() % (max_y - min_y + 1);
						counter -= 1;
					}
					else
					{
						live -= 1;
					}
				}
			}
			if (live > 0 && counter == 0)
			{
				char count[50] = "Counter: ";
				sprintf_s(count, "Counter Left: %d You Win", counter);
				AEGfxPrint(FontList[1], count, -0.30f, 0.90f, 1.0f, 1.0f, 1.0f, 1.0f);
			}
			if (live == 0 && counter > 0)
			{
				char count[50] = "Counter: ";
				sprintf_s(count, "Counter Left: %d, No More Live. You Lose", counter);
				AEGfxPrint(FontList[1], count, -0.45f, 0.90f, 1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}


/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_World_Draw() {

	// Set the background to black. 
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	// Tell the engine to get ready to draw something with texture. 
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	for (int j = 0; j < 12; j++) {
		for (int i = 0; i < 20; i++) {
			// Tell the engine to get ready to draw something with texture. 
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTransparency(1.0f);
			// Set the texture to spriteSheet
			AEGfxTextureSet(TextureList[0], 16.f / 192 * maps[i][j].TextureX, 16.f / 176 * maps[i][j].TextureY);
			// Create a scale matrix that scales by 100 x and y 
			AEMtx33 scale = { 0 };
			AEMtx33Scale(&scale, 80.f, 80.f);
			// Create a rotation matrix that rotates by 45 degrees 
			AEMtx33 rotate = { 0 };
			AEMtx33Rot(&rotate, 0);
			// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis 
			AEMtx33 translate = { 0 };
			AEMtx33Trans(&translate, -760 + i * 80, 410 - j * 80);
			// Concat the matrices (TRS) 
			AEMtx33 transform = { 0 };
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			// Choose the transform to use 
			AEGfxSetTransform(transform.m);
			// Actually drawing the mesh
			AEGfxMeshDraw(sGameObjList[1].pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	if (mapEditor == 1) {
		// Tell the engine to get ready to draw something with texture. 
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		// Set the texture to spriteSheet
		AEGfxTextureSet(TextureList[0], 16.f / 192 * mapX, 16.f / 176 * mapY);
		// Create a scale matrix that scales by 100 x and y 
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, 80.f, 80.f);
		// Create a rotation matrix that rotates by 45 degrees 
		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, 0);
		// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis 
		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, 570, 320);
		// Concat the matrices (TRS) 
		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		// Choose the transform to use 
		AEGfxSetTransform(transform.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(sGameObjList[1].pMesh, AE_GFX_MDM_TRIANGLES);

		char mapSelect[20] = "Selection";
		AEGfxPrint(FontList[0], mapSelect, -0.99f, 0.90f, 1.5f, 1.0f, 0.2f, 0.2f);
	}
	// Set the texture to pTex 
	AEGfxTextureSet(TextureList[0], 16.f / 192, 16.f / 176 * 8);
	AEMtx33 scale = { 0 };
	// Create a scale matrix that scales by 100 x and y 
	AEMtx33Scale(&scale, charScaleX, 60.f);
	// Create a rotation matrix that rotates by 45 degrees 
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, i);
	// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis 
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, player_pos.x, player_pos.y); // - pos y to match with mouse and player coordinate 
	// Concat the matrices (TRS) 
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	// Choose the transform to use 
	AEGfxSetTransform(transform.m);
	// Actually drawing the mesh
	AEGfxMeshDraw(sGameObjList[1].pMesh, AE_GFX_MDM_TRIANGLES);

	if (blackholestate == 1) {
		// Tell the engine to get ready to draw something with texture. 
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		// Set the texture to pTex 
		AEGfxTextureSet(TextureList[1], 0, 0);
		// Create a scale matrix that scales by 100 x and y 
		AEMtx33 scale2 = { 0 };
		AEMtx33Scale(&scale2, 100.f, 100.f);
		// Create a rotation matrix that rotates by 45 degrees 
		AEMtx33 rotate2 = { 0 };
		AEMtx33Rot(&rotate2, 0);
		// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis 
		AEMtx33 translate2 = { 0 };
		AEMtx33Trans(&translate2, blackhole.x, blackhole.y);
		// Concat the matrices (TRS) 
		AEMtx33 transform2 = { 0 };
		AEMtx33Concat(&transform2, &rotate2, &scale2);
		AEMtx33Concat(&transform2, &translate2, &transform2);
		// Choose the transform to use 
		AEGfxSetTransform(transform2.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(sGameObjList[3].pMesh, AE_GFX_MDM_TRIANGLES);
	}

	// Set the texture to pTex 
	if (slashTrigger == 1) {
		AEGfxTextureSet(TextureList[2], 0, 0);
		// Create a scale matrix that scales by 100 x and y 
		AEMtx33 scale2 = { 0 };
		AEMtx33Scale(&scale2, 100.f, 100.f);
		// Create a rotation matrix that rotates by 45 degrees 
		AEMtx33 rotate2 = { 0 };
		AEMtx33Rot(&rotate2, slashRot);
		// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis 
		AEMtx33 translate2 = { 0 };
		AEMtx33Trans(&translate2, player_pos.x + slashX, player_pos.y + slashY); // - for pos y to match player and mouse coordinate
		// Concat the matrices (TRS) 
		AEMtx33 transform2 = { 0 };
		AEMtx33Concat(&transform2, &rotate2, &scale2);
		AEMtx33Concat(&transform2, &translate2, &transform2);
		// Choose the transform to use 
		AEGfxSetTransform(transform2.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(sGameObjList[0].pMesh, AE_GFX_MDM_TRIANGLES);
	}

	if (counter > 0 && live > 0)
	{
		if (live == 2)
		{
			//first empty heart from the left
			AEGfxTextureSet(TextureList[6], 0, 0);
			AEMtx33 fheart_scale1 = { 0 };
			AEMtx33Scale(&fheart_scale1, scaling, scaling);
			AEMtx33 fheart_rotate1 = { 0 };
			AEMtx33Rot(&fheart_rotate1, 0);
			AEMtx33 fheart_translate1 = { 0 };
			AEMtx33Trans(&fheart_translate1, -52, 325);
			AEMtx33 fheart_transform1 = { 0 };
			AEMtx33Concat(&fheart_transform1, &fheart_rotate1, &fheart_scale1);
			AEMtx33Concat(&fheart_transform1, &fheart_translate1, &fheart_transform1);
			AEGfxSetTransform(fheart_transform1.m);
			AEGfxMeshDraw(sGameObjList[2].pMesh, AE_GFX_MDM_TRIANGLES);
		}

		if (live == 1)
		{
			//first empty heart from the left
			AEGfxTextureSet(TextureList[6], 0, 0);
			AEMtx33 fheart_scale1 = { 0 };
			AEMtx33Scale(&fheart_scale1, scaling, scaling);
			AEMtx33 fheart_rotate1 = { 0 };
			AEMtx33Rot(&fheart_rotate1, 0);
			AEMtx33 fheart_translate1 = { 0 };
			AEMtx33Trans(&fheart_translate1, -52, 325);
			AEMtx33 fheart_transform1 = { 0 };
			AEMtx33Concat(&fheart_transform1, &fheart_rotate1, &fheart_scale1);
			AEMtx33Concat(&fheart_transform1, &fheart_translate1, &fheart_transform1);
			AEGfxSetTransform(fheart_transform1.m);
			AEGfxMeshDraw(sGameObjList[2].pMesh, AE_GFX_MDM_TRIANGLES);

			//second empty heart from the left
			AEGfxTextureSet(TextureList[6], 0, 0);
			AEMtx33 fheart_scale2 = { 0 };
			AEMtx33Scale(&fheart_scale2, scaling, scaling);
			AEMtx33 fheart_rotate2 = { 0 };
			AEMtx33Rot(&fheart_rotate2, 0);
			AEMtx33 fheart_translate2 = { 0 };
			AEMtx33Trans(&fheart_translate2, 0, 325);
			AEMtx33 fheart_transform2 = { 0 };
			AEMtx33Concat(&fheart_transform2, &fheart_rotate2, &fheart_scale2);
			AEMtx33Concat(&fheart_transform2, &fheart_translate2, &fheart_transform2);
			AEGfxSetTransform(fheart_transform2.m);
			AEGfxMeshDraw(sGameObjList[2].pMesh, AE_GFX_MDM_TRIANGLES);
		}
		if (live == 0)
		{
			//first empty heart from the left
			AEGfxTextureSet(TextureList[6], 0, 0);
			AEMtx33 fheart_scale1 = { 0 };
			AEMtx33Scale(&fheart_scale1, scaling, scaling);
			AEMtx33 fheart_rotate1 = { 0 };
			AEMtx33Rot(&fheart_rotate1, 0);
			AEMtx33 fheart_translate1 = { 0 };
			AEMtx33Trans(&fheart_translate1, -52, 325);
			AEMtx33 fheart_transform1 = { 0 };
			AEMtx33Concat(&fheart_transform1, &fheart_rotate1, &fheart_scale1);
			AEMtx33Concat(&fheart_transform1, &fheart_translate1, &fheart_transform1);
			AEGfxSetTransform(fheart_transform1.m);
			AEGfxMeshDraw(sGameObjList[2].pMesh, AE_GFX_MDM_TRIANGLES);

			//second empty heart from the left
			AEGfxTextureSet(TextureList[6], 0, 0);
			AEMtx33 fheart_scale2 = { 0 };
			AEMtx33Scale(&fheart_scale2, scaling, scaling);
			AEMtx33 fheart_rotate2 = { 0 };
			AEMtx33Rot(&fheart_rotate2, 0);
			AEMtx33 fheart_translate2 = { 0 };
			AEMtx33Trans(&fheart_translate2, 0, 325);
			AEMtx33 fheart_transform2 = { 0 };
			AEMtx33Concat(&fheart_transform2, &fheart_rotate2, &fheart_scale2);
			AEMtx33Concat(&fheart_transform2, &fheart_translate2, &fheart_transform2);
			AEGfxSetTransform(fheart_transform2.m);
			AEGfxMeshDraw(sGameObjList[2].pMesh, AE_GFX_MDM_TRIANGLES);

			//third empty heart from the left
			AEGfxTextureSet(TextureList[6], 0, 0);
			AEMtx33 fheart_scale3 = { 0 };
			AEMtx33Scale(&fheart_scale3, scaling, scaling);
			AEMtx33 fheart_rotate3 = { 0 };
			AEMtx33Rot(&fheart_rotate3, 0);
			AEMtx33 fheart_translate3 = { 0 };
			AEMtx33Trans(&fheart_translate3, 52, 325);
			AEMtx33 fheart_transform3 = { 0 };
			AEMtx33Concat(&fheart_transform3, &fheart_rotate3, &fheart_scale3);
			AEMtx33Concat(&fheart_transform3, &fheart_translate3, &fheart_transform3);
			AEGfxSetTransform(fheart_transform3.m);
			AEGfxMeshDraw(sGameObjList[2].pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
	if (enable == 1)
	{
		if (counter > 0 && live > 0)
		{
			//creating the planetearh
			AEGfxTextureSet(TextureList[4], 0, 0);
			AEMtx33 scale = { 0 };
			AEMtx33Scale(&scale, scaling, scaling);
			AEMtx33 rotate = { 0 };
			AEMtx33Rot(&rotate, 0);
			AEMtx33 translate = { 0 };
			AEMtx33Trans(&translate, translateX, -(translateY));
			AEMtx33 transform = { 0 };
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(sGameObjList[0].pMesh, AE_GFX_MDM_TRIANGLES);

			if (live == 3)
			{
				//first heart from the left
				AEGfxTextureSet(TextureList[5], 0, 0);
				AEMtx33 fheart_scale1 = { 0 };
				AEMtx33Scale(&fheart_scale1, scaling, scaling);
				AEMtx33 fheart_rotate1 = { 0 };
				AEMtx33Rot(&fheart_rotate1, 0);
				AEMtx33 fheart_translate1 = { 0 };
				AEMtx33Trans(&fheart_translate1, -52, 325);
				AEMtx33 fheart_transform1 = { 0 };
				AEMtx33Concat(&fheart_transform1, &fheart_rotate1, &fheart_scale1);
				AEMtx33Concat(&fheart_transform1, &fheart_translate1, &fheart_transform1);
				AEGfxSetTransform(fheart_transform1.m);
				AEGfxMeshDraw(sGameObjList[2].pMesh, AE_GFX_MDM_TRIANGLES);

				//second heart from the left
				AEGfxTextureSet(TextureList[5], 0, 0);
				AEMtx33 fheart_scale2 = { 0 };
				AEMtx33Scale(&fheart_scale2, scaling, scaling);
				AEMtx33 fheart_rotate2 = { 0 };
				AEMtx33Rot(&fheart_rotate2, 0);
				AEMtx33 fheart_translate2 = { 0 };
				AEMtx33Trans(&fheart_translate2, 0, 325);
				AEMtx33 fheart_transform2 = { 0 };
				AEMtx33Concat(&fheart_transform2, &fheart_rotate2, &fheart_scale2);
				AEMtx33Concat(&fheart_transform2, &fheart_translate2, &fheart_transform2);
				AEGfxSetTransform(fheart_transform2.m);
				AEGfxMeshDraw(sGameObjList[2].pMesh, AE_GFX_MDM_TRIANGLES);

				//third heart from the left
				AEGfxTextureSet(TextureList[5], 0, 0);
				AEMtx33 fheart_scale3 = { 0 };
				AEMtx33Scale(&fheart_scale3, scaling, scaling);
				AEMtx33 fheart_rotate3 = { 0 };
				AEMtx33Rot(&fheart_rotate3, 0);
				AEMtx33 fheart_translate3 = { 0 };
				AEMtx33Trans(&fheart_translate3, 52, 325);
				AEMtx33 fheart_transform3 = { 0 };
				AEMtx33Concat(&fheart_transform3, &fheart_rotate3, &fheart_scale3);
				AEMtx33Concat(&fheart_transform3, &fheart_translate3, &fheart_transform3);
				AEGfxSetTransform(fheart_transform3.m);
				AEGfxMeshDraw(sGameObjList[2].pMesh, AE_GFX_MDM_TRIANGLES);
			}
			else if (live == 2)
			{
				//second heart from the left
				AEGfxTextureSet(TextureList[5], 0, 0);
				AEMtx33 fheart_scale2 = { 0 };
				AEMtx33Scale(&fheart_scale2, scaling, scaling);
				AEMtx33 fheart_rotate2 = { 0 };
				AEMtx33Rot(&fheart_rotate2, 0);
				AEMtx33 fheart_translate2 = { 0 };
				AEMtx33Trans(&fheart_translate2, 0, 325);
				AEMtx33 fheart_transform2 = { 0 };
				AEMtx33Concat(&fheart_transform2, &fheart_rotate2, &fheart_scale2);
				AEMtx33Concat(&fheart_transform2, &fheart_translate2, &fheart_transform2);
				AEGfxSetTransform(fheart_transform2.m);
				AEGfxMeshDraw(sGameObjList[2].pMesh, AE_GFX_MDM_TRIANGLES);

				//third heart from the left
				AEGfxTextureSet(TextureList[5], 0, 0);
				AEMtx33 fheart_scale3 = { 0 };
				AEMtx33Scale(&fheart_scale3, scaling, scaling);
				AEMtx33 fheart_rotate3 = { 0 };
				AEMtx33Rot(&fheart_rotate3, 0);
				AEMtx33 fheart_translate3 = { 0 };
				AEMtx33Trans(&fheart_translate3, 52, 325);
				AEMtx33 fheart_transform3 = { 0 };
				AEMtx33Concat(&fheart_transform3, &fheart_rotate3, &fheart_scale3);
				AEMtx33Concat(&fheart_transform3, &fheart_translate3, &fheart_transform3);
				AEGfxSetTransform(fheart_transform3.m);
				AEGfxMeshDraw(sGameObjList[2].pMesh, AE_GFX_MDM_TRIANGLES);
			}
			else if (live == 1)
			{
				//third heart from the left
				AEGfxTextureSet(TextureList[5], 0, 0);
				AEMtx33 fheart_scale3 = { 0 };
				AEMtx33Scale(&fheart_scale3, scaling, scaling);
				AEMtx33 fheart_rotate3 = { 0 };
				AEMtx33Rot(&fheart_rotate3, 0);
				AEMtx33 fheart_translate3 = { 0 };
				AEMtx33Trans(&fheart_translate3, 52, 325);
				AEMtx33 fheart_transform3 = { 0 };
				AEMtx33Concat(&fheart_transform3, &fheart_rotate3, &fheart_scale3);
				AEMtx33Concat(&fheart_transform3, &fheart_translate3, &fheart_transform3);
				AEGfxSetTransform(fheart_transform3.m);
				AEGfxMeshDraw(sGameObjList[2].pMesh, AE_GFX_MDM_TRIANGLES);


			}
		}
	}
	if (state == 1)
	{
		AEInputGetCursorPosition(&mx, &my);
		u8 keyw = AEInputCheckCurr(AEVK_W);
		u8 keya = AEInputCheckCurr(AEVK_A);
		u8 keys = AEInputCheckCurr(AEVK_S);
		u8 keyd = AEInputCheckCurr(AEVK_D);
		char debug[20] = "Debug Screen";
		char input[20] = "Input";
		char a[20] = "A";
		char w[20] = "W";
		char s[20] = "S";
		char d[20] = "D";
		char playerpos[100] = { player_pos.x,player_pos.y };
		AEGfxPrint(FontList[0], debug, -0.99f, 0.90f, 1.5f, 1.0f, 0.2f, 0.2f);
		AEGfxPrint(FontList[0], input, -0.99f, 0.65f, 1.5f, 1.0f, 0.2f, 0.2f);
		char mouse_xy_buffer[50] = " "; // buffer
		sprintf_s(mouse_xy_buffer, "Mouse Position X: %d", mx);
		AEGfxPrint(FontList[0], mouse_xy_buffer, -0.99f, 0.76f, 1.0f, 1.0f, 0.2f, 0.2f);
		sprintf_s(mouse_xy_buffer, "Mouse Position Y: %d", my);
		AEGfxPrint(FontList[0], mouse_xy_buffer, -0.99f, 0.71f, 1.0f, 1.0f, 0.2f, 0.2f);
		AEGfxPrint(FontList[0], a, -0.99f, 0.55f, 1.0f, 1.0f, 0.2f, 0.2f);
		AEGfxPrint(FontList[0], w, -0.99f, 0.60f, 1.0f, 1.0f, 0.2f, 0.2f);
		AEGfxPrint(FontList[0], s, -0.99f, 0.50f, 1.0f, 1.0f, 0.2f, 0.2f);
		AEGfxPrint(FontList[0], d, -0.99f, 0.45f, 1.0f, 1.0f, 0.2f, 0.2f);
		sprintf_s(playerpos, "Player Position X: %d", int(player_pos.x));
		AEGfxPrint(FontList[0], playerpos, -0.99f, 0.40f, 1.0f, 1.0f, 0.2f, 0.2f);
		sprintf_s(playerpos, "Player Position Y: %d", int(player_pos.y));
		AEGfxPrint(FontList[0], playerpos, -0.99f, 0.35f, 1.0f, 1.0f, 0.2f, 0.2f);


		if (AEInputCheckCurr(AEVK_W))
		{
			sprintf_s(w, "W Pressed", keyw);
			AEGfxPrint(FontList[0], w, -0.99f, 0.60f, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (AEInputCheckCurr(AEVK_A))
		{
			sprintf_s(a, "A Pressed", keya);
			AEGfxPrint(FontList[0], a, -0.99f, 0.55f, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (AEInputCheckCurr(AEVK_S))
		{
			sprintf_s(s, "S Pressed", keys);
			AEGfxPrint(FontList[0], s, -0.99f, 0.50f, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (AEInputCheckCurr(AEVK_D))
		{
			sprintf_s(s, "D Pressed", keyd);
			AEGfxPrint(FontList[0], s, -0.99f, 0.45f, 1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
}

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the World level.
*/
/******************************************************************************/
void GS_World_Free() {
	;
}

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	World level.
*/
/******************************************************************************/
void GS_World_Unload() {
	for (int i = 0; i < sGameObjNum; i++) {
		AEGfxMeshFree(sGameObjList[i].pMesh);
	}

	for (int i = 0; i < TextureListNum; i++) {
		AEGfxTextureUnload(TextureList[i]);
	}
}