#include "main.h"
const int			SPRITE_SCALE = 80;				// Scale of sprites
const int			TEXTURE_MAXWIDTH = 192;			// Canvas Width of sprite sheet
const int			TEXTURE_MAXHEIGHT = 208;		// Canvas Height of sprite sheet
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
const float			BULLET_SPEED = 4.5f;