/******************************************************************************/
/*!
\file		ParticleSystem.h
\author 	Jaasmeet Singh
\par    	email: jaasmeet.s@digipen.edu
\date   	March 01, 2023
\brief		header file for particle system

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#pragma once
#include "main.h"
/******************************************************************************/
/*!
	File globals
*/
/******************************************************************************/
extern float internalTimer;

/******************************************************************************/
/*!
	\brief this function loads particle assets
*/
/******************************************************************************/
void ParticleSystemLoad();

/******************************************************************************/
/*!
	\brief this function initializes particle system variables
*/
/******************************************************************************/
void ParticleSystemInit();

/******************************************************************************/
/*!
	\brief this function updates logic of particle system variables
*/
/******************************************************************************/
void ParticleSystemUpdate();

/******************************************************************************/
/*!
	\brief this function draws assets of particle
	\param AEMtx33* localTransform
*/
/******************************************************************************/
void ParticleSystemDraw(AEMtx33* localTransform);

/******************************************************************************/
/*!
	\brief this function free up variable that has allocated memory
*/
/******************************************************************************/
void ParticleSystemFree();

/******************************************************************************/
/*!
	\brief this function unloads particle assets
*/
/******************************************************************************/
void ParticleSystemUnload();

/******************************************************************************/
/*!
	\brief this function request the system to spawn the particles
	\param unsigned int type
	\param float scale
	\param AEVec2* pos
	\param AEVec2* vel
	\param float dir
	\param float life
	\param unsigned int amount
*/
/******************************************************************************/
void ParticleSystemRequest(unsigned int type, float scale, AEVec2* pos, AEVec2* vel, float dir, float life, unsigned int amount);

