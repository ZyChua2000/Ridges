/******************************************************************************/
/*!
\file		Mainmenu.h
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
#include "Globals.h"


// ---------------------------------------------------------------------------
static int debugstate = 0;
float timer = 0;
float tint = 0;
AEGfxVertexList* pMesh = 0;
AEGfxTexture* splashscreen;
/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Main Menu level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_SplashScreen_Load(void)
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x88880808, 0.0f, 0.0f,
		0.5f, -0.5f, 0x88880808, 0.0f, 0.0f,
		0.5f, 0.5f, 0x88880808, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, -0.5f, 0x88880808, 0.0f, 0.0f,
		-0.5f, 0.5f, 0x88880808, 0.0f, 0.0f,
		0.5f, 0.5f, 0x88880808, 0.0f, 0.0f);
	pMesh = AEGfxMeshEnd();

	
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Main Menu state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_SplashScreen_Init(void)
{
	AEGfxTexture* splashscreen = AEGfxTextureLoad("Assets/MainMenu/digipen.png");
	//AEGfxTextureSet(splashscreen, 1, 1);
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Main Menu state.
*/
/******************************************************************************/
void GS_SplashScreen_Update(void)
{
	timer += g_dt;
	tint += g_dt;
	if(timer > 2.f)
	{
		gGameStateNext = GS_MAINMENU;
		return;
	}

}

/******************************************************************************/
/*!
	"Draw" function of this state
	This function draws all the shapes onto the game screen, called repeatedly
	during game loop.
*/
/******************************************************************************/
void GS_SplashScreen_Draw(void)
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, tint);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(0.5f);
	AEGfxTextureSet(splashscreen, 0, 0);
	AEMtx33 scale, rot, trans, transform;
	AEMtx33Scale(&scale, 5, 5);
	AEMtx33Rot(&rot, 0);
	AEMtx33Trans(&trans, 0, 0);
	AEMtx33Concat(&transform, &rot, &scale);
	AEMtx33Concat(&transform, &trans, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	//Exit/////////////////////////////////////////

	
	if (debugstate == 1)
	{
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		char debug[20] = "Debug Screen";

		char mouse_xy_buffer[50] = " "; // buffer
		AEGfxPrint(1, debug, -0.99f, 0.90f, 1.5f, 1.0f, 1.0f, 1.0f);
		sprintf_s(mouse_xy_buffer, "Mouse Position X: %.2f", mouseX);
		AEGfxPrint(1, mouse_xy_buffer, -0.99f, 0.76f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(mouse_xy_buffer, "Mouse Position Y: %.2f", mouseY);
		AEGfxPrint(1, mouse_xy_buffer, -0.99f, 0.71f, 1.0f, 1.0f, 1.0f, 1.0f);
	}
	
}

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the Main Menu level.
*/
/******************************************************************************/
void GS_SplashScreen_Free(void)
{
	AEGfxMeshFree(pMesh);
	//AEGfxTextureUnload(splashscreen);
}

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Main Menu level.
*/
/******************************************************************************/
void GS_SplashScreen_Unload(void)
{

	
	
}