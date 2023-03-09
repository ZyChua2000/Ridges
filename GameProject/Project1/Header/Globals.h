#pragma once
#ifndef GLOBALS
#define GLOBALS

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

extern const AEVec2			TEXTURE_POTION;
extern const AEVec2			TEXTURE_TOWER;
extern const AEVec2			TEXTURE_BULLET;
extern const AEVec2			TEXTURE_ENEMY;
extern const AEVec2			TEXTURE_SPIKE;
extern const AEVec2			TEXTURE_UNOPENEDCHEST;
extern const AEVec2			TEXTURE_OPENEDCHEST;
extern const AEVec2			TEXTURE_FULLHEART;
extern const AEVec2			TEXTURE_DEADHEART;
extern const AEVec2			TEXTURE_LEVERS;
extern const AEVec2			TEXTURE_KEYS;
extern const AEVec2			TEXTURE_NUMBERS[10];
extern const AEVec2			TEXTURE_PLAYER;


static float				mouseX;					// Position of mouse X
static float				mouseY;					// Position of mouse Y

static f32					camX;					// Position of camera X
static f32					camY;					// Position of camera Y


#endif
