/******************************************************************************/
/*!
\file		Enemy.cpp
\author 	Javier Low Zhi Liang
\par    	email: zhiliangjavier.low@digipen.edu
\date   	February 02, 2023
\brief		This source file contains function definitions for enemy interactivity like A* path finding, 
			Initialising nodes for A* and enemy movement.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "main.h"
#include <iostream>
#include <list>
#include <vector>

/******************************************************************************/
/*!
	File globals
*/
/******************************************************************************/
int path_width{};
int path_height{};
float dist{};

extern Node* nodes{};
Node n_enemy{};
Node n_player{};


/******************************************************************/
/*!
\brief function definition for Initialising node variables
\param int *grid , taking in a pointer of the Binary Grid level
\param int width , taking in width of map 
\param int height , taking in height of map 	
*/
/*******************************************************************/
void NodesInit(int *grid, int width, int height)
{

	path_width = width; 
	path_height = height;
	nodes = new Node[path_width * path_height]; // allocate mem for *nodes to point to array holding type Nodes


	for (int x = 0; x < path_width; x++)// nested for loop to init struct members for Node
	{
		for (int y = 0; y < path_height; y++)
		{
			int index = y * path_width + x;
			Node* node = &nodes[index];

			nodes[index].ae_NodePos.x = x +0.5f; //init pos x for node, + 0.5 offset to set node in the middle of x axis grid
			nodes[index].ae_NodePos.y = -y -0.5f; //init pos y for node, - 0.5 offset to set node in the middle of y axis grid
			nodes[index].parent = nullptr; //init parent to point to nothing first 
			nodes[index].b_Closed = false; //init closed node to false

			// in bin map , 1 is wall, 0 is not
			if (*(grid+ x * height + y) == 1) // to check binary grid for obstacle
			{
 				
 				nodes[y * path_width + x].b_Obstacle = true; //if grid[x][y] == 1, it is  obstacle and set to true
			}
			else
			{
				nodes[y * path_width + x].b_Obstacle = false; // else, everything is not an obstacle

			}
		}
	}

	for (int x = 0; x < path_width; x++) // 2nd nested to to connect all the nodes together
	{
		for (int y = 0; y < path_height; y++)
		{
			Node* node = &nodes[y * path_width + x];

			//connecting the nodes together horizontally and vertically

			if (y > 0)//checking to prevent adding nodes that dont exist
				nodes[y * path_width + x].v_Neighbours.push_back(&nodes[(y - 1) * path_width + (x + 0)]); // connecting with node above current

			if (y < path_height - 1)//checking to prevent adding nodes that dont exist
				nodes[y * path_width + x].v_Neighbours.push_back(&nodes[(y + 1) * path_width + (x + 0)]); // connecting with node below current

			if (x > 0)//checking to prevent adding nodes that dont exist
				nodes[y * path_width + x].v_Neighbours.push_back(&nodes[(y + 0) * path_width + (x - 1)]); // connecting with right node 

			if (x < path_width - 1)//checking to prevent adding nodes that dont exist
				nodes[y * path_width + x].v_Neighbours.push_back(&nodes[(y + 0) * path_width + (x + 1)]); // connecting with left node

			
			  
			//connect diagonally

			if (y>0 && x>0)//checking to prevent adding nodes that dont exist
				nodes[y*path_width + x].v_Neighbours.push_back(&nodes[(y - 1) * path_width + (x - 1)]);

			if (y<path_height-1 && x>0)//checking to prevent adding nodes that dont exist
				nodes[y*path_width + x].v_Neighbours.push_back(&nodes[(y + 1) * path_width + (x - 1)]);

			if (y>0 && x<path_width-1)//checking to prevent adding nodes that dont exist
				nodes[y*path_width + x].v_Neighbours.push_back(&nodes[(y - 1) * path_width + (x + 1)]);

			if (y<path_height - 1 && x<width-1)//checking to prevent adding nodes that dont exist
				nodes[y*path_width + x].v_Neighbours.push_back(&nodes[(y + 1) * path_width + (x + 1)]);
			
		}
	}

	std::cout << "Nodes Initialised" << std::endl;
}


/******************************************************************/
/*!
\brief function definition for calculating distance
\param Node* a , taking in a pointer of type Node
\param Node* b, taking in a pointer of type Node
\return distance of type float.
*/
/*******************************************************************/

float distance(Node* a, Node* b) // to calculate dist
{
	return sqrtf((a->ae_NodePos.x - b->ae_NodePos.x) * (a->ae_NodePos.x - b->ae_NodePos.x) + (a->ae_NodePos.y - b->ae_NodePos.y) * (a->ae_NodePos.y - b->ae_NodePos.y));
}

/******************************************************************/
/*!
\brief function definition for calculating hcost of nodes
\param Node* a , taking in a pointer of type Node
\param Node* b, taking in a pointer of type Node
\return hcost of type float.
*/
/*******************************************************************/
float heuristic(Node* a, Node* b) // to calculate heuristic
{
	return distance(a, b);
}



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
std::vector<Node*> pathfind(float x, float y, float x1, float y1)
{

	std::vector<Node*> shortest;
	// NOTE TO SELF: SRC IS START, DESC IS END
	Node* nodesrc = &nodes[(-(int)y) * path_width + (int)x];
	Node* nodedesc = &nodes[(-(int)y1) * path_width + (int)x1];

	// assign enemy GameObjInst pos xy to node Pos xy
	nodesrc->ae_NodePos.x = x;
	nodesrc->ae_NodePos.y = y;
	nodesrc->b_Obstacle = false;

	// assign player GameObjInst pos xy to node Pos xy
	nodedesc->ae_NodePos.x = x1;
	nodedesc->ae_NodePos.y = y1;
	nodedesc->b_Obstacle = false;


	for (int x2 = 0; x2 < path_width; x2++)
	{
		for (int y2 = 0; y2 < path_height; y2++)
		{
			nodes[y2 * path_width + x2].b_Closed = false;
			nodes[y2 * path_width + x2].f_hcost = INFINITY;
			nodes[y2 * path_width + x2].f_fcost = INFINITY;
			nodes[y2 * path_width + x2].parent = nullptr;	// set nodes to have no parents at the start
		}

	}

	Node* current = nodesrc; // create Node ptr current and set nodesrc 
	nodesrc->f_fcost = 0.0f; // init f_fcost 
	nodesrc->f_hcost = heuristic(nodesrc, nodedesc); // set h_cost of current using heuricstic func

	std::list<Node*> NodesNotTested; //list to store nodes that needs to be tested
	NodesNotTested.push_back(current);//source node does not need to be tested as it is the starting node

	while (!NodesNotTested.empty() && current != nodedesc)
	{
		// lamda function sorting untested nodes by hcost, so lowest goes first
		NodesNotTested.sort([](const Node* lhs, const Node* rhs) { return lhs->f_hcost < rhs->f_hcost; });


		// Front of NodesNotTested is potentially the lowest hcost node.
		// Or list may also contain nodes that have been closed, so this while loop is to ditch these..
		while (!NodesNotTested.empty() && NodesNotTested.front()->b_Closed)
			NodesNotTested.pop_front();

		//abort because there are no valid nodes left to test
		if (NodesNotTested.empty())
			break;

		current = NodesNotTested.front();
		current->b_Closed = true; // We only explore a node once


		//check each of this nodes neighbour
		for (Node* NeighbourNode : current->v_Neighbours)
		{

			// if the neighbour is not visited and is 
			// not an obstacle, add it to ist
			if (!NeighbourNode->b_Closed && NeighbourNode->b_Obstacle == false)
				NodesNotTested.push_back(NeighbourNode);

			// Calculate the neighbours potential lowest parent distance
			float f_possiblelowest = current->f_fcost + distance(current, NeighbourNode);

			// If choosing to path through this node is a lower distance than what 
			// the neighbour currently has set, update the neighbour to use this node
			// as the path sourceshortestt its distance scores as necessary
			if (f_possiblelowest < NeighbourNode->f_fcost)
			{
				NeighbourNode->parent = current;
				NeighbourNode->f_fcost = f_possiblelowest;

				// The best path length to the neighbour being tested has changed, so
				// update the neighbour's score. The heuristic is used to globally bias
				// the path algorithm, so it knows if its getting better or worse. At some
				// point the algo will realise this path is worse and abandon it, and then go
				// and search along the next best path.
				NeighbourNode->f_hcost = NeighbourNode->f_fcost + heuristic(NeighbourNode, nodedesc);
			}
		}
	}


	Node* p = nodedesc; // node pointer points to destination node

	shortest.clear(); // need to clear as shortest is global.

	while (p->parent != nullptr) // p-> parent not equal to nullptr
	{
		//adding node to vector shortest
		p = p->parent;

		// move up parent all the way till source
		shortest.push_back(p);

	}
	// reverse vector so it goes from source to destination instead of destination to source
	std::reverse(shortest.begin(), shortest.end());

	return shortest;
}
	


/******************************************************************/
/*!
\brief function definition deleting nodes
*/
/*******************************************************************/
void deletenodes()
{
	
	delete[] nodes;
}


/******************************************************************/
/*!
\brief function definition for mobsKilled
*/
/*******************************************************************/

void GameObjInst::mobsKilled() {

	srand(static_cast<unsigned int>(time(NULL)));
	if (rand() % 2 == 0)
	{
		AEVec2 Pos = { posCurr.x, posCurr.y };
		staticObjInst* Potion = staticObjInstCreate(TYPE_ITEMS, 1, &Pos, 0);
		Potion->TextureMap = TEXTURE_POTION;
	}

	gameObjInstDestroy(this);
}

/******************************************************************/
/*!
\brief function definition for mobsPathfind with movement implemented
\param GameObjInst target
*/
/*******************************************************************/
void GameObjInst::mobsPathFind(GameObjInst target) {
	pathtimer -= g_dt; // timer counting down 

	//bool is_at_end = false;
	if (pathtimer <= 0)
	{
		// perform pathfinding for this enemy
		path.clear();
		path = pathfind(posCurr.x, posCurr.y, target.posCurr.x, target.posCurr.y); //pathfind function
		pathtimer = pathfindtime; // set timer back to default;
		target_node = 0; // target node for enemy to find the next node
		//is_at_end = false;
	}

	// update enemy velocity based on path
	if (!path.empty())// as long as path not empty 
	{
		//Node* pNextNode = pEnemy->path[1];

		// calculate the distance between the enemy and player
		float distance = sqrtf(powf(target.posCurr.x - posCurr.x, 2) + powf(target.posCurr.y - posCurr.y, 2));

		AEVec2 target_pos;
		AEVec2Set(&target_pos, 0, 0);

		// update enemy velocity only if it is farther than the maximum distance
		if (distance > MAX_ENEMY_DISTANCE)
		{
			float dist = AEVec2Distance(&posCurr, &path[target_node]->ae_NodePos);

			// to check the enemy is at the node 
			if (dist <= RANGE_FROM_PLAYER)
			{
				//reached the node!!!!
				//find the next node
				if (target_node + 1 < path.size())
				{
					target_node++;
					//is_at_end = false;
				}

				//its the last node!
				else
				{
					//is_at_end = true;
					target_pos.x = target.posCurr.x;
					target_pos.y = target.posCurr.y;

					path.clear();
				}
			}
			else /*if(is_at_end)*/
			{
				target_pos.x = path[target_node]->ae_NodePos.x;//
				target_pos.y = path[target_node]->ae_NodePos.y;//
			}
			velCurr.x = target_pos.x - posCurr.x;//
			velCurr.y = target_pos.y - posCurr.y;//
			AEVec2Normalize(&velCurr, &velCurr);//normalise to unit vec 1
			velCurr.x *= (g_dt * NPC_SPEED); //
			velCurr.y *= (g_dt * NPC_SPEED);
		}
		else
		{
			// stop moving if already close to the player
			velCurr.x = 0;
			velCurr.y = 0;

			// set flag to indicate stopped
			stopped = true;
			path.clear();
		}
	}
}

/******************************************************************/
/*!
\brief function definition for mobsKnockback
\param GameObjInst slash 
*/
/*******************************************************************/
void GameObjInst::mobKnockback(staticObjInst slash) {
	AEVec2 slash2Mob = slash.posCurr - posCurr; //Based on distance between mob and slash
	posCurr -= slash2Mob;
}


