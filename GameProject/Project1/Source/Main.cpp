// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "Utilities.h"
#include "iostream"
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

	AEGfxVertexList* spriteMesh = 0;


	AEGfxMeshStart();

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFF00FF, 16.0f / 192, 16.0f / 176,
		0.5f, -0.5f, 0xFFFFFF00, 0.0f, 16.0f / 176,
		-0.5f, 0.5f, 0xFF00FFFF, 16.0f / 192, 0.0f);

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 16.0f / 176,
		0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 16.0f / 192, 0.0f);

	spriteMesh = AEGfxMeshEnd();


	AEGfxTexture* pTex = AEGfxTextureLoad("../Assets/Tilemap/tilemap_packed.png");
	AEGfxVertexList* pblack = 0;
	AEGfxMeshStart();

	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

	AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);
	AEGfxTexture* pblacktex = AEGfxTextureLoad("../Assets/blackhole.png");
	AEGfxTexture* sTex = AEGfxTextureLoad("../Assets/slash.png");
	pblack = AEGfxMeshEnd();

	entity planet;
	planet.x = 0;
	planet.y = 0;

	s32 x{ 0 }, y{ 0 }; //init xy pos
	
	entity blackhole;
	blackhole.x = -200;
	blackhole.y = 100;

	int direction = 0;
	float cursorAngle;

	//Rotate Init
	float i{ 0 };

	//Movement Init
	AEVec2 player_pos{ 0, 0 }; // player position
	AEVec2 player_direction{ 0,0 }; //player direction
	float speed{ 150.f };

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		// Your own update logic goes here


		float Angle = utilities::getAngle(blackhole.x, blackhole.y, player_pos.x, player_pos.y);
		blackhole.x -= cos(Angle) / 5;
		blackhole.y -= sin(Angle) / 5;
		
		AEInputGetCursorPosition(&x, &y); // check cursor pos
		//std::cout << x << std::endl << std::endl << y;

		cursorAngle = utilities::getAngle(player_pos.x + 600, player_pos.y + 350, x, y);

		player_direction.x = 0;// set y direction to 0 initially, if key is released x direction is set back to 0
		player_direction.y = 0;// set y direction to 0 initially, if key is released y direction is set back to 0

		if (y < planet.y + 350) {
			cursorAngle = -cursorAngle;
		}

		if (cursorAngle <= -(PI * 3 / 4) || cursorAngle > (PI * 3 / 4)) {
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

		if (AEInputCheckCurr(AEVK_Q))
		{
			//std::cout << "diu" << std::endl;
			i -= 0.02;
			std::cout << " rotating" << std::endl;
		}

		if (AEInputCheckCurr(AEVK_W) || AEInputCheckCurr(AEVK_UP)) // movement for W key 
		{
			player_direction.y = 1;// this is direction , positive y direction
			std::cout << "W key" << std::endl;
		}
		if (AEInputCheckCurr(AEVK_S) || AEInputCheckCurr(AEVK_DOWN))
		{
			player_direction.y = -1;// this is direction , negative y direction
			std::cout << "S key" << std::endl;
		}
		if (AEInputCheckCurr(AEVK_A) || AEInputCheckCurr(AEVK_LEFT))
		{
			player_direction.x = -1;// this is direction , negative x direction
			std::cout << "A key" << std::endl;
		}
		if (AEInputCheckCurr(AEVK_D) || AEInputCheckCurr(AEVK_RIGHT))
		{
			player_direction.x = 1;// this is direction , positive y direction
			std::cout << "D key" << std::endl;
		}

		// add velo to player_pos

		if (player_direction.x != 0 || player_direction.y != 0) //if player direction is not 0, as you cannot normalize 0.
		{
			AEVec2 temp_velo{ player_direction.x, player_direction.y };
			AEVec2Normalize(&player_direction, &temp_velo); // normalize
			player_direction.x *= speed; // magnitude/speed of velo.x
			player_direction.y *= speed; // magnitude/speed of velo.y
		}

		player_pos.x += player_direction.x * 0.016; // 0.016 is delta time temp value
		player_pos.y += player_direction.y * 0.016;

		if (AEInputCheckTriggered(AEVK_E) == 1) {
			blackhole.x += 5 * cos(Angle);
			blackhole.y += 5 * sin(Angle);
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
		AEGfxTextureSet(pTex, 16.f/192, 16.f/176 * 8);
		AEMtx33 scale = { 0 };
		// Create a scale matrix that scales by 100 x and y 
		AEMtx33Scale(&scale, 100.f, 100.f);
		// Create a rotation matrix that rotates by 45 degrees 
		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, i);
		// Create a translation matrix that translates by // 100 in the x-axis and 100 in the y-axis 
		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, player_pos.x, player_pos.y);
		// Concat the matrices (TRS) 
		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		// Choose the transform to use 
		AEGfxSetTransform(transform.m);
		// Actually drawing the mesh
		AEGfxMeshDraw(spriteMesh, AE_GFX_MDM_TRIANGLES);

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
			AEMtx33Trans(&translate, player_pos.x + slashX, player_pos.y + slashY);
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
	AEGfxMeshFree(pblack);

	AEGfxTextureUnload(pTex);
	AEGfxTextureUnload(pblacktex);

	// free the system
	AESysExit();
}