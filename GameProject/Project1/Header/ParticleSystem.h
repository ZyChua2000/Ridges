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
#include "AEEngine.h"


void ParticleSystemLoad();
void ParticleSystemInit();
void ParticleSystemUpdate();
void ParticleSystemDraw(AEMtx33* localTransform);
void ParticleSystemFree();
void ParticleSystemUnload();
void ParticleSystemRequest(unsigned int type, float scale, AEVec2* pos, AEVec2* vel, float dir, float life, unsigned int amount);

