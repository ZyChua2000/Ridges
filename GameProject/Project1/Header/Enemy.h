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

class state
{
	// virtual keyword is to override the function
public:
	virtual void on_enter(GameObjInst* pInst);
	virtual void on_update(GameObjInst* pInst);
	virtual void on_exit(GameObjInst* pInst);

protected:

};

class Basic_Atk :state
{

};

class AOE :state
{

};

class spawn_enemy :state
{

};

class spawn_bullet :state
{

};

class spawn_blind :state
{

};

enum STATE // MOVESETS FOR BOSS
{
	STATE_PATROL = 0,//IDLE STATE OR NO STATE
	STATE_BASIC, //STATE FOR BASIC ATTACK
	STATE_AOE,  //STATE FOR AOE ATTACK
	STATE_SPAWN_ENEMIES, // STATE TO SPAWN ENEMIES
	STATE_SPAWN_BULLETS, // STATE TO SPAWN BULLETS
	STATE_MAZE_DARKEN,	// STATE TO DARKEN THE WHOLE LEVEL 
	STATE_SPAWN_BLACKHOLE //STATE TO SPAWN BLACKHOLE //not a must have due to time constrain

};

enum INNER_STATE // INNER STATE FOR BOSS
{
	INNER_STATE_ON_ENTER,
	INNER_STATE_ON_UPDATE,
	INNER_STATE_ON_EXIT
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

/******************************************************************
function declaration for boss state machine
*******************************************************************/
void BossStateMachine(GameObjInst* pInst);
