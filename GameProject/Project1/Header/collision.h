#pragma once

#include "GameObjs.h"
#include <iostream>
#include "Globals.h"


extern float			    g_dt;


bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
	const AABB& aabb2, const AEVec2& vel2); //AABB STRUCT FOR PLAYER, AEVEC2 VELOCITY OF PLAYER,AABB STRUCT FOR OBJECT, AEVEC2 VELOCITY OF OBJECT

int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY, int binaryMap[124][42]);



bool ShittyCollisionMap(float posx, float posy);

void snaptocelladd(float* coordinate);

void snaptocellsub(float* coordinate);

