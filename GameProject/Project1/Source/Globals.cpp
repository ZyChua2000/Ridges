#include "main.h"

// list of original object
GameObj				sGameObjList[GAME_OBJ_NUM_MAX] = { 0 };				// Each element in this array represents a unique game object (shape)
unsigned long		sGameObjNum = 0;								// The number of defined game objects

// list of object instances
GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX] = { 0 };	// Each element in this array represents a dynamic unique game object instance (sprite)
unsigned long		sGameObjInstNum = 0;							// The number of used dynamic game object instances

// list of static instances
staticObjInst		sStaticObjInstList[STATIC_OBJ_INST_NUM_MAX] = { 0 };// Each element in this array represents a unique static game object instance (sprite)
unsigned long		sStaticObjInstNum = 0;							// The number of used static game object instances

s8					FontList[FONT_NUM_MAX] = { 0 };						// Each element in this array represents a Font
unsigned long		FontListNum = 0;								// The number of used fonts

const unsigned long FLAG_ACTIVE = 0x00000001;			// For whether object instance is active

const int			SPRITE_SCALE = 80;				// Scale of sprites
const int			TEXTURE_MAXWIDTH = 192;			// Canvas Width of sprite sheet
const int			TEXTURE_MAXHEIGHT = 208;		// Canvas Height of sprite sheet
const float			TEXTURE_CELLSIZE = 16;			// Number of pixel size for each sprite

const int			COLLISION_LEFT = 0x00000001;	// 0001
const int			COLLISION_RIGHT = 0x00000002;	// 0010
const int			COLLISION_TOP = 0x00000004;		// 0100
const int			COLLISION_BOTTOM = 0x00000008;	// 1000

const float			TOWER_RIGHT = PI/2;
const float			TOWER_LEFT = -PI/2;
const float			TOWER_UP = PI;
const float			TOWER_DOWN = 0;

const int			CAM_CELL_WIDTH = 20;			// Number of cells in viewport for width
const int			CAM_CELL_HEIGHT = 12;			// Number of cells in viewport for height

const float			BOUNDING_RECT_SIZE = 1.0f;		// this is the normalized bounding rectangle (width and height) sizes - AABB collision data
const float			PLAYER_SPEED = 5.f;				// Player movement sped
const float			NPC_SPEED = 2.5f;				// Mob movement speed
const float			BULLET_SPEED = 4.5f;			// Speed of the bullet

const float			SLASH_COOLDOWN_t = 0.5f;		// Time for slash to cooldown
const float			WALK_COOLDOWN_t = 0.2f;			// Time for walk to be stopped after slash
const float			DAMAGE_COODLDOWN_t = 0.5f;		// Time for damage invulenrability to reset
const float			SPIKE_TRANSITION_t = 0.5f;		// Time for spike to fade in/out
const float			SPIKE_DEATH_t = 0.5f;			// Duration for spike to be non-existent
const float			SPIKE_LIFE_t = 2.f;				// Duration for spike to be existent

const AEVec2		TEXTURE_POTION = { 6,9 };
const AEVec2		TEXTURE_TOWER = { 2,6 };
const AEVec2		TEXTURE_BULLET = { 5,12 };
const AEVec2		TEXTURE_ENEMY = { 0,9 };
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

const AEVec2		TEXTURE_PLAYER = { 1,8 };
const float			TOWER_REFRESH = 2.f;			// Rate at which tower shoot bullets