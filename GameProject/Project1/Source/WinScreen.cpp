#include "Main.h"
#include <iostream>
#include "Globals.h"
/******************************************************************************/
/*!
\file		WinScreen.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 02, 2023
\brief		This source file contains the functions for winscreen state

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

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
struct WinObj
{
	unsigned long type;
	AEGfxVertexList* pMesh;
	AEGfxTexture* pTexture;
	bool refMesh;
	bool refTexture;
};

struct WinObjInst
{
	WinObj* pObject;
	unsigned long flag = 0;
	float scale;
	AEVec2 posCurr;
	float	dirCurr;
	AEMtx33				transform;
};

static const unsigned int	Win_OBJ_NUM_MAX = 8;
static const unsigned int	Win_OBJ_INST_NUM_MAX = 32;


static WinObj				sWinObjList[Win_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sWinObjNum;
static WinObjInst			sWinObjInstList[Win_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sWinObjInstNum;

static WinObjInst* mBack;
static AEGfxTexture* CycleBG[3];

//MenuObjInst* Animation[6] = { mBack1,mBack2,mBack3,mBack4,mBack5,mBack6 };
static float animated = 1;

//MenuObjInst* Background[6] = { mBack1,mBack2,mBack3,mBack4,mBack5,mBack6 };


static const float BackSize = 10;
static AEGfxTexture* WinanimationBG[6];
static int cycle = 0;
WinObjInst* WinObjInstCreate(unsigned long type, float scale, AEVec2* pPos, float dir);
void WinObjInstDestroy(WinObjInst* pInst);
AEAudio WinBGM;
AEAudioGroup WinGroup;

/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Main Menu level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_WinScreen_Load(void) {

	sWinObjNum = 0;

	WinanimationBG[0] = AEGfxTextureLoad("Assets/WinScreen/Win_Screen1.png");
	WinanimationBG[1] = AEGfxTextureLoad("Assets/WinScreen/Win_Screen2.png");
	WinanimationBG[2] = AEGfxTextureLoad("Assets/WinScreen/Win_Screen3.png");
	WinanimationBG[3] = AEGfxTextureLoad("Assets/WinScreen/Win_Screen4.png");
	WinanimationBG[4] = AEGfxTextureLoad("Assets/WinScreen/Win_Screen5.png");
	WinanimationBG[5] = AEGfxTextureLoad("Assets/WinScreen/Win_Screen6.png");



	WinObj* Background_1;
	Background_1 = sWinObjList + sWinObjNum++;
	AEGfxMeshStart();
	AEGfxTriAdd(-800.f, 450.f, 0x00FF00, 0.f, 0.f,
		-800.f, -450.f, 0x00FF00, 0.0f, 1.0f,
		800.f, 450.f, 0x00FF00, 1.f, 0.0f);

	AEGfxTriAdd(800.f, -450.f, 0x00FF00, 1.0f, 1.0f,
		-800.f, -450.f, 0x00FF00, 0.0f, 1.f,
		800.f, 450.f, 0x00FF00, 1.f, 0.0f);
	Background_1->pMesh = AEGfxMeshEnd();

	Background_1->type = TYPE_BACK1;
	Background_1->pTexture = CycleBG[0];

	Background_1->refTexture = false;
	Background_1->refMesh = false;

	WinBGM = AEAudioLoadMusic("Assets/Music/SUPERSTRUCTURE - Corporate MSCCRP2_01.wav");
	WinGroup = AEAudioCreateGroup();
}

/******************************************************************************/
/*!
	"Initialize" function of this state
	This function initialises all the values of the Main Menu state. It should
	be called once at the start of the level.
*/
/******************************************************************************/
void GS_WinScreen_Init(void) {
	AEGfxSetBackgroundColor(0, 0, 0);

	AEVec2 Backpos;
	AEVec2Set(&Backpos, 0, 0);

	mBack = WinObjInstCreate(TYPE_BACK1, 1, &Backpos, 0.0f);
	AEAudioPlay(WinBGM, WinGroup, 0.3f, 1, 1);
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Main Menu state.
*/
/******************************************************************************/
void GS_WinScreen_Update(void) {


	animated += g_dt;

	mBack->pObject->pTexture = WinanimationBG[(int)(animated * 10) % 6];

	if (AEInputCheckTriggered(AEVK_3)) {
		AEAudioStopGroup(WinGroup);
		gGameStateNext = GS_MAZE;
	}

	if (AEInputCheckTriggered(AEVK_4)) {
		AEAudioStopGroup(WinGroup);
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
	if (AEInputCheckTriggered(AEVK_ESCAPE)) {
		AEAudioStopGroup(WinGroup);
		gGameStateNext = GS_MAINMENU;
	}

	




	for (unsigned long i = 0; i < Win_OBJ_INST_NUM_MAX; i++)
	{
		WinObjInst* pInst = sWinObjInstList + i;
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
void GS_WinScreen_Draw(void) {




	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxTextureSet(NULL, 0, 0);

	AEGfxSetTransparency(1.0f);


	for (unsigned long i = 0; i < Win_OBJ_INST_NUM_MAX; i++)
	{
		WinObjInst* pInst = sWinObjInstList + i;


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
	This function frees all the instances created for the Main Menu level.
*/
/******************************************************************************/
void GS_WinScreen_Free(void) {

	for (unsigned long i = 0; i < Win_OBJ_INST_NUM_MAX; i++)
	{
		WinObjInst* pInst = sWinObjInstList + i;
		if (pInst)
			WinObjInstDestroy(pInst);
	}

}

/******************************************************************************/
/*!
	"Unload" function of this state
	This function frees all the shapes and assets that were loaded for the
	Main Menu level.
*/
/******************************************************************************/
void GS_WinScreen_Unload(void) {


	for (unsigned int i = 0; i < sWinObjNum; i++) {
		if ((sWinObjList + i)->refMesh == false)
			AEGfxMeshFree((sWinObjList + i)->pMesh);
	}

	for (int i = 0; i < 6; i++) {
		AEGfxTextureUnload(WinanimationBG[i]);
	}

	//AEGfxMeshFree(CreditMesh);
	
}

// ---------------------------------------------------------------------------
/******************************************************************************/
/*!

*/
/******************************************************************************/
WinObjInst* WinObjInstCreate(unsigned long type, float scale, AEVec2* pPos, float dir)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	//AE_ASSERT_PARM(type < sMenuObjNum);

	// loop through the object instance list to find a non-used object instance
	for (unsigned long i = 0; i < Win_OBJ_INST_NUM_MAX; i++)
	{
		WinObjInst* pInst = sWinObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = sWinObjList + type;
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

void WinObjInstDestroy(WinObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;


	sWinObjInstNum--; //Decrement the number of game object instance
	// zero out the flag
	pInst->flag = 0;
}