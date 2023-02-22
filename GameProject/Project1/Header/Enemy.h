#pragma once
#include "AEEngine.h"
#include "GameObjs.h"
#include "math.h"
#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <vector>

//struct Enemy:GameObjInst {
//
//	Enemy()
//	{
//		
//	}
//
//
//};
//
//void enemy_Init();


struct Node 
{
	bool b_Obstacle = false;
	bool b_Visited = false;
	float f_DistLeft;
	float f_AltDist;
	AEVec2 ae_NodePos;
	std::vector<Node*> v_Neighbours;
	Node* parent;
};

void NodesInit(int** grid, Node node );