/******************************************************************************/
/*!
\file		SplashScreen.h
\author 	Liu Chengrong
\par    	email: chengrong.liu\@digipen.edu
\date   	March 31, 2023
\brief		This header file contains the functions for the splash screen level.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/



// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Splash Screen level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_SplashScreen_Load(void);

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Splash Screen state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_SplashScreen_Init(void);


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Splash Screen state.
*/
/******************************************************************************/
void GS_SplashScreen_Update(void);

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_SplashScreen_Draw(void);

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the Splash Screen level.
*/
/******************************************************************************/
void GS_SplashScreen_Free(void);

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Splash Screen level.
*/
/******************************************************************************/
void GS_SplashScreen_Unload(void);

// ---------------------------------------------------------------------------





