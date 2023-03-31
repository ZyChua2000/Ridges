#pragma once
#ifndef GLOBALS
#define GLOBALS

/******************************************************************************/
/*!
	Objects-related Globals for all levels
*/
/******************************************************************************/

static const unsigned int	GAME_OBJ_NUM_MAX = 32;				// The total number of unique objects (Shapes)
static const unsigned int	TEXTURE_NUM_MAX = 32;				// The total number of Textures
static const unsigned int	GAME_OBJ_INST_NUM_MAX = 124;			// The total number of dynamic game object instances
static const unsigned int	FONT_NUM_MAX = 10;					// The total number of fonts
static const unsigned int	STATIC_OBJ_INST_NUM_MAX = 124;		// The total number of static game object instances

// list of original object
extern GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
extern unsigned long		sGameObjNum;								// The number of defined game objects

// list of object instances
extern GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a dynamic unique game object instance (sprite)
extern unsigned long		sGameObjInstNum;							// The number of used dynamic game object instances

// list of static instances
extern staticObjInst		sStaticObjInstList[STATIC_OBJ_INST_NUM_MAX];// Each element in this array represents a unique static game object instance (sprite)
extern unsigned long		sStaticObjInstNum;							// The number of used static game object instances

extern s8					FontList[FONT_NUM_MAX];						// Each element in this array represents a Font
extern unsigned long		FontListNum;								// The number of used fonts

extern const unsigned long FLAG_ACTIVE;			// For whether object instance is active

/******************************************************************************/
/*!
	Cell related globals
*/
/******************************************************************************/

extern const int			SPRITE_SCALE;			// Scale of sprites
extern const int			TEXTURE_MAXWIDTH;		// Canvas Width of sprite sheet
extern const int			TEXTURE_MAXHEIGHT;		// Canvas Height of sprite sheet
extern const float			TEXTURE_CELLSIZE;		// Number of pixel size for each sprite
extern const int			CAM_CELL_WIDTH;			// Number of cells in viewport for width
extern const int			CAM_CELL_HEIGHT;		// Number of cells in viewport for height

/******************************************************************************/
/*!
	Texture related globals
*/
/******************************************************************************/
extern const AEVec2			TEXTURE_POTION;
extern const AEVec2			TEXTURE_TOWER;
extern const AEVec2			TEXTURE_BULLET;
extern const AEVec2			TEXTURE_ENEMY;
extern const AEVec2			TEXTURE_BOSS;
extern const AEVec2			TEXTURE_SPIKE;
extern const AEVec2			TEXTURE_UNOPENEDCHEST;
extern const AEVec2			TEXTURE_OPENEDCHEST;
extern const AEVec2			TEXTURE_FULLHEART;
extern const AEVec2			TEXTURE_DEADHEART;
extern const AEVec2			TEXTURE_LEVERS;
extern const AEVec2			TEXTURE_KEYS;
extern const AEVec2			TEXTURE_NUMBERS[10];
extern const AEVec2			TEXTURE_PLAYER;
extern const AEVec2			TEXTURE_FLOOR;
extern const AEVec2			TEXTURE_FENCE;
extern const AEVec2			TEXTURE_BOSSCIRCLE;

/******************************************************************************/
/*!
	Collision related globals
*/
/******************************************************************************/

extern const int			COLLISION_LEFT;			// 0001
extern const int			COLLISION_RIGHT;		// 0010
extern const int			COLLISION_TOP;			// 0100
extern const int			COLLISION_BOTTOM;		// 1000
extern const float			BOUNDING_RECT_SIZE;		// this is the normalized bounding rectangle (width and height) sizes - AABB collision data

/******************************************************************************/
/*!
	Logic related globals
*/
/******************************************************************************/

//Enemy globals
extern const float			NPC_SPEED;				// Mob movement speed
extern const float			enemySightRange;		// Distance from which mobs will pathfind towards player

//Tower Globals
extern const float			TOWER_REFRESH;			// Rate at which tower shoot bullets
extern const float			BULLET_SPEED;			// Bullet movement speed
extern const float			TOWER_RIGHT;			// Direction of tower facing right
extern const float			TOWER_LEFT;				// Direction of tower facing left
extern const float			TOWER_UP;				// Direction of tower facing up
extern const float			TOWER_DOWN;				// Direction of tower facing down

//Boss Globals
extern const float			aoeREFRESH;				// Time for AOE attack to be refreshed
extern const float			challengeATKREFRESH;	// Time for Challenges attack to be refreshed

//Spike globals
extern const float			SPIKE_TRANSITION_t;		// Time for spike to fade in/out
extern const float			SPIKE_DEATH_t;			// Duration for spike to be non-existent
extern const float			SPIKE_LIFE_t;			// Duration for spike to be existent

//Player globals
extern const float			PLAYER_SPEED;			// Player movement speed
extern const int			MAX_PLAYER_HEALTH;		// Max player health
extern const float			pickUpRange;			// Range for player to pick up items
extern const float			slashRange;				// Range of distance for slash to hit enemy
extern const float			SLASH_COOLDOWN_t;		// Time for slash to cooldown
extern const float			WALK_COOLDOWN_t;		// Time for walk to be stopped after slash
extern const float			DAMAGE_COODLDOWN_t;		// Time for damage invulenrability to reset

/******************************************************************************/
/*!
	Audio related globals
*/
/******************************************************************************/

extern AEAudio				HeroDamaged;
extern AEAudio				HeroSlash;
extern AEAudio				BulletShot;
extern AEAudio				Interact;
extern AEAudio				Movement;
extern AEAudioGroup			InteractGroup;
extern AEAudioGroup			BulletGroup;
extern AEAudioGroup			MovementGroup;
extern AEAudioGroup			Damage;

/******************************************************************************/
/*!
	Other globals
*/
/******************************************************************************/

extern bool					damageFlag;
static float				mouseX;					// Position of mouse X
static float				mouseY;					// Position of mouse Y
static f32					camX;					// Position of camera X
static f32					camY;					// Position of camera Y

extern bool					levelCleared[3];		// Array of boolean checking which level is cleared

enum levelCleared  {
	tower = 0,
	maze,
	colosseum,
};

#endif