/******************************************************************************/
/*!
\file		GameStateList.h
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 01, 2023
\brief		This header file contains the enums representing all the game states
		

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/

#ifndef CSD1130_GAME_STATE_LIST_H_
#define CSD1130_GAME_STATE_LIST_H_

// ---------------------------------------------------------------------------
// game state list



enum
{
	// list of all game states 
	GS_MAINMENU = 0,
	GS_WORLD,
	GS_BOSSLEVEL,
	GS_MAZE,
	GS_COLOSSEUM,
	GS_TOWER,
	GS_DEATHSCREEN,
	GS_HELP,
	
	// special game state.
	GS_RESTART,
	GS_QUIT, 
	GS_NONE
};

// ---------------------------------------------------------------------------

#endif // CSD1130_GAME_STATE_LIST_H_

