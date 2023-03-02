#pragma once
#include "AEEngine.h"
#include <vector>

struct Node;

// This struct is for GameObj, called and initalised during load for each unique game object
struct GameObj
{
	unsigned long	type;		// object type
	AEGfxVertexList* pMesh;		// This will hold the triangles which will form the shape of the object
	AEGfxTexture* pTexture;		// This holds a pointer to the texture that is being used
	bool refMesh;			// True if Mesh is referencing another object's mesh, else false by default
	bool refTexture;		// True if texture is referencing another object's texture, else false by default
	
};

// This struct holds the max and min of the axis of an object for collision purpose
struct AABB
{
	AEVec2 min;
	AEVec2 max;
};

// This struct is for objects that are static, meaning they will not be moving
struct staticObjInst
{
	GameObj* pObject;	// pointer to the 'original' shape
	unsigned long		flag;		// bit flag or-ed together
	float				scale;		// scaling value of the object instance
	AEVec2				posCurr;	// object current position
	AEMtx33				transform;	// object transformation matrix: Each frame, 
	AEVec2				TextureMap; // object's coordinates for sprite in spritesheet, irrelevant if spritesheet not used
	float				dirCurr;	// object's direction(rotation value)
	float				timetracker;// object's time of existence
	float				Alpha;		// object's transparency value
	AABB				boundingBox;// object bouding box that encapsulates the object
};


// This struct is for dynamic objects, meaning game entities that will be moving
struct GameObjInst
{
	GameObj* pObject;	// pointer to the 'original' shape
	unsigned long		flag;		// bit flag or-ed together
	float				scale;		// scaling value of the object instance
	AEVec2				posCurr;	// object current position
	AEVec2				velCurr;	// object current velocity
	float				dirCurr;	// object current direction
	AABB				boundingBox;// object bouding box that encapsulates the object
	AEMtx33				transform;	// object transformation matrix: Each frame, 
	AEVec2				TextureMap; // object's coordinates for sprite in spritesheet, irrelevant if spritesheet not used
	int					health;		// object's health level
	int					damage;		// object's damage parameter
	std::vector<Node*>	path; // this is only for enemy or any ai/ npc that requires path
	float				timetracker;// object's time of existence
	bool				stopped; //to check if enemy stopped moving
	bool				enemy_dead; // flag to check if enemy is alive or not


	// Member functions

	/*!***********************************************************************
	\brief
		This function deducts health from the game object
	\param[in] damage
		Amount of health of deduct from the game object, by default 1
	*************************************************************************/
	void deducthealth(int damage = 1);


	/*!***********************************************************************
	\brief
		This function restores health to the game object
	\param[in] recover
		Amount of health to restore for the game object, by default 1
	*************************************************************************/
	void recoverhealth(int recover = 1);


	/*!***********************************************************************
	\brief
		This function calculates the distance to another dynamic game object
	\param[in] dynamicObj
		The dynamic game object that the current game object is referencing
		distance to
	\return
		The calculated distance between the 2 objects
	*************************************************************************/
	float calculateDistance(GameObjInst dynamicObj);

	/*!***********************************************************************
	\brief
		This function calculates the distance to another static game object
	\param[in] staticObj
		The static game object that the current game object is referencing
		distance to
	\return
		The calculated distance between the 2 objects
	*************************************************************************/
	float calculateDistance(staticObjInst staticObj);

	void walk();
};

// This enum is a list of the different types of game objects
enum TYPE
{
	// list of game object types
	TYPE_CHARACTER = 0,
	TYPE_NPCS,
	TYPE_ITEMS,
	TYPE_MAP,
	TYPE_SLASH,
	TYPE_REF,
	TYPE_HEALTH,
	TYPE_LEVERS,
	TYPE_ENEMY,
	TYPE_CHEST,
	TYPE_KEY,
	TYPE_SPIKE,
	TYPE_MASK,
	TYPE_TOWER,
	TYPE_BULLET,


	TYPE_NUM
};

struct Inventory {
	int Potion;
	int Key;
	int lastItem;

	Inventory(){
		Potion = 0;
		Key = 0;
		lastItem = 0;
	}
};