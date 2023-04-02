/******************************************************************************/
/*!
\file		Mainmenu.cpp
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
	TYPE_BACK1=0,
	TYPE_BACK2,
	TYPE_BACK3,
	TYPE_BACK4,
	TYPE_BACK5,
	TYPE_BACK6,
	



	
};
struct MenuObj
{
	unsigned long type{};
	AEGfxVertexList* pMesh{};
	AEGfxTexture* pTexture{};
	bool refMesh{};
	bool refTexture{};
};

struct MenuObjInst
{
	MenuObj* pObject{};
	unsigned long flag{};
	float scale{};
	AEVec2 posCurr{};
	float	dirCurr{};
	AEMtx33	transform{};
};

static const unsigned int	MENU_OBJ_NUM_MAX = 8;
static const unsigned int	MENU_OBJ_INST_NUM_MAX = 32;


static MenuObj				sMenuObjList[MENU_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sMenuObjNum;
static MenuObjInst			sMenuObjInstList[MENU_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sMenuObjInstNum;

static MenuObjInst* mBack;
static AEGfxTexture* animationBG[6];

//MenuObjInst* Animation[6] = { mBack1,mBack2,mBack3,mBack4,mBack5,mBack6 };
static float animated = 1;

//MenuObjInst* Background[6] = { mBack1,mBack2,mBack3,mBack4,mBack5,mBack6 };


static const float BackSize = 10;
static bool credit = false;
AEGfxVertexList* CreditMesh;
AEGfxTexture* Credit_Img;

MenuObjInst* menuObjInstCreate(unsigned long type, float scale, AEVec2* pPos, float dir);
void menuObjInstDestroy(MenuObjInst* pInst);
AEAudio BackgroundMusic;
AEAudioGroup Group1;

/******************************************************************************/
/*!
	"Load" function of this state
	This function loads all necessary assets for the Main Menu level.
	It should be called once before the start of the level.
	It loads assets like textures, meshes and music files etc
*/
/******************************************************************************/
void GS_MainMenu_Load(void) {

	sMenuObjNum = 0;

	animationBG[0] = AEGfxTextureLoad("Assets/MainMenu/Mainback1.png");
	animationBG[1] = AEGfxTextureLoad("Assets/MainMenu/Mainback2.png");
	animationBG[2] = AEGfxTextureLoad("Assets/MainMenu/Mainback3.png");
	animationBG[3] = AEGfxTextureLoad("Assets/MainMenu/Mainback4.png");
	animationBG[4] = AEGfxTextureLoad("Assets/MainMenu/Mainback5.png");
	animationBG[5] = AEGfxTextureLoad("Assets/MainMenu/Mainback6.png");

	MenuObj* Background_1;
	Background_1= sMenuObjList + sMenuObjNum++;
	AEGfxMeshStart();
	AEGfxTriAdd(-80.f, 45.f, 0x00FF00, 0.f, 0.f,
		-80.f, -45.f, 0x00FF00, 0.0f, 1.0f,
		80.f, 45.f, 0x00FF00, 1.f, 0.0f);

	AEGfxTriAdd(80.f, -45.f, 0x00FF00, 1.0f, 1.0f,
		-80.f, -45.f, 0x00FF00, 0.0f, 1.f,
		80.f, 45.f, 0x00FF00, 1.f, 0.0f);
	Background_1->pMesh = AEGfxMeshEnd();

	Background_1->type = TYPE_BACK1;
	Background_1->pTexture = animationBG[0];

	Background_1->refTexture = false;
	Background_1->refMesh = false;



	
	AEGfxMeshStart();
	AEGfxTriAdd(-800.f, 450.f, 0x00FF00, 0.f, 0.f,
		-800.f, -450.f, 0x00FF00, 0.0f, 1.0f,
		800.f, 450.f, 0x00FF00, 1.f, 0.0f);

	AEGfxTriAdd(800.f, -450.f, 0x00FF00, 1.0f, 1.0f,
		-800.f, -450.f, 0x00FF00, 0.0f, 1.f,
		800.f, 450.f, 0x00FF00, 1.f, 0.0f);
	CreditMesh = AEGfxMeshEnd();

	BackgroundMusic = AEAudioLoadMusic("Assets/Music/SCI-FI-FOREST_GEN-HDF-20668.wav");
	Group1 = AEAudioCreateGroup();
	Credit_Img = AEGfxTextureLoad("Assets/Credits.png");
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
	
	AEVec2 Backpos;
	AEVec2Set(&Backpos, 0, 0);

		mBack = menuObjInstCreate(TYPE_BACK1, BackSize, &Backpos, 0.0f);
		
		AEAudioPlay(BackgroundMusic, Group1, 0.1f, 1, 1);
}


/******************************************************************************/
/*!
	"Update" function of this state
	This function updates the game logic, physics and collision. It runs while
	the game loop runs for the Main Menu state.
*/
/******************************************************************************/
void GS_MainMenu_Update(void) {
	
	animated += g_dt;
	
	mBack->pObject->pTexture = animationBG[(int)(animated*10) %6];

	
		
	if (AEInputCheckTriggered(AEVK_1)) {
		AEAudioStopGroup(Group1);
		gGameStateNext = GS_WIN;
		return;
	}
	
	if (AEInputCheckTriggered(AEVK_3)) {
		AEAudioStopGroup(Group1);
		gGameStateNext = GS_MAZE;
		return;
	}

	if (AEInputCheckTriggered(AEVK_4)) {
		AEAudioStopGroup(Group1);
		gGameStateNext = GS_COLOSSEUM;
		return;
	}

	if (AEInputCheckTriggered(AEVK_5)) {
		AEAudioStopGroup(Group1);
		gGameStateNext = GS_TOWER;
		return;
	}


	if (AEInputCheckTriggered(AEVK_6)) {
		AEAudioStopGroup(Group1);
		gGameStateNext = GS_BOSSLEVEL;
		return;
	}
	if (AEInputCheckTriggered(AEVK_H)) {
		AEAudioPauseGroup(Group1);
		gGameStateNext = GS_HELP;
		return;
	}

	if (AEInputCheckTriggered(AEVK_ESCAPE)) {
		credit = false;
		
	}

	s32 mX, mY;
	AEInputGetCursorPosition(&mX, &mY);
	mouseX = float (mX);
	mouseY = float (mY);

	//pPlay = nullptr;
	
	if (AEInputCheckTriggered(AEVK_F3)) {
		debugstate ^= 1;

	}
	
	
		if (AEInputCheckReleased(AEVK_LBUTTON)) {
			
			
			if (utilities::rectbuttonClicked_AlignCtr(800.f, 445.f, 245.f, 85.f) == 1)//width 245 height 85
			{
				loadState = 0;
				AEAudioStopGroup(Group1);
				gGameStateNext = GS_WORLD;
				return;
				

			}

			if (utilities::rectbuttonClicked_AlignCtr(800.f, 585.f, 245.f, 85.f) == 1)//width 245 height 85
			{
				gGameStateNext = GS_QUIT;
					return;
			}

			if (utilities::rectbuttonClicked_AlignCtr(155.f, 820.f, 245.f, 85.f) == 1)//width 245 height 85
			{
				credit = true;
				
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
	if (credit == true)
	{
		AEGfxTextureSet(Credit_Img, 0, 0);
		// Create a scale matrix that scales by 100 x and y
		AEMtx33 lscale = { 0 };
		AEMtx33Scale(&lscale, 1, 1);
		// Create a rotation matrix that rotates by 45 degrees
		AEMtx33 lrotate = { 0, };

		AEMtx33Rot(&lrotate, 0);

		// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis
		AEMtx33 ltranslate = { 0 };
		AEMtx33Trans(&ltranslate, camX * SPRITE_SCALE, camY * SPRITE_SCALE);
		// Concat the matrices (TRS) 
		AEMtx33 ltransform = { 0 };
		AEMtx33Concat(&ltransform, &lrotate, &lscale);
		AEMtx33Concat(&ltransform, &ltranslate, &ltransform);
		// Choose the transform to use 
		AEGfxSetTransform(ltransform.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(CreditMesh, AE_GFX_MDM_TRIANGLES);
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
void GS_MainMenu_Free(void) {
	
	for (unsigned long i = 0; i < MENU_OBJ_INST_NUM_MAX; i++)
	{
		MenuObjInst* pInst = sMenuObjInstList + i;
		if(pInst)
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
	}

	for (int i = 0; i < 6; i++) {
		AEGfxTextureUnload(animationBG[i]);
	}

	AEGfxMeshFree(CreditMesh);
	AEGfxTextureUnload(Credit_Img);

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

	
	sMenuObjInstNum--; //Decrement the number of game object instance
	// zero out the flag
	pInst->flag = 0;
}


