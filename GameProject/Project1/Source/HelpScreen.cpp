/******************************************************************************/
/*!
\file		HelpScreen.cpp
\author 	Alvin Woo Jia Hao
\par    	email: a.woo\@digipen.edu
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

static int debugstate = 0;

enum TYPE_BUTTON
{
	TYPE_BACK1 = 0,
	TYPE_BACK2,
	TYPE_BACK3,
	TYPE_BACK4,
	TYPE_BACK5,
	TYPE_BACK6,





};
struct HelpObj
{
	unsigned long type;
	AEGfxVertexList* pMesh;
	AEGfxTexture* pTexture;
	bool refMesh;
	bool refTexture;
};

struct HelpObjInst
{
	HelpObj* pObject;
	unsigned long flag = 0;
	float scale;
	AEVec2 posCurr;
	float	dirCurr;
	AEMtx33				transform;
};

/*!
	Defines
*/
/******************************************************************************/

static const unsigned int	Help_OBJ_NUM_MAX = 8;						// Max number of unique objects
static const unsigned int	Help_OBJ_INST_NUM_MAX = 32;					// Max number of object instances
static HelpObj				sHelpObjList[Help_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sHelpObjNum;								// Number of existing game obj
static HelpObjInst			sHelpObjInstList[Help_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sHelpObjInstNum;							// Number of existing game obj inst
static const float BackSize = 10;										// Scale of BG

static HelpObjInst* mBack;												// Pointer to Background object
static AEGfxTexture* CycleBG[3];										// Pointer to 3 pages


static float animated = 1;												// Timer for Animation

// Functions for HelpScreen.cpp only
/*!***********************************************************************
\brief
	This function creates a static object instance
\param[in] type
	The type of object
\param[in] scale
	The scale of the object
\param[in] pPos
	Pointer to the initial position of the object
\param[in] dir
	The initial direction of the object
\return
	Pointer to the instance
*************************************************************************/
HelpObjInst* HelpObjInstCreate(unsigned long type, float scale, AEVec2* pPos, float dir);

/*!***********************************************************************
\brief
	This function frees a static object instance
\param[in] pInst
	Pointer to the object instance
*************************************************************************/
void HelpObjInstDestroy(HelpObjInst* pInst);


/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Help Screen level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_HelpScreen_Load(void) {

	sHelpObjNum = 0;

	CycleBG[0] = AEGfxTextureLoad("Assets/MainMenu/Instruction_1.png");
	CycleBG[1] = AEGfxTextureLoad("Assets/MainMenu/Instruction_2.png");
	CycleBG[2] = AEGfxTextureLoad("Assets/MainMenu/Instruction_3.png");
	


	HelpObj* Background_1;
	Background_1 = sHelpObjList + sHelpObjNum++;
	AEGfxMeshStart();
	AEGfxTriAdd(-80.f, 45.f, 0x00FF00, 0.f, 0.f,
		-80.f, -45.f, 0x00FF00, 0.0f, 1.0f,
		80.f, 45.f, 0x00FF00, 1.f, 0.0f);

	AEGfxTriAdd(80.f, -45.f, 0x00FF00, 1.0f, 1.0f,
		-80.f, -45.f, 0x00FF00, 0.0f, 1.f,
		80.f, 45.f, 0x00FF00, 1.f, 0.0f);
	Background_1->pMesh = AEGfxMeshEnd();

	Background_1->type = TYPE_BACK1;
	Background_1->pTexture = CycleBG[0];

	Background_1->refTexture = false;
	Background_1->refMesh = false;
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Help Screen state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_HelpScreen_Init(void) {
	AEGfxSetBackgroundColor(0, 0, 0);

	AEVec2 Backpos;
	AEVec2Set(&Backpos, 0, 0);

	mBack = HelpObjInstCreate(TYPE_BACK1, BackSize, &Backpos, 0.0f);

}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Help Screen state.
*/
/******************************************************************************/
void GS_HelpScreen_Update(void) {

	if (AEInputCheckTriggered(AEVK_RIGHT))
	{
		cycle++;

	}
	if (cycle >= 3)
	{
		cycle = 0;
		gGameStateNext=gGameStatePrev;
	}
	mBack->pObject->pTexture = CycleBG[cycle];


	if (AEInputCheckTriggered(AEVK_3)) {
		gGameStateNext = GS_MAZE;
	}

	if (AEInputCheckTriggered(AEVK_4)) {
		gGameStateNext = GS_COLOSSEUM;
	}

	s32 mX, mY;
	AEInputGetCursorPosition(&mX, &mY);
	mouseX = float(mX);
	mouseY = float(mY);

	//pPlay = nullptr;

	if (AEInputCheckTriggered(AEVK_F3)) {
		debugstate ^= 1;

	}

	if (AEInputCheckReleased(AEVK_LBUTTON)) {


		if (utilities::rectbuttonClicked_AlignCtr(800.f, 445.f, 245.f, 85.f) == 1)//width 245 height 85
		{

			gGameStateNext = GS_MAINMENU;
			return;
		}

		if (utilities::rectbuttonClicked_AlignCtr(800.f, 585.f, 245.f, 85.f) == 1)//width 245 height 85
		{
			gGameStateNext = GS_QUIT;
			return;
		}
		//gGameStateNext = GS_WORLD;
	}




	for (unsigned long i = 0; i < Help_OBJ_INST_NUM_MAX; i++)
	{
		HelpObjInst* pInst = sHelpObjInstList + i;
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
void GS_HelpScreen_Draw(void) {




	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxTextureSet(NULL, 0, 0);

	AEGfxSetTransparency(1.0f);


	for (unsigned long i = 0; i < Help_OBJ_INST_NUM_MAX; i++)
	{
		HelpObjInst* pInst = sHelpObjInstList + i;


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
	This function frees all the instances created for the Help Screen level.
*/
/******************************************************************************/
void GS_HelpScreen_Free(void) {

	for (unsigned long i = 0; i < Help_OBJ_INST_NUM_MAX; i++)
	{
		HelpObjInst* pInst = sHelpObjInstList + i;
		if (pInst)
			HelpObjInstDestroy(pInst);
	}

}

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Help Screen level.
*/
/******************************************************************************/
void GS_HelpScreen_Unload(void) {


	for (unsigned int i = 0; i < sHelpObjNum; i++) {
		if ((sHelpObjList + i)->refMesh == false)
			AEGfxMeshFree((sHelpObjList + i)->pMesh);
	}

	for (int i = 0; i < 3; i++) {
		AEGfxTextureUnload(CycleBG[i]);
	}
}

// ---------------------------------------------------------------------------
/*!***********************************************************************
\brief
	This function creates a static object instance
\param[in] type
	The type of object
\param[in] scale
	The scale of the object
\param[in] pPos
	Pointer to the initial position of the object
\param[in] dir
	The initial direction of the object
\return
	Pointer to the instance
*************************************************************************/
HelpObjInst* HelpObjInstCreate(unsigned long type, float scale, AEVec2* pPos, float dir)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	//AE_ASSERT_PARM(type < sMenuObjNum);

	// loop through the object instance list to find a non-used object instance
	for (unsigned long i = 0; i < Help_OBJ_INST_NUM_MAX; i++)
	{
		HelpObjInst* pInst = sHelpObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = sHelpObjList + type;
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

/*!***********************************************************************
\brief
	This function frees a static object instance
\param[in] pInst
	Pointer to the object instance
*************************************************************************/

void HelpObjInstDestroy(HelpObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;


	sHelpObjInstNum--; //Decrement the number of game object instance
	// zero out the flag
	pInst->flag = 0;
}