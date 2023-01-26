// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "Utilities.h"
#include <iostream>
struct entity {
	double x;
	double y;
};



// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	int gGameRunning = 1;

	// Initialization of your own variables go here

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1200, 700, 1, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("Game Name");

	// reset the system modules
	AESysReset();

	AEGfxVertexList* pMesh = 0;


	AEGfxMeshStart();

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f, 
				 0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f, 
				 -0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f, 
				0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f, 
				-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

	pMesh = AEGfxMeshEnd();
	

	AEGfxTexture* pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	AEGfxTexture* sTex = AEGfxTextureLoad("Assets/slash.png");


	entity planet;
	planet.x = 0;
		planet.y = 0;
		s32 x = 0, y = 0;
	
		int direction = 0;
		float cursorAngle;


	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		// Your own update logic goes here
		
		AEInputGetCursorPosition(&x, &y);
		cursorAngle = utilities::getAngle(planet.x+600, planet.y+350, x, y);
		
		if (y < planet.y + 350) {
			cursorAngle = -cursorAngle;
		}

		if (cursorAngle <= -(PI * 3 /4) || cursorAngle >(PI * 3 / 4)) {
			direction = 1;
		}
		if (cursorAngle > -(PI * 3 / 4) && cursorAngle <= -(PI * 1 / 4)) {
			direction = 2;
		}
		if (cursorAngle <= (PI * 1 / 4) && cursorAngle > -(PI * 1 / 4)) {
			direction = 3;
		}
		if (cursorAngle > (PI * 1 / 4) && cursorAngle <= (PI * 3 / 4)) {
			direction = 4;
		}


		if (AEInputCheckCurr(AEVK_W)) {
			planet.y -= 10;
		}
		if (AEInputCheckCurr(AEVK_S)) {
			planet.y += 10;
		}

		if (AEInputCheckCurr(AEVK_A)) {
			planet.x -= 10;
		}
		if (AEInputCheckCurr(AEVK_D)) {
			planet.x += 10;
		}

		// Your own rendering logic goes here

		// Set the background to black. 
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
		// Tell the engine to get ready to draw something with texture. 
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
		AEGfxSetBlendMode(AE_GFX_BM_BLEND); 
		AEGfxSetTransparency(1.0f);
		// Set the texture to pTex 
		AEGfxTextureSet(pTex, 0, 0);
		// Create a scale matrix that scales by 100 x and y 
		AEMtx33 scale = { 0 }; 
		AEMtx33Scale(&scale, 100.f, 100.f);
		// Create a rotation matrix that rotates by 45 degrees 
		AEMtx33 rotate = { 0 }; 
		AEMtx33Rot(&rotate, 0);
		// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis 
		AEMtx33 translate = { 0 }; 
		AEMtx33Trans(&translate, planet.x, -planet.y);
		// Concat the matrices (TRS) 
		AEMtx33 transform = { 0 }; 
		AEMtx33Concat(&transform, &rotate, &scale); 
		AEMtx33Concat(&transform, &translate, &transform);
		// Choose the transform to use 
		AEGfxSetTransform(transform.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);


		if (AEInputCheckCurr(AEVK_LBUTTON)) {
			float slashX;
			float slashY;
			float slashRot;
			switch (direction) {
			case 1: //right
				slashX = 50;
				slashY = 0;
				slashRot = 0;
				break;
			case 2: //up
				slashY = 50;
				slashX = 0;
				slashRot = PI / 2;
				break;
			case 3: //left
				slashX = -50;
				slashY = 0;
				slashRot = PI;
				break;
			case 4: //down
				slashY = -50;
				slashX = 0;
				slashRot = -PI / 2;
				break;
			default:
				break;
			}

			// Set the texture to pTex 
			AEGfxTextureSet(sTex, 0, 0);
			// Create a scale matrix that scales by 100 x and y 
			AEMtx33 scale = { 0 };
			AEMtx33Scale(&scale, 100.f, 100.f);
			// Create a rotation matrix that rotates by 45 degrees 
			AEMtx33 rotate = { 0 };
			AEMtx33Rot(&rotate, slashRot);
			// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis 
			AEMtx33 translate = { 0 };
			AEMtx33Trans(&translate, planet.x + slashX, -planet.y + slashY);
			// Concat the matrices (TRS) 
			AEMtx33 transform = { 0 };
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			// Choose the transform to use 
			AEGfxSetTransform(transform.m);
			// Actually drawing the mesh
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}


		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}
	
	AEGfxMeshFree(pMesh);

	AEGfxTextureUnload(pTex);
	AEGfxTextureUnload(sTex);

	// free the system
	AESysExit();
}