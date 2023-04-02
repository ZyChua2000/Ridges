/******************************************************************************/
/*!
\file		Enemy.h
\author 	Javier Low Zhi Liang
\par    	email: zhiliangjavier.low@digipen.edu
\date   	February 02, 2023
\brief		This source file contains function declarations for enemy interactivity like A* path finding, Initialising nodes for A* and enemy movement.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#pragma once
#include "AEEngine.h"
#include "math.h"
#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

/******************************************************************
node struct for path finding
*******************************************************************/
struct Node 
{
	bool b_Obstacle = false;
	bool b_Closed = false;
	float f_hcost{};
	float f_fcost{};
	AEVec2 ae_NodePos{};
	std::vector<Node*> v_Neighbours;
	Node* parent;
};

/******************************************************************/
/*!
\brief function definition for Initialising node variables
\param int *grid , taking in a pointer of the Binary Grid level
\param int width , taking in width of map
\param int height , taking in height of map
*/
/*******************************************************************/
void NodesInit(int *grid , int width, int height);


/******************************************************************/
/*!
\brief function definition for A* pathfinding
\param float x ,taking in x coordinate of Player GameObj
\param float y ,taking in y coordinate of Player GameObj
\param float x1,taking in x coordinate of Enemy GameObj
\param float y1,taking in y coordinate of Enemy GameObj
\return shortest of type vector holding node pointers
*/
/*******************************************************************/
std::vector<Node*> pathfind(float x, float y, float x1, float y1);


/******************************************************************/
/*!
\brief function definition for calculating distance
\param Node* a , taking in a pointer of type Node
\param Node* b, taking in a pointer of type Node
\return distance of type float.
*/
/*******************************************************************/
float distance(Node* a, Node* b);

/******************************************************************/
/*!
\brief function definition for calculating hcost of nodes
\param Node* a , taking in a pointer of type Node
\param Node* b, taking in a pointer of type Node
\return hcost of type float.
*/
/*******************************************************************/
float heuristic(Node* a, Node* b);

/******************************************************************/
/*!
\brief function definition deleting nodes
*/
/*******************************************************************/
void deletenodes();
