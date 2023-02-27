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
#include "Globals.h"

// ---------------------------------------------------------------------------

static int debugstate = 0;

enum TYPE_BUTTON
{
	TYPE_PLAY = 0,
	TYPE_EXIT,
	TYPE_CREDIT,
	TYPE_INSTR,

	
};

struct MenuObj
{
	unsigned long type;
	AEGfxVertexList* pMesh;
	AEGfxTexture* pTexture;
	bool refMesh;
	bool refTexture;
};

struct MenuObjInst
{
	MenuObj* pObject;
	unsigned long flag;
	float scale;
	AEVec2 posCurr;
	float	dirCurr;
	AEMtx33				transform;
};

static const unsigned int	MENU_OBJ_NUM_MAX = 8;
static const unsigned int	MENU_OBJ_INST_NUM_MAX = 32;
static const unsigned long	FLAG_ACTIVE = 0x00000001;


static MenuObj				sMenuObjList[MENU_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sMenuObjNum;
static MenuObjInst			sMenuObjInstList[MENU_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sMenuObjInstNum;

static MenuObjInst* pPlay;
static MenuObjInst* pExit;

static const float ButtonSize = 5;

static s8 font;

MenuObjInst* menuObjInstCreate(unsigned long type, float scale, AEVec2* pPos, float dir);
void menuObjInstDestroy(MenuObjInst* pInst);


/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Main Menu level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etcc
*/
/******************************************************************************/
void GS_MainMenu_Load(void) {
	MenuObj* Play;

	Play = sMenuObjList + sMenuObjNum++;
	Play->type = TYPE_PLAY;
	AEGfxMeshStart();

	AEGfxTriAdd(-15.f, 30.f, 0x00FF00, 0.f, 0.f,
		-15.f, 0.f, 0x00FF00, 0.0f, 1.0f ,
		15.f, 30.f, 0x00FF00, 1.f, 0.0f);

	AEGfxTriAdd(15.f, 0.f, 0x00FF00, 1.0f, 1.0f,
		-15.f, 0.f, 0x00FF00, 0.0f, 1.f,
		15.f, 30.f, 0x00FF00, 1.f, 0.0f);

	Play->pMesh = AEGfxMeshEnd();
	Play->pTexture = AEGfxTextureLoad("Assets/StartButton.png");
	Play->refMesh = true;
	Play->refTexture = true;

	MenuObj* Exit;
	Exit = sMenuObjList + sMenuObjNum++;
	Exit->pMesh = Play->pMesh;
	Exit->type = TYPE_EXIT;
	Exit->pTexture = AEGfxTextureLoad("Assets/ExitButton.png");
	Exit->refMesh = true;
	Exit->refTexture = true;
    font = AEGfxCreateFont("Assets/OpenSans-Regular.ttf", 12);

	
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Main Menu state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_MainMenu_Init(void) {
	AEGfxSetBackgroundColor(0, 0, 0);
	

	AEVec2 Exitpos;
	AEVec2Set(&Exitpos, 0, 0);
	pExit = menuObjInstCreate(TYPE_EXIT, ButtonSize, &Exitpos, 0.0f);
	pExit = sMenuObjInstList + sMenuObjInstNum++;
	AEVec2 Playpos;
	AEVec2Set(&Playpos, 0, 70);
	pPlay = menuObjInstCreate(TYPE_PLAY, ButtonSize, &Playpos, 0.0f); //width 105 height 35
	pPlay = sMenuObjInstList + sMenuObjInstNum++;
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Main Menu state.
*/
/******************************************************************************/
void GS_MainMenu_Update(void) {
	
	if (AEInputCheckTriggered(AEVK_3)) {
		gGameStateNext = GS_MAZE;
	}

	if (AEInputCheckTriggered(AEVK_4)) {
		gGameStateNext = GS_COLOSSEUM;
	}

	s32 mX, mY;
	AEInputGetCursorPosition(&mX, &mY);
	mouseX = float (mX);
	mouseY = float (mY);

	pPlay = nullptr;
	
	if (AEInputCheckTriggered(AEVK_F3)) {
		debugstate ^= 1;

	}

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		if (utilities::rectbuttonClicked_AlignCtr(800.f, 300.f, 105.f, 35.f) == 1)
		{
			gGameStateNext = GS_WORLD;
		}

		if (utilities::rectbuttonClicked_AlignCtr(800.f, 370.f, 105.f, 35.f) == 1)
		{
			gGameStateNext = GS_QUIT;
		}
		//gGameStateNext = GS_WORLD;
	}

	for (unsigned long i = 0; i < MENU_OBJ_INST_NUM_MAX; i++)
	{
		MenuObjInst* pInst = sMenuObjInstList + i;
		AEMtx33		 trans = { 0 }, rot = { 0 }, scale = { 0 };


		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;
		// =====================================
	// calculate the matrix for all objects
	// =====================================
		// Compute the scaling matrix
		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
		// Compute the rotation matrix 
		AEMtx33Rot(&rot, pInst->dirCurr);
		// Compute the translation matrix
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
		AEMtx33Concat(&pInst->transform, &trans, &rot);
		AEMtx33Concat(&pInst->transform, &pInst->transform, &scale);
	

		
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
	
	


	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxTextureSet(NULL, 0, 0);

	AEGfxSetTransparency(1.0f);
	for (unsigned long i = 0; i < MENU_OBJ_INST_NUM_MAX; i++)
	{
		MenuObjInst* pInst = sMenuObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		
		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		AEGfxTextureSet(pInst->pObject->pTexture, 0, 0);
		AEGfxSetTransform(pInst->transform.m);
		
		// Actually drawing the mesh
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		
	}


	

	//Exit/////////////////////////////////////////

	
	if (debugstate == 1)
	{
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		char debug[20] = "Debug Screen";

		char mouse_xy_buffer[50] = " "; // buffer
		AEGfxPrint(font, debug, -0.99f, 0.90f, 1.5f, 1.0f, 1.0f, 1.0f);
		sprintf_s(mouse_xy_buffer, "Mouse Position X: %.2f", mouseX);
		AEGfxPrint(font, mouse_xy_buffer, -0.99f, 0.76f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(mouse_xy_buffer, "Mouse Position Y: %.2f", mouseY);
		AEGfxPrint(font, mouse_xy_buffer, -0.99f, 0.71f, 1.0f, 1.0f, 1.0f, 1.0f);


	}
	
}

/******************************************************************************/
/*!
	"Free" function of this state
	This function frees all the instances created for the Main Menu level.
*/
/******************************************************************************/
void GS_MainMenu_Free(void) {
	//AEGfxMeshFree(BoxMesh);
	for (unsigned long i = 0; i < MENU_OBJ_INST_NUM_MAX; i++)
	{
		MenuObjInst* pInst = sMenuObjInstList + i;
		menuObjInstDestroy(pInst);
	}
}

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Main Menu level.
*/
/******************************************************************************/
void GS_MainMenu_Unload(void) {
	
	for (unsigned int i = 0; i < sMenuObjNum; i++) {
		if ((sMenuObjList + i)->refMesh == false)
			AEGfxMeshFree((sMenuObjList + i)->pMesh);
		//if ((sMenuObjList + i)->refTexture == false)
		//	AEGfxTextureUnload((sMenuObjList + i)->pTexture);
	}

	for (int i = 0; i < 5; i++) {
		AEGfxTextureUnload(animationBG[i]);
	}
	AEGfxDestroyFont(font);
}

// ---------------------------------------------------------------------------
/******************************************************************************/
/*!

*/
/******************************************************************************/
MenuObjInst* menuObjInstCreate(unsigned long type, float scale, AEVec2* pPos,float dir)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	//AE_ASSERT_PARM(type < sMenuObjNum);

	// loop through the object instance list to find a non-used object instance
	for (unsigned long i = 0; i < MENU_OBJ_INST_NUM_MAX; i++)
	{
		MenuObjInst* pInst = sMenuObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = sMenuObjList + type;
			pInst->flag = FLAG_ACTIVE;
			pInst->scale = scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->dirCurr = dir;

			

			// return the newly created instance
			return pInst;
		}
	}

	// cannot find empty slot => return 0
	return 0;
}

/******************************************************************************/
/*!

*/
/******************************************************************************/

void menuObjInstDestroy(MenuObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}


