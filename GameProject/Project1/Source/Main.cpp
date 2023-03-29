/******************************************************************************/
/*!
\file		Main.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 02, 2023
\brief		This source file contains the main function, entry point of the
program.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Main.h"
#include <memory>
#include <iostream>

// ---------------------------------------------------------------------------
// Globals
float	 g_dt;
double	 g_appTime;

/******************************************************************************/
/*!
	Starting point of the application
*/
/******************************************************************************/
int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//int * pi = new int;
	//delete pi;
		//_crtBreakAlloc = 4272;

	// Initialize the system
	AESysInit(instanceH, show, 1600, 900, 1, 60, false, NULL);

	AEGfxCreateFont("Assets/OpenSans-Regular.ttf", 12);

	// Changing the window title
	AESysSetWindowTitle("Dungeon Redemption");

	//set background color
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	GameStateMgrInit(GS_MAINMENU);

	while (gGameStateCurr != GS_QUIT)
	{
		// reset the system modules
		AESysReset();

		// If not restarting, load the gamestate
		if (gGameStateCurr != GS_RESTART)
		{
			GameStateMgrUpdate();
			GameStateLoad();
		}
		else
			gGameStateNext = gGameStateCurr = gGameStatePrev;

		// Initialize the gamestate
		GameStateInit();

		while (gGameStateCurr == gGameStateNext)
		{
			AESysFrameStart();

			AEInputUpdate();

			GameStateUpdate();
			

			GameStateDraw();

			AESysFrameEnd();

			// check if forcing the application to quit
			if ((AESysDoesWindowExist() == false))
				gGameStateNext = GS_QUIT;

			g_dt = (f32)AEFrameRateControllerGetFrameTime();

			//capping the game loop - delta time, to 1/60.0f
			if (g_dt > 1.f/30)	//0.01667f = 1/60.0f
				g_dt = 1.f / 30;

			g_appTime += g_dt;
		}
		GameStateFree();

		if (gGameStateNext != GS_RESTART)
			GameStateUnload();

		gGameStatePrev = gGameStateCurr;
		gGameStateCurr = gGameStateNext;
	}

	AEGfxDestroyFont(1);
	// free the system
	AESysExit();
}
