#include "main.h"
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
const float			TOWER_UP = 0;
const float			TOWER_DOWN = PI;

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

const float			TOWER_REFRESH = 2.f;			// Rate at which tower shoot bullets