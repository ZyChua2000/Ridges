/******************************************************************************/
/*!
\file		BossLevel.h
\author 	Javier Low Zhi Liang
\par    	email: zhiliangjavier.low@digipen.edu
\date   	February 02, 2023
\date   	February 02, 2023
\brief		This header file contains the functions for the level of Boss Level.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/



// ---------------------------------------------------------------------------

enum STATE // MOVESETS FOR BOSS
{
	STATE_PATROL = 0,//IDLE STATE OR NO STATE
	STATE_BASIC, //STATE FOR BASIC ATTACK
	STATE_AOE,  //STATE FOR AOE ATTACK
	STATE_SPAWN_ENEMIES, // STATE TO SPAWN ENEMIES
	STATE_SPAWN_BULLETS, // STATE TO SPAWN BULLETS
	STATE_MAZE_DARKEN,	// STATE TO DARKEN THE WHOLE LEVEL 

};

enum INNER_STATE // INNER STATE FOR BOSS
{
	INNER_STATE_ON_ENTER,
	INNER_STATE_ON_UPDATE,
	INNER_STATE_ON_EXIT
};


/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Boss Level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_BossLevel_Load(void);

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Boss Level state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_BossLevel_Init(void);


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Boss Level state.
*/
/******************************************************************************/
void GS_BossLevel_Update(void);

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_BossLevel_Draw(void);

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the Boss Level level.
*/
/******************************************************************************/
void GS_BossLevel_Free(void);

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Boss Level level.
*/
/******************************************************************************/
void GS_BossLevel_Unload(void);

// ---------------------------------------------------------------------------



