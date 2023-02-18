#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H
const int			SPRITE_SCALE = 80;				// Scale of sprites
const int			TEXTURE_MAXWIDTH = 192;			// Canvas Width of sprite sheet
const int			TEXTURE_MAXHEIGHT = 192;		// Canvas Height of sprite sheet
const float			TEXTURE_CELLSIZE = 16;			// Number of pixel size for each sprite

const int			COLLISION_LEFT = 0x00000001;	// 0001
const int			COLLISION_RIGHT = 0x00000002;	// 0010
const int			COLLISION_TOP = 0x00000004;		// 0100
const int			COLLISION_BOTTOM = 0x00000008;	// 1000

const int			CAM_CELL_WIDTH = 20;			// Number of cells in viewport for width
const int			CAM_CELL_HEIGHT = 12;			// Number of cells in viewport for height

const float			BOUNDING_RECT_SIZE = 1.0f;		// this is the normalized bounding rectangle (width and height) sizes - AABB collision data
const float			PLAYER_SPEED = 5.f;				// Player movement sped
const float			NPC_SPEED = 2.5f;				// Mob movement speed

float				mouseX;							// Position of mouse X
float				mouseY;							// Position of mouse Y

f32					camX;							// Position of camera X
f32					camY;							// Position of camera Y
#else
extern const int			SPRITE_SCALE;			// Scale of sprites
extern const int			TEXTURE_MAXWIDTH;		// Canvas Width of sprite sheet
extern const int			TEXTURE_MAXHEIGHT;		// Canvas Height of sprite sheet
extern const float			TEXTURE_CELLSIZE;			// Number of pixel size for each sprite

extern const int			COLLISION_LEFT;	// 0001
extern const int			COLLISION_RIGHT;	// 0010
extern const int			COLLISION_TOP;		// 0100
extern const int			COLLISION_BOTTOM;	// 1000

extern const int			CAM_CELL_WIDTH;			// Number of cells in viewport for width
extern const int			CAM_CELL_HEIGHT;			// Number of cells in viewport for height

extern const float			BOUNDING_RECT_SIZE;		// this is the normalized bounding rectangle (width and height) sizes - AABB collision data
extern const float			PLAYER_SPEED;				// Player movement sped
extern const float			NPC_SPEED;				// Mob movement speed

extern float				mouseX;							// Position of mouse X
extern float				mouseY;							// Position of mouse Y

extern f32					camX;							// Position of camera X
extern f32					camY;							// Position of camera Y
#endif // GLOBAL_H