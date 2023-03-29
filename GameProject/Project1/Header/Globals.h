#pragma once
#ifndef GLOBALS
#define GLOBALS

static const unsigned int	GAME_OBJ_NUM_MAX = 32;				// The total number of unique objects (Shapes)
static const unsigned int	TEXTURE_NUM_MAX = 32;				// The total number of Textures
static const unsigned int	GAME_OBJ_INST_NUM_MAX = 2048;			// The total number of dynamic game object instances
static const unsigned int	FONT_NUM_MAX = 10;					// The total number of fonts
static const unsigned int	STATIC_OBJ_INST_NUM_MAX = 1024;		// The total number of static game object instances

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

extern bool					levelCleared[4];

extern const unsigned long FLAG_ACTIVE;			// For whether object instance is active


extern const int			SPRITE_SCALE;			// Scale of sprites
extern const int			TEXTURE_MAXWIDTH;		// Canvas Width of sprite sheet
extern const int			TEXTURE_MAXHEIGHT;		// Canvas Height of sprite sheet
extern const float			TEXTURE_CELLSIZE;		// Number of pixel size for each sprite

extern const int			COLLISION_LEFT;			// 0001
extern const int			COLLISION_RIGHT;		// 0010
extern const int			COLLISION_TOP;			// 0100
extern const int			COLLISION_BOTTOM;		// 1000

extern const float			TOWER_RIGHT;
extern const float			TOWER_LEFT;
extern const float			TOWER_UP;
extern const float			TOWER_DOWN;

extern const int			CAM_CELL_WIDTH;			// Number of cells in viewport for width
extern const int			CAM_CELL_HEIGHT;		// Number of cells in viewport for height

extern const float			BOUNDING_RECT_SIZE;		// this is the normalized bounding rectangle (width and height) sizes - AABB collision data
extern const float			PLAYER_SPEED;			// Player movement sped
extern const float			NPC_SPEED;				// Mob movement speed
extern const float			BULLET_SPEED;

extern const float			SLASH_COOLDOWN_t;		// Time for slash to cooldown
extern const float			WALK_COOLDOWN_t;		// Time for walk to be stopped after slash
extern const float			DAMAGE_COODLDOWN_t;		// Time for damage invulenrability to reset
extern const float			SPIKE_TRANSITION_t;		// Time for spike to fade in/out
extern const float			SPIKE_DEATH_t;			// Duration for spike to be non-existent
extern const float			SPIKE_LIFE_t;			// Duration for spike to be existent

extern const float			TOWER_REFRESH;			// Rate at which tower shoot bullets

extern const float			aoeREFRESH;
extern const float			challengeATKREFRESH;

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


extern bool damageFlag;
extern AEAudio HeroDamaged;
extern AEAudioGroup Damage;
extern AEAudio HeroSlash;
extern AEAudio BulletShot;
extern AEAudioGroup BulletGroup;


static float				mouseX;					// Position of mouse X
static float				mouseY;					// Position of mouse Y

static f32					camX;					// Position of camera X
static f32					camY;					// Position of camera Y

enum levelCleared  {
	tower = 0,
	maze,
	colosseum,
	blackhole
};

#endif
