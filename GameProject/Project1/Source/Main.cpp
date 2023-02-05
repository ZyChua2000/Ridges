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

#include "main.h"
#include <memory>
#include <iostream>

// ---------------------------------------------------------------------------
// Globals
float	 g_dt;
double	 g_appTime;

u32	gAEGameStateInit = GS_MAINMENU;           //!< Initial GameState
u32 gAEGameStateCurr = GS_MAINMENU;;           //!< Current GameState
u32 gAEGameStatePrev = GS_MAINMENU;;           //!< Previous GameState
u32 gAEGameStateNext = GS_MAINMENU;;           //!< Next GameState

void (*AEGameStateLoad)(void) =nullptr;    //!< Function pointer for load
void (*AEGameStateInit)(void) =nullptr;    //!< Function pointer for init
void (*AEGameStateUpdate)(void) =nullptr;  //!< Function pointer for update
void (*AEGameStateDraw)(void) =nullptr;    //!< Function pointer for draw
void (*AEGameStateFree)(void) = nullptr;    //!< Function pointer for free
void (*AEGameStateUnload)(void)= nullptr;  //!< Function pointer for unload


/******************************************************************************/
/*!
	Starting point of the application
*/
/******************************************************************************/
int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{

	//SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);
	std::cout << "Test";
	//// Enable run-time memory check for debug builds.
	//#if defined(DEBUG) | defined(_DEBUG)
		//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//#endif

	//int * pi = new int;
	//delete pi;

	// Initialize the system
	AESysInit(instanceH, show, 1600, 900, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("Dungeon Redemption");

	//set background color
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	AEGameStateMgrAdd(GS_MAINMENU, AEGameStateLoad=GS_MainMenu_Load, AEGameStateInit= GS_MainMenu_Init, AEGameStateUpdate= GS_MainMenu_Update, 
		AEGameStateDraw= GS_MainMenu_Draw, AEGameStateFree= GS_MainMenu_Free, AEGameStateUnload= GS_MainMenu_Unload);
	AEGameStateMgrAdd(GS_WORLD, AEGameStateLoad = GS_World_Load, AEGameStateInit = GS_World_Init, AEGameStateUpdate = GS_World_Update,
		AEGameStateDraw = GS_World_Draw, AEGameStateFree = GS_World_Free, AEGameStateUnload = GS_World_Unload);
	//AEGameStateMgrAdd(GS_BOSSLEVEL, &GS_BossLevel_Load, &GS_BossLevel_Init, &GS_BossLevel_Update, &GS_BossLevel_Draw, &GS_BossLevel_Free, &GS_BossLevel_Unload);

	AEGameStateMgrInit(gAEGameStateInit);

	while (gAEGameStateCurr != GS_QUIT)
	{
		// reset the system modules
		AESysReset();

		// If not restarting, load the gamestate
		if (gAEGameStateCurr != GS_RESTART)
		{
			AEGameStateMgrUpdate();
			AEGameStateLoad();
		}
		else
			gAEGameStateNext = gAEGameStateCurr = gAEGameStatePrev;

		// Initialize the gamestate
		AEGameStateInit();

		while (gAEGameStateCurr == gAEGameStateNext)
		{
			AESysFrameStart();

			AEInputUpdate();

			AEGameStateUpdate();

			AEGameStateDraw();

			AESysFrameEnd();

			// check if forcing the application to quit
			if ((AESysDoesWindowExist() == false) || AEInputCheckTriggered(AEVK_ESCAPE))
				gAEGameStateNext = GS_QUIT;

			g_dt = (f32)AEFrameRateControllerGetFrameTime();
			g_appTime += g_dt;
		}

		AEGameStateFree();

		if (gAEGameStateNext != GS_RESTART)
			AEGameStateUnload();

		gAEGameStatePrev = gAEGameStateCurr;
		gAEGameStateCurr = gAEGameStateNext;
	}

	// free the system
	AESysExit();
}
