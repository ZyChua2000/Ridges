/******************************************************************************/
/*!
\file		Main.h
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 01, 2023
\brief		This header file stores global variables meant for usage of main.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef CS230_MAIN_H_
#define CS230_MAIN_H_

//------------------------------------
// Globals

extern float	g_dt;
extern double	g_appTime;

// ---------------------------------------------------------------------------
// includes
#include <vector>

// Levels
#include "Mainmenu.h"
#include "World.h"
#include "Maze.h"
#include "Tower.h"
#include "Colosseum.h"
#include "BossLevel.h"
// Screens
#include "WinScreen.h"
#include "DeathScreen.h"
#include "HelpScreen.h"
#include "SplashScreen.h"

#include "AEEngine.h"
#include "GameStateList.h"
#include "GSM.h"
// Functions
#include "GameObjs.h"
#include "collision.h"
#include "Enemy.h"
#include "ParticleSystem.h"
#include "Utilities.h"
#include "mapFunctions.h"
#include "AEVecOverloads.h"

#include "Globals.h"











#endif











