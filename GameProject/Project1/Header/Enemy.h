#pragma once
#include "AEEngine.h"
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
	bool b_Closed = false;
	float f_hcost;
	float f_fcost;
	AEVec2 ae_NodePos;
	std::vector<Node*> v_Neighbours;
	Node* parent;
};

void NodesInit(int *grid , int width, int height);
std::vector<Node*> pathfind(float x, float y, float x1, float y1);
float distance(Node* a, Node* b);
float heuristic(Node* a, Node* b);
void deletenodes();
