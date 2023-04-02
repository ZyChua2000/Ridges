/******************************************************************************/
/*!
\file		SplashScreen.cpp
\author 	Liu Chengrong
\par    	email: chengrong.liu\@digipen.edu
\date   	March 31, 2023
\brief		This source file contains the functions for the splash screen to run

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Main.h"



// ---------------------------------------------------------------------------
static float timer = 0;
static float tint = 0;

static float fadeDuration = 2.0f;;
static float endTime = 5.f;
static float fadeoutStart = endTime - fadeDuration;
static AEGfxVertexList* pMesh = 0;
static AEGfxTexture* splashscreen;
/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Splash Screen level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_SplashScreen_Load(void)
{
	AEGfxMeshStart();

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFF00FF, 1.0f, 1.0f,
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
		0.5f, 0.5f, 0xFF00FFFF, 1.0f, 0.0f);

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f);
	pMesh = AEGfxMeshEnd();

	splashscreen = AEGfxTextureLoad("Assets/MainMenu/digipen.png");
	
	
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Splash Screen state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_SplashScreen_Init(void)
{
	
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Splash Screen state.
*/
/******************************************************************************/
void GS_SplashScreen_Update(void)
{
	timer += g_dt;

	if (timer < fadeDuration) {
		tint = timer / fadeDuration;
	}
	else {
		tint = 1;
	}
	if (timer > fadeoutStart) // Fade time
	{
		tint = endTime/2 - timer/ fadeDuration;
	}

	if(timer > endTime) // End time
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
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(tint);
	AEGfxTextureSet(splashscreen, 0, 0);
	AEMtx33 scale, transform;
	AEMtx33Scale(&scale, 1600, 900);
	AEMtx33Rot(&transform, 0);
	AEMtx33Concat(&transform, &transform, &scale);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);


}

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the Splash Screen level.
*/
/******************************************************************************/
void GS_SplashScreen_Free(void)
{

}

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Splash Screen level.
*/
/******************************************************************************/
void GS_SplashScreen_Unload(void)
{

	AEGfxMeshFree(pMesh);
	AEGfxTextureUnload(splashscreen);
	
}