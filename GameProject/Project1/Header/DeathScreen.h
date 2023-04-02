/******************************************************************************/
/*!
\file		DeathScreen.h
\author 	Alvin Woo Jia Hao
\par    	email: a.woo\@digipen.edu
\date   	February 02, 2023
\brief		This header file contains the functions for the level of Main Menu.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/



// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Death Screen level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc?c
*/
/******************************************************************************/
void GS_DeathScreen_Load(void);

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Death Screen state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_DeathScreen_Init(void);


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Death Screen state.
*/
/******************************************************************************/
void GS_DeathScreen_Update(void);

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_DeathScreen_Draw(void);

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the Death Screen level.
*/
/******************************************************************************/
void GS_DeathScreen_Free(void);

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Death Screen level.
*/
/******************************************************************************/
void GS_DeathScreen_Unload(void);

// ---------------------------------------------------------------------------

