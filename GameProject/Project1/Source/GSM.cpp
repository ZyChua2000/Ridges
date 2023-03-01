/******************************************************************************/
/*!
\file		GSM.cpp
\author 	Chua Zheng Yang, c.zhengyang, 620005922
\par    	email: c.zhengyang\@digipen.edu
\date   	February 01, 2023
\brief		This source file contains all the functions for the Game state Manager

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Main.h"

// ---------------------------------------------------------------------------
// globals

// variables to keep track the current, previous and next game state
unsigned int	gGameStateInit;
unsigned int	gGameStateCurr;
unsigned int	gGameStatePrev;
unsigned int	gGameStateNext;

// pointer to functions for game state life cycles functions
void (*GameStateLoad)() = 0;
void (*GameStateInit)() = 0;
void (*GameStateUpdate)() = 0;
void (*GameStateDraw)() = 0;
void (*GameStateFree)() = 0;
void (*GameStateUnload)() = 0;

/******************************************************************************/
/*!
	This function initialises the Game state manager into the first game state
*/
/******************************************************************************/
void GameStateMgrInit(unsigned int gameStateInit)
{
	// set the initial game state
	gGameStateInit = gameStateInit;

	// reset the current, previous and next game
		gGameStateCurr =
		gGameStatePrev =
		gGameStateNext = gGameStateInit;

	// call the update to set the function pointers
	GameStateMgrUpdate();
}

/******************************************************************************/
/*!
	This function updates the Game State Manager to switch the function pointers
	to point to the next game state's functions
*/
/******************************************************************************/
void GameStateMgrUpdate()
{
	if ((gGameStateCurr == GS_RESTART) || (gGameStateCurr == GS_QUIT))
		return;

	switch (gGameStateCurr)
	{
	case GS_MAINMENU:
		GameStateLoad = GS_MainMenu_Load;
		GameStateInit = GS_MainMenu_Init;
		GameStateUpdate = GS_MainMenu_Update;
		GameStateDraw = GS_MainMenu_Draw;
		GameStateFree = GS_MainMenu_Free;
		GameStateUnload = GS_MainMenu_Unload;
		break;
	case GS_WORLD:
		GameStateLoad = GS_World_Load;
		GameStateInit = GS_World_Init;
		GameStateUpdate = GS_World_Update;
		GameStateDraw = GS_World_Draw;
		GameStateFree = GS_World_Free;
		GameStateUnload = GS_World_Unload;
		break;
	case GS_MAZE:
		GameStateLoad = GS_Maze_Load;
		GameStateInit = GS_Maze_Init;
		GameStateUpdate = GS_Maze_Update;
		GameStateDraw = GS_Maze_Draw;
		GameStateFree = GS_Maze_Free;
		GameStateUnload = GS_Maze_Unload;
		break;
	case GS_COLOSSEUM:
		GameStateLoad = GS_Colosseum_Load;
		GameStateInit = GS_Colosseum_Init;
		GameStateUpdate = GS_Colosseum_Update;
		GameStateDraw = GS_Colosseum_Draw;
		GameStateFree = GS_Colosseum_Free;
		GameStateUnload = GS_Colosseum_Unload;
		break;
	case GS_DEATHSCREEN:
		GameStateLoad = GS_DeathScreen_Load;
		GameStateInit = GS_DeathScreen_Init;
		GameStateUpdate = GS_DeathScreen_Update;
		GameStateDraw = GS_DeathScreen_Draw;
		GameStateFree = GS_DeathScreen_Free;
		GameStateUnload = GS_DeathScreen_Unload;
		break;
	default:
		AE_FATAL_ERROR("invalid state!!");
	}
}
