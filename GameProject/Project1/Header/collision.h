#pragma once
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

/******************************************************************************/
/*!
	\brief This function checks for the intersection between two bounding boxes
	\param const AABB& aabb1
	\param const AEVec2& vel1
	\param const AABB& aabb2
	\param const AEVec2& vel2
	\return true if intersection happens, else return false
*/
/******************************************************************************/
bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
	const AABB& aabb2, const AEVec2& vel2); //AABB STRUCT FOR PLAYER, AEVEC2 VELOCITY OF PLAYER,AABB STRUCT FOR OBJECT, AEVEC2 VELOCITY OF OBJECT

/******************************************************************************/
/*!
	\brief This function checks the instance of binary map collision
	\param float PosX
	\param float PosY
	\param int binaryMap[][]
	\param float scaleX
	\return sets flag 
*/
/******************************************************************************/
int CheckInstanceBinaryMapCollision(float PosX, float PosY, int binaryMap[124][42], float scaleX = 1.0f, float scaleY = 1.0f);

/******************************************************************************/
/*!
	\brief This function checks the instance of binary map collision for coloseum
	\param float PosX
	\param float PosY
	\param int binaryMap[][]
	\param float scaleX
	\return sets flag
*/
/******************************************************************************/
int CheckInstanceBinaryMapCollisionCollo(float PosX, float PosY, int binaryMap[28][29], float scaleX = 1.0f, float scaleY = 1.0f);

/******************************************************************************/
/*!
	\brief This function snaps player to grid of x axis
	\param float* coordinate
*/
/******************************************************************************/
void snaptocelladd(float* coordinate);

/******************************************************************************/
/*!
	\brief This function snaps player to grid of inverted y axis
	\param float* coordinate
*/
/******************************************************************************/
void snaptocellsub(float* coordinate);

/******************************************************************************/
/*!
	\brief This function snaps and implements collision
	\param GameObjInst& Object
	\param int& flag
	\return true if collides, else false

*/
/******************************************************************************/
bool snapCollision(GameObjInst& Object, int& flag);
