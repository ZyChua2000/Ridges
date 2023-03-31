#include "main.h"

/******************************************************************************/
/*!
\file		Globals.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 02, 2023
\brief		This source file contains the definition for all global variables

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

/******************************************************************************/
/*!
	Objects-related Globals for all levels
*/
/******************************************************************************/

// list of original object
GameObj				sGameObjList[GAME_OBJ_NUM_MAX] = { 0 };				// Each element in this array represents a unique game object (shape)
unsigned long		sGameObjNum = 0;									// The number of defined game objects

// list of object instances
GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX] = { 0 };	// Each element in this array represents a dynamic unique game object instance (sprite)
unsigned long		sGameObjInstNum = 0;								// The number of used dynamic game object instances

// list of static instances
staticObjInst		sStaticObjInstList[STATIC_OBJ_INST_NUM_MAX] = { 0 };// Each element in this array represents a unique static game object instance (sprite)
unsigned long		sStaticObjInstNum = 0;								// The number of used static game object instances

s8					FontList[FONT_NUM_MAX] = { 0 };						// Each element in this array represents a Font
unsigned long		FontListNum = 0;									// The number of used fonts

const unsigned long FLAG_ACTIVE = 0x00000001;							// For whether object instance is active

std::vector<AEGfxTexture*> textureList;									// Dynamic List of textures
std::vector<AEGfxVertexList*> meshList;									// Dynamic list of meshes

/******************************************************************************/
/*!
	Cell related globals
*/
/******************************************************************************/

const int			SPRITE_SCALE = 80;				// Scale of sprites
const int			TEXTURE_MAXWIDTH = 192;			// Canvas Width of sprite sheet
const int			TEXTURE_MAXHEIGHT = 208;		// Canvas Height of sprite sheet
const float			TEXTURE_CELLSIZE = 16;			// Number of pixel size for each sprite
const int			CAM_CELL_WIDTH = 20;			// Number of cells in viewport for width
const int			CAM_CELL_HEIGHT = 12;			// Number of cells in viewport for height

/******************************************************************************/
/*!
	Collision related globals
*/
/******************************************************************************/

const int			COLLISION_LEFT = 0x00000001;	// 0001
const int			COLLISION_RIGHT = 0x00000002;	// 0010
const int			COLLISION_TOP = 0x00000004;		// 0100
const int			COLLISION_BOTTOM = 0x00000008;	// 1000
const float			BOUNDING_RECT_SIZE = 1.0f;		// this is the normalized bounding rectangle (width and height) sizes - AABB collision data

/******************************************************************************/
/*!
	Texture related globals
*/
/******************************************************************************/

const AEVec2		TEXTURE_POTION = { 6,9 };
const AEVec2		TEXTURE_TOWER = { 2,6 };
const AEVec2		TEXTURE_BULLET = { 5,12 };
const AEVec2		TEXTURE_ENEMY = { 0,9 };
const AEVec2		TEXTURE_BOSS = { 3,9 };
const AEVec2		TEXTURE_SPIKE = { 5,3 };
const AEVec2		TEXTURE_UNOPENEDCHEST = { 5,7 };
const AEVec2		TEXTURE_OPENEDCHEST = { 8,7 };
const AEVec2		TEXTURE_FULLHEART = { 0,11 };
const AEVec2		TEXTURE_DEADHEART = { 1,11 };
const AEVec2		TEXTURE_LEVERS = { 2,11 };
const AEVec2		TEXTURE_KEYS = { 4,11 };
const AEVec2		TEXTURE_NUMBERS[10] = { {2,12},{5,11},{6,11},
											{7,11},{8,11},{9,11},
											{10,11},{11,11},{0,12},
											{1,12} };
const AEVec2		TEXTURE_FLOOR = { 0,4 };
const AEVec2		TEXTURE_FENCE = { 5,6 };
const AEVec2		TEXTURE_BOSSCIRCLE = {6,12};

const AEVec2		TEXTURE_PLAYER = { 1,8 };


/******************************************************************************/
/*!
	Logic related globals
*/
/******************************************************************************/

//Enemy globals
const float			NPC_SPEED = 2.5f;				// Mob movement speed
const float			enemySightRange = 10.f;			// Distance from which mobs will pathfind towards player

//Tower Globals
const float			TOWER_REFRESH = 2.f;			// Rate at which tower shoot bullets
const float			BULLET_SPEED = 4.5f;			// Speed of the bullet
const float			TOWER_RIGHT = PI / 2;			// Direction of tower facing right
const float			TOWER_LEFT = -PI / 2;			// Direction of tower facing left
const float			TOWER_UP = PI;					// Direction of tower facing up
const float			TOWER_DOWN = 0;					// Direction of tower facing down

//Boss Globals
const float			aoeREFRESH = 5.0f;				// Time for AOE attack to be refreshed
const float			challengeATKREFRESH = 10.f;		// Time for Challenges attack to be refreshed

//Spike Globals
const float			SPIKE_TRANSITION_t = 0.5f;		// Time for spike to fade in/out
const float			SPIKE_DEATH_t = 0.5f;			// Duration for spike to be non-existent
const float			SPIKE_LIFE_t = 2.f;				// Duration for spike to be existent

//Player Globals
const float			PLAYER_SPEED = 5.f;				// Player movement speed
const int			MAX_PLAYER_HEALTH = 3;			// Max player health
const float			pickUpRange = 0.5f;				// Range for player to pick up items
const float			slashRange = 0.9f;				// Range of distance for slash to hit enemy
const float			SLASH_COOLDOWN_t = 0.5f;		// Time for slash to cooldown
const float			WALK_COOLDOWN_t = 0.2f;			// Time for walk to be stopped after slash
const float			DAMAGE_COODLDOWN_t = 1.5f;		// Time for damage invulenrability to reset


/******************************************************************************/
/*!
	Audio related globals
*/
/******************************************************************************/
AEAudio HeroDamaged = AEAudioLoadMusic("Assets/Music/HUMAN-GRUNT_GEN-HDF-15047.wav");
AEAudio HeroSlash = AEAudioLoadMusic("Assets/Music/METAL-HIT_GEN-HDF-17085.wav");
AEAudio Interact = AEAudioLoadMusic("Assets/Music/SWITCH-LEVER_GEN-HDF-22196.wav");
AEAudio BulletShot = AEAudioLoadMusic("Assets/Music/SCI-FI-LASER_GEN-HDF-20725.wav");
AEAudio Movement = AEAudioLoadMusic("Assets/Music/FOOTSTEPS-OUTDOOR_GEN-HDF-12363.mp3");;
AEAudioGroup MovementGroup = AEAudioCreateGroup();
AEAudioGroup BulletGroup = AEAudioCreateGroup();
AEAudioGroup InteractGroup = AEAudioCreateGroup();
AEAudioGroup Damage = AEAudioCreateGroup();

/******************************************************************************/
/*!
	Other globals
*/
/******************************************************************************/
bool				levelCleared[3] = { false };	// Array of boolean checking which level is cleared
bool				damageFlag = 0;

bool pause = 0;										// Bool of pause state
bool levelstart = 0;								// Bool of level instruction state
int cycle = 0;