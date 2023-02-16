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

AEGfxTexture* bTex;

int state = 0;
float mX, mY;

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


const unsigned int	MENU_OBJ_NUM_MAX = 32;
const unsigned int	MENU_OBJ_INST_NUM_MAX = 2048;
const unsigned long FLAG_ACTIVE = 0x00000001;


static MenuObj				sMenuObjList[MENU_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sMenuObjNum;
static MenuObjInst			sMenuObjInstList[MENU_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sMenuObjInstNum;

static MenuObjInst* pPlay;

const float ButtonSize = 30;

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
	MenuObj* pObj;

	pObj = sMenuObjList + sMenuObjNum++;
	pObj->type = TYPE_PLAY;
	AEGfxMeshStart();

	AEGfxTriAdd(-2.f, 1.5f, 0x00FF00, 0.f, 0.f,
		-2.f, 0.f, 0x00FF00, 0.0f, 1.0f ,
		2.f, 1.5f, 0x00FF00, 1.f, 1.0f);

	AEGfxTriAdd(2.f, 0.f, 0x00FF00, 1.0f, 1.0f,
		-2.f, 0.f, 0x00FF00, 0.0f, 1.f,
		2.f, 1.5f, 0x00FF00, 1.f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();

    font = AEGfxCreateFont("Assets/OpenSans-Regular.ttf", 12);

	pPlay = nullptr;
	
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
	AEVec2 Playpos;
	AEVec2Set(&Playpos, 0, 0);
	pPlay = menuObjInstCreate(TYPE_PLAY, ButtonSize, &Playpos,0.0f);
	pPlay = sMenuObjInstList + sMenuObjInstNum++;
	//AE_ASSERT(pPlay);
	//bTex = AEGfxTextureLoad("Assets/bluee.jpg");
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Main Menu state.
*/
/******************************************************************************/
void GS_MainMenu_Update(void) {
	
	s32				mouseX;
	s32				mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	mX = float (mouseX);
	mY = float (mouseY);


	if (AEInputCheckTriggered(AEVK_F3)) {
		state ^= 1;

	}

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		if (utilities::rectbuttonClicked_AlignCtr(800.f, 420.f, 200.f, 50.f) == 1)
		{
			gGameStateNext = GS_WORLD;
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
	
	


	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);

	AEGfxSetTransparency(1.0f);
	for (unsigned long i = 0; i < MENU_OBJ_INST_NUM_MAX; i++)
	{
		MenuObjInst* pInst = sMenuObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;


		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		AEGfxSetTransform(pInst->transform.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		
	}

	if (state == 1)
	{
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		char debug[20] = "Debug Screen";

		char mouse_xy_buffer[50] = " "; // buffer
		AEGfxPrint(font, debug, -0.99f, 0.90f, 1.5f, 1.0f, 1.0f, 1.0f);
		sprintf_s(mouse_xy_buffer, "Mouse Position X: %.2f", mX);
		AEGfxPrint(font, mouse_xy_buffer, -0.99f, 0.76f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprintf_s(mouse_xy_buffer, "Mouse Position Y: %.2f", mY);
		AEGfxPrint(font, mouse_xy_buffer, -0.99f, 0.71f, 1.0f, 1.0f, 1.0f, 1.0f);


	}
	
	//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//// Set the tint to white, so that the sprite can // display the full range of colors (default is black).
	//AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	//// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxSetTransparency(1.0f);
	//// Set the texture to pTex 
	//AEGfxTextureSet(bTex, NULL, NULL);
	//AEMtx33Scale(&scale, 100.f, (100.f));
	//

	//AEMtx33Rot(&rotate, 0);
	//
	//// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis
	//
	//AEMtx33Trans(&translate, 0.f, 0.f);
	//// Concat the matrices (TRS) 
	//AEMtx33Concat(&transform, &rotate, &scale);
	//AEMtx33Concat(&transform, &translate, &transform);
	//// Choose the transform to use 
	//AEGfxSetTransform(transform.m);
	//// Actually drawing the mesh
	//AEGfxMeshDraw(BoxMesh, AE_GFX_MDM_TRIANGLES);

	//Exit/////////////////////////////////////////

	
	// Choose the transform to use 
	//AEGfxSetTransform(transform.m);
	
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
	
	for (unsigned long i = 0; i < MENU_OBJ_NUM_MAX; i++)
	{
		if ((sMenuObjList + i)->pMesh == nullptr)
			continue;
		AEGfxMeshFree((sMenuObjList + i)->pMesh);
	}
	AEGfxDestroyFont(font);
	//AEGfxTextureUnload(bTex);
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


