#include "AEEngine.h"
#include "Enemy.h"
#include <iostream>
#include <list>
#include <vector>

extern Node* nodes{};

// for enemy 
// FOR FUTURE USE MAYBE ???
//void enemy_Init()
//{
//	AEVec2 pos {0,0};
//	int hp = 100;
//	int damage = 10;
//
//}
//
//void enemy_update()
//{
//
//}
//
//void enemy_draw()
//{

//Node *nodes = nullptr; // Pointer to hold an array of nodes,  need this in world.cpp
int path_width{};
int path_height{};
float dist{};

Node n_enemy{};
Node n_player{};


// func to initialise nodes 
void NodesInit(int grid[][42], int width, int height)
{
	//init size 
	/*path_width = sizeof(grid) / sizeof(grid[0]);
	path_height = sizeof(grid) / sizeof(path_width);*/

	path_width = width;
	path_height = height;

	nodes = new Node[path_width * path_height]; // allocate mem for *nodes to point to array holding type Nodes


	for (int x = 0; x < path_width; x++)// nested for loop to init struct members for Node
	{
		for (int y = 0; y < path_height; y++)
		{
			int index = y * path_width + x;
			Node* node = &nodes[index];

			nodes[index].ae_NodePos.x = x+0.5f; //init pos x for node
			nodes[index].ae_NodePos.y = -y-0.5f; //init pos y for node
			nodes[index].parent = nullptr; //init parent to point to nothing first 
			nodes[index].b_Closed = false; //init closed node to false

			// in bin map , 1 is wall, 0 is not
			if (grid[x][y] == 1) // to check binary grid for obstacle
			{
 				
 				nodes[y * path_width + x].b_Obstacle = true; //if grid[x][y] == 1, it is  obstacle and set to true
				//std::cout << y * path_width + x << std::endl;
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

			if (y > 0)//checking to prevent adding nodes that dont exist
				nodes[y * path_width + x].v_Neighbours.push_back(&nodes[(y - 1) * path_width + (x + 0)]); // connecting with node above current

			if (y < path_height - 1)//checking to prevent adding nodes that dont exist
				nodes[y * path_width + x].v_Neighbours.push_back(&nodes[(y + 1) * path_width + (x + 0)]); // connecting with node below current

			if (x > 0)//checking to prevent adding nodes that dont exist
				nodes[y * path_width + x].v_Neighbours.push_back(&nodes[(y + 0) * path_width + (x - 1)]); // connecting with right node 

			if (x < path_width - 1)//checking to prevent adding nodes that dont exist
				nodes[y * path_width + x].v_Neighbours.push_back(&nodes[(y + 0) * path_width + (x + 1)]); // connecting with left node

			//std::cout << "Size: " << nodes[y * path_width + x].v_Neighbours.size() << std::endl;
			//  connect diagonally
			//
			//if (y>0 && x>0)
			//	nodes[y*path_width + x].v_Neighbours.push_back(&nodes[(y - 1) * path_width + (x - 1)]);

			//if (y<path_height-1 && x>0)
			//	nodes[y*path_width + x].v_Neighbours.push_back(&nodes[(y + 1) * path_width + (x - 1)]);

			//if (y>0 && x<path_width-1)
			//	nodes[y*path_width + x].v_Neighbours.push_back(&nodes[(y - 1) * path_width + (x + 1)]);

			//if (y<path_height - 1 && x<width-1)
			//	nodes[y*path_width + x].v_Neighbours.push_back(&nodes[(y + 1) * path_width + (x + 1)]);
			
		}
	}

	std::cout << "Nodes Initialised" << std::endl;
}

float distance(Node* a, Node* b) // to calculate dist
{
	return sqrtf((a->ae_NodePos.x - b->ae_NodePos.x) * (a->ae_NodePos.x - b->ae_NodePos.x) + (a->ae_NodePos.y - b->ae_NodePos.y) * (a->ae_NodePos.y - b->ae_NodePos.y));
}

float heuristic(Node* a, Node* b) // to calculate heuristic
{
	return distance(a, b);
}

//float i_hcost = heuristic(nodesrc, nodedesc);

std::vector<Node*> shortest;

std::vector<Node*> pathfind(int grid[][42],float x, float y, float x1, float y1)
{
	
	// NOTE TO SELF: SRC IS START, DESC IS END
	Node* nodesrc = &nodes[(-(int)y) * path_width + (int)x];
	Node* nodedesc= &nodes[(-(int)y1) * path_width + (int)x1];

	// assign enemy GameObjInst pos xy to node Pos xy
	nodesrc->ae_NodePos.x = x;
	nodesrc->ae_NodePos.y = y;
	nodesrc->b_Obstacle = false;

	// assign player GameObjInst pos xy to node Pos xy
	nodedesc->ae_NodePos.x = x1;
	nodedesc->ae_NodePos.y = y1;
	nodedesc->b_Obstacle = false;

	//nodesrc = n_enemy;
	//nodedesc = n_player;

	for (int x2 = 0; x2 < path_width; x2++)
	{
		for (int y2 = 0; y2 < path_height; y2++)
		{
			nodes[y2 *path_width + x2].b_Closed = false;
			nodes[y2 *path_width + x2].f_hcost = INFINITY;
			nodes[y2 *path_width + x2].f_fcost = INFINITY;
			nodes[y2 *path_width + x2].parent = nullptr;	// set nodes to have no parents at the start
		}

	}

	Node* current = nodesrc; 
	nodesrc->f_fcost = 0.0f;
	nodesrc->f_hcost = heuristic(nodesrc, nodedesc);

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
		for(Node *NeighbourNode : current->v_Neighbours)
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
	if (nodedesc != nullptr) //checks if destination note its not null  
	{
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

}

void deletenodes()// free New for nodes
{
	
	delete[] nodes;
}


