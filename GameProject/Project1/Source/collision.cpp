/******************************************************************************/
/*!
\file		collision.cpp
\author 	Jaasmeet Singh
\par    	email: jaasmeet.s@digipen.edu
\date   	february 02, 2023
\brief		File containing collision functions

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "collision.h"

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
	const AABB& aabb2, const AEVec2& vel2)
{


	//Step 1: Check for static collision detection between rectangles (before moving). 
	//			If the check returns no overlap you continue with the following next steps (dynamics).
	//			Otherwise you return collision true
	if (aabb1.max.x < aabb2.min.x || aabb1.min.x > aabb2.max.x || aabb1.max.y < aabb2.min.y || aabb1.min.y > aabb2.max.y) {
		//Check that both objects are not static. If both are static, return false
		if (vel1.x == 0 || vel1.y == 0 || vel2.x == 0 || vel2.y == 0) {
			return false;
		}
		else {

			//Step 2: Initialize and calculate the new velocity of Vb

			AEVec2 Vb = { vel2.x - vel1.x,vel2.y - vel1.y }; //Normalise vectors
			AEVec2 tFirst = { 0,0 };
			AEVec2 tLast = { (float)(AEFrameRateControllerGetFrameTime()),(float)(AEFrameRateControllerGetFrameTime()) };

			//Step 3: Working with one dimension (x-axis).
			if (Vb.x > 0) {
				// Case 3: Boxes moving away from each other
				if (aabb1.max.x < aabb2.min.x) {
					return false;
				}

				//Case 2:
				if (aabb1.min.x > aabb2.max.x) {
					// Maximum between dFirst/Vb and original tFirst value for x
					if (tFirst.x < (aabb1.min.x - aabb2.max.x) / Vb.x) {
						tFirst.x = (aabb1.min.x - aabb2.max.x) / Vb.x;
					}
					// Minimum between dLast/Vb and original tLast value for x
					if (tLast.x > (aabb1.max.x - aabb2.min.x) / Vb.x) {
						tLast.x = (aabb1.max.x - aabb2.min.x) / Vb.x;
					}
				}
			}

			if (Vb.x < 0) {
				//Case 1: Boxes moving away from each other
				if (aabb1.min.x > aabb2.max.x) {
					return false;
				}

				//Case 4:
				if (aabb1.max.x < aabb2.min.x) {
					// Maximum between dFirst/Vb and original tFirst value for x
					if (tFirst.x < (aabb1.max.x - aabb2.min.x) / Vb.x) {
						tFirst.x = (aabb1.max.x - aabb2.min.x) / Vb.x;
					}
					// Minimum between dLast/Vb and original tLast value for x
					if (tLast.x > (aabb1.min.x - aabb2.max.x) / Vb.x) {
						tLast.x = (aabb1.min.x - aabb2.max.x) / Vb.x;
					}
				}
			}

			//Step 4: Repeat step 3 on the y-axis
			if (Vb.y > 0) {
				// Case 3: Boxes moving away from each other
				if (aabb1.max.y < aabb2.min.y) {
					return false;
				}

				//Case 2:
				if (aabb1.min.y > aabb2.max.y) {
					// Maximum between dFirst/Vb and original tFirst value for y
					if (tFirst.y < (aabb1.min.y - aabb2.max.y) / Vb.y) {
						tFirst.y = (aabb1.min.y - aabb2.max.y) / Vb.y;
					}
					// Minimum between dLast/Vb and original tLast value for y
					if (tLast.y > (aabb1.max.y - aabb2.min.y) / Vb.y) {
						tLast.y = (aabb1.max.y - aabb2.min.y) / Vb.y;
					}
				}
			}

			if (Vb.y < 0) {
				// Case 1: Boxes moving away from each other
				if (aabb1.min.y > aabb2.max.y) {
					return false;
				}

				// Case 4:
				if (aabb1.max.y < aabb2.min.y) {
					// Maximum between dFirst/Vb and original tFirst value for y
					if (tFirst.y < (aabb1.max.y - aabb2.min.y) / Vb.y) {
						tFirst.y = (aabb1.max.y - aabb2.min.y) / Vb.y;
					}
					// Minimum between dLast/Vb and original tLast value for y
					if (tLast.y > (aabb1.min.y - aabb2.max.y) / Vb.y) {
						tLast.y = (aabb1.min.y - aabb2.max.y) / Vb.y;
					}
				}
			}

			// Check if ray passes through box
			if (tFirst.x > tLast.y || tFirst.y > tLast.x) {
				return false;
			}

			//Step 5: Otherwise the rectangles intersect
			else {
				return true;
			}
		}
	}// For static collision
	else {
		return true;
	}

}

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
int CheckInstanceBinaryMapCollision(float PosX, float PosY, int binaryMap[124][42], float scaleX, float scaleY)
{
	int Flag = 0;
	int x1, y1, x2, y2;

	//std::cout << "X: " << PosX << "\tY: " << PosY << std::endl;

	// Check the right side of the object instance
	//Hotspot 1f
	x1 = int(PosX + scaleX / 2);
	y1 = int(PosY + scaleY / 4);
	//Hotspot 2
	x2 = int(PosX + scaleX / 2);
	y2 = int(PosY - scaleY / 4);

	//std::cout << "X1: " << x1 << "\tY1: " << y1 << std::endl;
	//std::cout << "X2: " << x2 << "\tY2: " << y2 << std::endl;


	if (binaryMap[x1][abs(y1)] == 1 || binaryMap[x2][abs(y2)] == 1) {
		Flag |= COLLISION_RIGHT;
		std::cout << "r" << std::endl;

	}


	// Check the left side of the object instance
	//Hotspot 1
	x1 = int(PosX - scaleX / 2);
	y1 = int(PosY - scaleY / 4);
	//Hotspot 2
	x2 = int(PosX - scaleX / 2);
	y2 = int(PosY + scaleY / 4);


	if (binaryMap[x1][abs(y1)] == 1 || binaryMap[x2][abs(y2)] == 1) {
		Flag |= COLLISION_LEFT;
		std::cout << "l" << std::endl;
	}

	// Check the top side of the object instance
	//Hotspot 1
	x1 = int(PosX - scaleX / 4);
	y1 = int(PosY + scaleY / 2);
	//Hotspot 2
	x2 = int(PosX + scaleX / 4);
	y2 = int(PosY + scaleY / 2);


	if (binaryMap[x1][abs(y1)] == 1 || binaryMap[x2][abs(y2)] == 1) {
		Flag |= COLLISION_BOTTOM;
		std::cout << "b" << std::endl;
	}


	// Check the top side of the object instance
	//Hotspot 1
	x1 = int(PosX + scaleX / 4);
	y1 = int(PosY - scaleY / 2);
	//Hotspot 2
	x2 = int(PosX - scaleX / 4);
	y2 = int(PosY - scaleY / 2);


	if (binaryMap[x1][abs(y1)] == 1 || binaryMap[x2][abs(y2)] == 1) {
		Flag |= COLLISION_TOP;
		std::cout << "t" << std::endl;
	}

	return Flag;
}
/******************************************************************************/
/*!
	\brief This function snaps player to grid of x axis
	\param float* coordinate
*/
/******************************************************************************/
void snaptocelladd(float* coordinate) {
	*coordinate = static_cast<float>(static_cast<int>(*coordinate)) + 0.5f;
}
/******************************************************************************/
/*!
	\brief This function snaps player to grid of inverted y axis
	\param float* coordinate
*/
/******************************************************************************/
void snaptocellsub(float* coordinate) {
	*coordinate = static_cast<float>(static_cast<int>(*coordinate)) - 0.5f;
}

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
int CheckInstanceBinaryMapCollisionCollo(float PosX, float PosY, int binaryMap[28][29], float scaleX, float scaleY)
{
	int Flag = 0;
	int x1, y1, x2, y2;

	//std::cout << "X: " << PosX << "\tY: " << PosY << std::endl;

	// Check the right side of the object instance
	//Hotspot 1f
	x1 = int(PosX + scaleX / 2);
	y1 = int(PosY + scaleY / 4);
	//Hotspot 2
	x2 = int(PosX + scaleX / 2);
	y2 = int(PosY - scaleY / 4);

	//std::cout << "X1: " << x1 << "\tY1: " << y1 << std::endl;
	//std::cout << "X2: " << x2 << "\tY2: " << y2 << std::endl;


	if (binaryMap[x1][abs(y1)] == 1 || binaryMap[x2][abs(y2)] == 1) {
		Flag |= COLLISION_RIGHT;
		std::cout << "r" << std::endl;

	}


	// Check the left side of the object instance
	//Hotspot 1
	x1 = int(PosX - scaleX / 2);
	y1 = int(PosY - scaleY / 4);
	//Hotspot 2
	x2 = int(PosX - scaleX / 2);
	y2 = int(PosY + scaleY / 4);


	if (binaryMap[x1][abs(y1)] == 1 || binaryMap[x2][abs(y2)] == 1) {
		Flag |= COLLISION_LEFT;
		std::cout << "l" << std::endl;
	}

	// Check the top side of the object instance
	//Hotspot 1
	x1 = int(PosX - scaleX / 4);
	y1 = int(PosY + scaleY / 2);
	//Hotspot 2
	x2 = int(PosX + scaleX / 4);
	y2 = int(PosY + scaleY / 2);


	if (binaryMap[x1][abs(y1)] == 1 || binaryMap[x2][abs(y2)] == 1) {
		Flag |= COLLISION_BOTTOM;
		std::cout << "b" << std::endl;
	}


	// Check the top side of the object instance
	//Hotspot 1
	x1 = int(PosX + scaleX / 4);
	y1 = int(PosY - scaleY / 2);
	//Hotspot 2
	x2 = int(PosX - scaleX / 4);
	y2 = int(PosY - scaleY / 2);


	if (binaryMap[x1][abs(y1)] == 1 || binaryMap[x2][abs(y2)] == 1) {
		Flag |= COLLISION_TOP;
		std::cout << "t" << std::endl;
	}

	return Flag;
}

/******************************************************************************/
/*!
	\brief This function snaps and implements collision
	\param GameObjInst& Object
	\param int& flag
	\return true if collides, else false

*/
/******************************************************************************/
bool snapCollision(GameObjInst& Object, int& flag) {
	bool trueflag = false;
	if (flag & COLLISION_TOP) {
		//Top collision
		std::cout << "collide top" << std::endl;
		snaptocellsub(&Object.posCurr.y);
		//Player->posCurr.y + 0.5;
		trueflag = true;
	}

	if (flag & COLLISION_BOTTOM) {
		//bottom collision
		std::cout << "collide botton" << std::endl;
		snaptocellsub(&Object.posCurr.y);

		//Player->posCurr.y - 0.5;
		trueflag = true;
	}

	if (flag & COLLISION_LEFT) {
		//Left collision
		std::cout << "collide left" << std::endl;
		snaptocelladd(&Object.posCurr.x);

		//Player->posCurr.x + 0.5;
		trueflag = true;
	}
	if (flag & COLLISION_RIGHT) {
		//Right collision
		std::cout << "collide right" << std::endl;
		snaptocelladd(&Object.posCurr.x);

		//Player->posCurr.x - 0.5;
		trueflag = true;
	}
	return trueflag;
}

