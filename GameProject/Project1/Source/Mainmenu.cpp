/******************************************************************************/
/*!
\file		Mainmenu.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 02, 2023
\brief		This header file contains the functions for the level of Main Menu.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Main.h"
#include <iostream>

// ---------------------------------------------------------------------------
AEGfxVertexList* BoxMesh;
/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Main Menu level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etcc
*/
/******************************************************************************/
void GS_MainMenu_Load(void) {
	AEGfxMeshStart();

	AEGfxTriAdd(2.f, 1.f, 0xFFFF00FF, 16.0f / 192, 16.0f / 176,
		-2.f, -1.f, 0xFFFFFF00, 0.0f, 16.0f / 176,
		-2.f, 1.f, 0xFF00FFFF, 16.0f / 192, 0.0f);

	AEGfxTriAdd(2.f, 1.f, 0xFFFFFFFF, 0.0f, 16.0f / 176,
		2.f, -1.f, 0xFFFFFFFF, 0.0f, 0.0f,
		-2.f, -1.f, 0xFFFFFFFF, 16.0f / 192, 0.0f);

	BoxMesh = AEGfxMeshEnd();
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Main Menu state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_MainMenu_Init(void) {
	;
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Main Menu state.
*/
/******************************************************************************/
void GS_MainMenu_Update(void) {
	AEGfxSetBackgroundColor(1, 0, 0);
	if (AEInputCheckTriggered(AEVK_1)) {
		gGameStateNext = GS_WORLD;
	}
}

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_MainMenu_Draw(void) {
	AEMtx33		 trans = { 0 }, rot = { 0 }, scale = { 0 };
	// Tell the engine to get ready to draw something with texture. 
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTransform(BoxMesh->trans.m);
	AEGfxMeshDraw(BoxMesh, AE_GFX_MDM_TRIANGLES);
}

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the Main Menu level.
*/
/******************************************************************************/
void GS_MainMenu_Free(void) {
	;
}

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Main Menu level.
*/
/******************************************************************************/
void GS_MainMenu_Unload(void) {
	;
}

// ---------------------------------------------------------------------------



