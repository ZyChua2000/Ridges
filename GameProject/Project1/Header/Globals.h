#pragma once

static const int			SPRITE_SCALE = 80;				// Scale of sprites
static const int			TEXTURE_MAXWIDTH = 192;			// Canvas Width of sprite sheet
static const int			TEXTURE_MAXHEIGHT = 208;		// Canvas Height of sprite sheet
static const float			TEXTURE_CELLSIZE = 16;			// Number of pixel size for each sprite

static const int			COLLISION_LEFT = 0x00000001;	// 0001
static const int			COLLISION_RIGHT = 0x00000002;	// 0010
static const int			COLLISION_TOP = 0x00000004;		// 0100
static const int			COLLISION_BOTTOM = 0x00000008;	// 1000

static const int			CAM_CELL_WIDTH = 20;			// Number of cells in viewport for width
static const int			CAM_CELL_HEIGHT = 12;			// Number of cells in viewport for height

static const float			BOUNDING_RECT_SIZE = 1.0f;		// this is the normalized bounding rectangle (width and height) sizes - AABB collision data
static const float			PLAYER_SPEED = 5.f;				// Player movement sped
static const float			NPC_SPEED = 2.5f;				// Mob movement speed

static float				mouseX;							// Position of mouse X
static float				mouseY;							// Position of mouse Y

static f32					camX;							// Position of camera X
static f32					camY;							// Position of camera Y

