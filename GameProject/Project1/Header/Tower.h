#pragma once
/******************************************************************************/
/*!
\file		Tower.h
\author 	Liu Chengrong
\par    	email: chengrong.liu\@digipen.edu
\date   	March 31, 2023
\brief		This header file contains the functions for the level of Tower.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/



// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Tower level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_Tower_Load(void);

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Tower state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_Tower_Init(void);


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Tower state.
*/
/******************************************************************************/
void GS_Tower_Update(void);

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_Tower_Draw(void);

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the Tower level.
*/
/******************************************************************************/
void GS_Tower_Free(void);

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Tower level.
*/
/******************************************************************************/
void GS_Tower_Unload(void);

// ---------------------------------------------------------------------------



