// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "Utilities.h"
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
	AEGfxVertexList* pblack = 0;
	AEGfxMeshStart();

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
	AEGfxTexture* pblacktex = AEGfxTextureLoad("Assets/blackhole.png");
	pblack = AEGfxMeshEnd();

	entity planet;
	planet.x = 0;
		planet.y = 0;
	entity blackhole;
	blackhole.x = -200;
		blackhole.y = 100;

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		// Your own update logic goes here
		
		

		float Angle = utilities::getAngle(blackhole.x, blackhole.y, planet.x, planet.y);
		blackhole.x -= cos(Angle)/5;
		blackhole.y -= sin(Angle)/5;


		if (AEInputCheckTriggered(AEVK_D) == 1) {
			blackhole.x += 5*cos(Angle);
			blackhole.y += 5*sin(Angle);
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
		AEMtx33Trans(&translate, planet.x, planet.y);
		// Concat the matrices (TRS) 
		AEMtx33 transform = { 0 }; 
		AEMtx33Concat(&transform, &rotate, &scale); 
		AEMtx33Concat(&transform, &translate, &transform);
		// Choose the transform to use 
		AEGfxSetTransform(transform.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// Tell the engine to get ready to draw something with texture. 
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set the tint to white, so that the sprite can // display the full range of colors (default is black). 
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Set blend mode to AE_GFX_BM_BLEND // This will allow transparency. 
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		// Set the texture to pTex 
		AEGfxTextureSet(pblacktex, 0, 0);
		// Create a scale matrix that scales by 100 x and y 
		AEMtx33 scale2 = { 0 };
		AEMtx33Scale(&scale2, 100.f, 100.f);
		// Create a rotation matrix that rotates by 45 degrees 
		AEMtx33 rotate2 = { 0 };
		AEMtx33Rot(&rotate2, 0);
		// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis 
		AEMtx33 translate2 = { 0 };
		AEMtx33Trans(&translate2, blackhole.x, blackhole.y);
		// Concat the matrices (TRS) 
		AEMtx33 transform2 = { 0 };
		AEMtx33Concat(&transform2, &rotate2, &scale2);
		AEMtx33Concat(&transform2, &translate2, &transform2);
		// Choose the transform to use 
		AEGfxSetTransform(transform2.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(pblack, AE_GFX_MDM_TRIANGLES);

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}
	
	AEGfxMeshFree(pMesh);
	AEGfxMeshFree(pblack);

	AEGfxTextureUnload(pTex);
	AEGfxTextureUnload(pblacktex);

	// free the system
	AESysExit();
}