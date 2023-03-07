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

extern const int			CAM_CELL_WIDTH;			// Number of cells in viewport for width
extern const int			CAM_CELL_HEIGHT;		// Number of cells in viewport for height

extern const float			BOUNDING_RECT_SIZE;		// this is the normalized bounding rectangle (width and height) sizes - AABB collision data
extern const float			PLAYER_SPEED;			// Player movement sped
extern const float			NPC_SPEED;				// Mob movement speed
extern const float			BULLET_SPEED;

static float				mouseX;					// Position of mouse X
static float				mouseY;					// Position of mouse Y

static f32					camX;					// Position of camera X
static f32					camY;					// Position of camera Y
#endif
