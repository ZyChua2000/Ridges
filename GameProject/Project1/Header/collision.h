/******************************************************************************/
/*!
\file		collision.h
\author 	Jaasmeet Singh
\par    	email: jaasmeet.s@digipen.edu
\date   	february 02, 2023
\brief		File containing collision functions

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/


#include "main.h"


extern float			    g_dt;


bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
	const AABB& aabb2, const AEVec2& vel2); //AABB STRUCT FOR PLAYER, AEVEC2 VELOCITY OF PLAYER,AABB STRUCT FOR OBJECT, AEVEC2 VELOCITY OF OBJECT

int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY, int binaryMap[124][42]);

int CheckInstanceBinaryMapCollisionCollo(float PosX, float PosY, float scaleX, float scaleY, int binaryMap[28][29]);



//bool ShittyCollisionMap(float posx, float posy);

void snaptocelladd(float* coordinate);

void snaptocellsub(float* coordinate);

bool snapCollision(GameObjInst& Object, int& flag);
