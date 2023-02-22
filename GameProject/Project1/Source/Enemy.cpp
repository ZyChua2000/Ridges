#include "AEEngine.h"
#include "Enemy.h"


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

Node *nodes = nullptr; // Pointer to hold an array of nodes
Node *nodesrc = nullptr;
Node* nodedesc = nullptr;

void NodeInit(int** grid, Node node)
{
	//init size 
	int width = sizeof(grid) / sizeof(grid[0]);
	int height = sizeof(grid) / sizeof(width);


	nodes = new Node[sizeof(grid)]; // allocate mem for *nodes to point to array holding type Nodes

	for (int x = 0; x < grid[width][0]; x++)// nested for loop to init struct members for Node
	{
		for (int y = 0; y < grid[0][height]; y++)
		{
			nodes[y * grid[width][0] + x].ae_NodePos.x = x; //init pos x for node
			nodes[y * grid[width][0] + x].ae_NodePos.y = y; //init pos y for node
			nodes[y * grid[width][0] + x].parent = nullptr; //init parent to point to nothing
			nodes[y * grid[width][0] + x].b_Closed = false; //init closed node to false

			if (grid[x][y] == 1) // to check binary grid for obstacle
			{
				nodes[y * grid[width][0] + x].b_Obstacle = false; //if grid[x][y] == 1, it is not obstacle and set to false
			}
			else
			{
				nodes[y * grid[width][0] + x].b_Obstacle = true; // else, everything is an obstacle

			}
		}
		for (int x = 0; x < grid[width][0]; x++) // 2nd nested to to connect all the nodes together
		{
			for (int y = 0; y < grid[0][height]; y++)
			{
				if (y > 0)//checking to prevent adding nodes that dont exist
					nodes[y * width + x].v_Neighbours.push_back(&nodes[(y - 1) * width + (x + 0)]); // connecting with node above current
				if (y < height - 1)//checking to prevent adding nodes that dont exist
					nodes[y * width + x].v_Neighbours.push_back(&nodes[(y + 1) * width + (x + 0)]); // connecting with node below current
				if (x > 0)//checking to prevent adding nodes that dont exist
					nodes[y * width + x].v_Neighbours.push_back(&nodes[(y + 0) * width + (x - 1)]); // connecting with right node 
				if (x < width - 1)//checking to prevent adding nodes that dont exist
					nodes[y * width + x].v_Neighbours.push_back(&nodes[(y + 0) * width + (x + 1)]); // connecting with left node

				//  connect diagonally
				/*if (y>0 && x>0)
					nodes[y*width + x].v_Neighbours.push_back(&nodes[(y - 1) * width + (x - 1)]);
				if (y<height-1 && x>0)
					nodes[y*width + x].v_Neighbours.push_back(&nodes[(y + 1) * width + (x - 1)]);
				if (y>0 && x<width-1)
					nodes[y*width + x].v_Neighbours.push_back(&nodes[(y - 1) * width + (x + 1)]);
				if (y<height - 1 && x<width-1)
					nodes[y*width + x].v_Neighbours.push_back(&nodes[(y + 1) * width + (x + 1)]);
				*/
			}
		}


	}
}



//

