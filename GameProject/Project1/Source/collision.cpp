#include "collision.h"


bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
	const AABB& aabb2, const AEVec2& vel2)
{
	// Implement the collision intersection over here. The steps are: 

	// Step 1: Check for static collision detection between rectangles (before moving). 
			//If the check returns no overlap you continue with the following next steps (dynamics).
			//Otherwise you return collision true
	if (!((aabb1.max.x > aabb2.min.x) || (aabb2.max.x > aabb1.min.x)
		|| (aabb1.max.y > aabb2.min.y) || (aabb2.max.y > aabb1.min.y)))
		return 0;

	// Step 2: Initialize and calculate the new velocity of Vb
	AEVec2 vel;
	AEVec2Set(&vel, (vel2.x - vel1.x), (vel2.y - vel1.y));

	float tFirst = 0;
	float tLast = g_dt;

	// Step 3: Working with one dimension (x-axis).
	if (vel.x <= 0)
	{
		if (aabb1.min.x > aabb2.max.x) // case 1
			return 0; // no intersection and B is moving away
		if (aabb1.max.x < aabb2.min.x) //case 4 p1
			tFirst = AEMax((aabb1.max.x - aabb2.min.x) / vel.x, tFirst);
		if (aabb1.min.x < aabb2.max.x) //case 4 p2
			tLast = AEMin((aabb1.min.x - aabb2.max.x) / vel.x, tLast);
	}
	else if (vel.x > 0)
	{
		if (aabb1.max.x < aabb2.min.x) // case 3
			return 0; //no intersection and B is moving away
		if (aabb1.min.x > aabb2.max.x) // case 2 p1
			tFirst = AEMax((aabb1.min.x - aabb2.max.x) / vel.x, tFirst);
		if (aabb1.max.x > aabb2.min.x) // case 2 p2
			tLast = AEMin((aabb1.max.x - aabb2.min.x) / vel.x, tLast);
	}

	// Step 5: Otherwise the rectangles intersect
	if (tFirst > tLast)
		return 0; // no intersection

	// Step 4: Repeat step 3 on the y-axis
	if (vel.y <= 0)
	{
	if (aabb1.min.y > aabb2.max.y) // case 1
		return 0; // no intersection and B is moving away
	if (aabb1.max.y < aabb2.min.y) //case 4 p1
		tFirst = AEMax((aabb1.max.y - aabb2.min.y) / vel.y, tFirst);
	if (aabb1.min.y < aabb2.max.y) //case 4 p2
		tLast = AEMin((aabb1.min.y - aabb2.max.y) / vel.y, tLast);
	}
	else if (vel.y > 0)
	{
	if (aabb1.max.y < aabb2.min.y) // case 3
		return 0; //no intersection and B is moving away
	if (aabb1.min.y > aabb2.max.y) // case 2 p1
		tFirst = AEMax((aabb1.min.y - aabb2.max.y) / vel.y, tFirst);
	if (aabb1.max.y > aabb2.min.y) // case 2 p2
		tLast = AEMin((aabb1.max.y - aabb2.min.y) / vel.y, tLast);
	}

	// Step 5: Otherwise the rectangles intersect
	if (tFirst > tLast)
		return 0; // no intersection

	else return 1;
}

int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY, int binaryMap[124][42])
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

void snaptocelladd(float* coordinate) {
	*coordinate = static_cast<float>(static_cast<int>(*coordinate)) + 0.5f;
}

void snaptocellsub(float* coordinate) {
	*coordinate = static_cast<float>(static_cast<int>(*coordinate)) - 0.5f;
}



//bool ShittyCollisionMap(float posx, float posy) {
//	if (binaryMap[((int(posx + 8)))][abs((int(posy)))]) {
//		std::cout << "r" << std::endl;
//
//	}
//	if (binaryMap[((int(posx - 8)))][abs((int(posy)))]) {
//		std::cout << "l" << std::endl;
//
//	}
//	if (binaryMap[((int(posx)))][abs((int(posy)))+8]) {
//		std::cout << "t" << std::endl;
//
//	}
//	if (binaryMap[((int(posx)))][abs((int(posy)))-8]) {
//		std::cout << "b" << std::endl;
//
//	}
//	return 0;
//}

//binaryMap[((int(Player->posCurr.x)))][abs((int(Player->posCurr.y)))])
