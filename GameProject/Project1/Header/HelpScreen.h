/******************************************************************************/
/*!
\file		HelpScreen.h
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
	This function loads all necessary assets for the Help screen level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_HelpScreen_Load(void);

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Help screen state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_HelpScreen_Init(void);


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Help screen state.
*/
/******************************************************************************/
void GS_HelpScreen_Update(void);

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_HelpScreen_Draw(void);

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the Help screen level.
*/
/******************************************************************************/
void GS_HelpScreen_Free(void);

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Help screen level.
*/
/******************************************************************************/
void GS_HelpScreen_Unload(void);

// ---------------------------------------------------------------------------
