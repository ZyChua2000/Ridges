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
	float f_hcost;
	float f_fcost;
	AEVec2 ae_NodePos;
	std::vector<Node*> v_Neighbours;
	Node* parent;
};


/******************************************************************
function declaration for Initialising node variables
*******************************************************************/
void NodesInit(int *grid , int width, int height);

/******************************************************************
function declaration for A* pathfinding
*******************************************************************/
std::vector<Node*> pathfind(float x, float y, float x1, float y1);

/******************************************************************
function declaration for calculating distance
*******************************************************************/
float distance(Node* a, Node* b);

/******************************************************************
function declaration for calculating hcost of nodes
*******************************************************************/
float heuristic(Node* a, Node* b);

/******************************************************************
function declaration deleting nodes 
*******************************************************************/
void deletenodes();
