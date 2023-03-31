#pragma once
#include "main.h"
#include <vector>
/******************************************************************************/
/*!
\file		GameObjs.h
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	March 31, 2023
\brief		This header file contains the functions for game object related
			structs and functions

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

extern const float MAX_ENEMY_DISTANCE;							// define the maximum distance at which enemies should stop moving

extern const float RANGE_FROM_PLAYER;							// define the range for enemy and player intereaction 

struct Node;

struct tint {
	float red;
	float blue;
	float green;
};

// This struct is for GameObj, called and initalised during load for each unique game object
struct GameObj
{
	unsigned long	type;		// object type
	AEGfxVertexList* pMesh;		// This will hold the triangles which will form the shape of the object
	AEGfxTexture* pTexture;		// This holds a pointer to the texture that is being used
	bool refMesh;				// True if Mesh is referencing another object's mesh, else false by default
	bool refTexture;			// True if texture is referencing another object's texture, else false by default
	
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
	float				timetracker;// Timer value for usage
	float				timetracker2;// Tiemr value for usage
	float				Alpha;		// object's transparency value
	AABB				boundingBox;// object bouding box that encapsulates the object


	// Member functions

	/*!***********************************************************************
	\brief
		This function updates spike's opacity, meant to use for spike objects
	*************************************************************************/
	void spikeUpdate();

	/*!***********************************************************************
	\brief
		This function calculates the bounding box of the object and updates
		the member variable
	*************************************************************************/
	void calculateBB();

	/*!***********************************************************************
	\brief
		This function calculates the transformation matrix of the object
		and stores it in the member variable
	*************************************************************************/
	void calculateTransMatrix();

	/*!***********************************************************************
	\brief
		This function spawns the map editor object at where the mouse is at
		to show what the map editor is currently selecting
	\param[in] mouseX
		Mouse X position
	\param[in] mouseY
		Mouse Y position
	\param[in] camX
		Cam X position
	\param[in] camY
		Cam Y position
	*************************************************************************/
	void mapEditorObjectSpawn(float mouseX, float mouseY, float camX, float camY);

	/*!***********************************************************************
	\brief
		This function turns a chest object into a potion object
	*************************************************************************/
	void chest2Potion();

	/*!***********************************************************************
	\brief
		This function shoots a bullet from the direction the tower is facing
	*************************************************************************/
	void shootBullet();

	/*!***********************************************************************
	\brief
		This function rotates an object by 45 degrees
	*************************************************************************/
	void tilt45();

	/*!***********************************************************************
	\brief
		This function updates slash's opacity, meant for slash objects
	*************************************************************************/
	void playerSlashUpdate();
};

// This struct represents the inventory of the player
struct Inventory {
	int Potion;
	int Key;

	Inventory() {
		Potion = 0;
		Key = 0;
	}

	/*!***********************************************************************
	\brief
		This function picks up the item object and updates the inventory of
		the player
	*************************************************************************/
	void itemPickUp(staticObjInst* item);
};

// This struct is for dynamic objects, meaning game entities that will be moving
struct GameObjInst
{
	// Variables for all objects
	GameObj* pObject;					// pointer to the 'original' shape
	unsigned long		flag{};			// bit flag or-ed together
	float				scale{};		// scaling value of the object instance
	AEVec2				posCurr{};		// object current position
	AEVec2				velCurr{};		// object current velocity
	float				dirCurr{};		// object current direction
	AABB				boundingBox{};	// object bouding box that encapsulates the object
	AEMtx33				transform{};	// object transformation matrix: Each frame, 
	AEVec2				TextureMap{};	// object's coordinates for sprite in spritesheet, irrelevant if spritesheet not used
	float				timetracker{};// object's time of existence

	int					health{};		// object's health level
	int					damage{};		// object's damage parameter

	// Variables exclusively for Player
	tint				damagetint{};	// Tint for player when damaged
	float				damagebuffer{};	// Time buffer to alternate tint

	// Variables exclusively for Enemy
	bool				stopped{};		//to check if enemy stopped moving
	bool				enemy_dead{};	// flag to check if enemy is alive or not
	bool				is_hit{};		//check if enemy is hit
	float				hit_time{};		//time got hit
	float				pathfindtime{};	//buffer time for enemies
	float				pathtimer{};	//timer to count down
	int					target_node{};	// node checker for path finding
	std::vector<Node*>	path{};			// this is only for enemy or any ai/ npc that requires path

	// Variables exclusively for Boss
	enum				STATE stateFlag {};			//Flag to assign state in state machine
	enum				STATE state {};				//state for boss obj instance
	enum				INNER_STATE innerState {};	// inner state for boss obj instance
	float				timeCD{};					// timer for state machine 
	

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

	 /*!***********************************************************************
	\brief
		This function calculates the bounding box of the object
	*************************************************************************/
	void calculateBB();

	/*!***********************************************************************
	\brief
		This function updates the position according to the velocity depending
		on the type of the object
	*************************************************************************/
	void velToPos();

	/*!***********************************************************************
	\brief
		This function calculates the transformation matrix of the object
	*************************************************************************/
	void calculateTransMatrix();

	/******************************************************************************/
	/*!
		Player related Functions
	*/
	/******************************************************************************/

	/*!***********************************************************************
	\brief
		This function restores health to the game object
	\param[in] recover
		Amount of health to restore for the game object, by default 1
	*************************************************************************/
	void recoverhealth(int recover = 1);

	/*!***********************************************************************
	\brief
		This function makes the player walk, inclusive of animation, velocity
		and audio.
	\param[in] walkCD
		Buffer when player uses slash
	*************************************************************************/
	void playerWalk(float walkCD);

	/*!***********************************************************************
	\brief
		This function makes the player stand still. Resets the velocity to 0
	*************************************************************************/
	void playerStand();

	/*!***********************************************************************
	\brief
		This function makes the player drink the potion and recover health
	\param[in] menuObj
		The menu objs that reflect the number of potions left
	\param[in] backPack
		The inventory
	*************************************************************************/
	void drinkPotion(staticObjInst* menuObj[3], Inventory& backPack);

	/*!***********************************************************************
	\brief
		This function creates the slash according to the angle
	\param[in] angle
		The angle between the player and the mouse
	*************************************************************************/
	void playerSlashCreate(float angle);

	/*!***********************************************************************
	\brief
		This function creates dust particles where the player walks
	*************************************************************************/
	void dustParticles();

	/*!***********************************************************************
	\brief
		This function knocks the player back from touching the mob
	\param[in] mob
		The mob that touches the player
	*************************************************************************/
	void playerKnockback(GameObjInst mob);

	/*!***********************************************************************
	\brief
		This function makes the player tint from being damaged
	\param[in] damageCD
		The time left for invulnerability
	*************************************************************************/
	void playerDamaged(float damageCD);

	/******************************************************************************/
	/*!
		Enemy related Functions
	*/
	/******************************************************************************/

	/*!***********************************************************************
	\brief
		This function kills the mob and converts it to a potion at a random
		chance
	*************************************************************************/
	void mobsKilled();

	/*!***********************************************************************
	\brief
		This function activates the pathfinding for the mob
	\param[in] target
		The object to pathfind to
	*************************************************************************/
	void mobsPathFind(GameObjInst target);

	/*!***********************************************************************
	\brief
		This function knocks the mob back upon being slashed
	\param[in] slash
		The slash object
	*************************************************************************/
	void mobKnockback(staticObjInst slash);
	
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
	TYPE_SPIKE_NONFADE,
	TYPE_MASK,
	TYPE_TOWER,
	TYPE_BULLET,
	TYPE_MINIMAP,
	TYPE_MAPCHARACTER,
	TYPE_BOSS,
	TYPE_SPLASHSCREEN,
	TYPE_BOSSCIRCLE,
	TYPE_BOSSCIRCLEATTACK,
	TYPE_PAUSE,
	TYPE_START,
	TYPE_BOSSSLASH,



	TYPE_NUM
};

/*!***********************************************************************
\brief
	This function creates a dynamic object instance
\param[in] type
	The type of object
\param[in] scale
	The scale of the object
\param[in] pPos
	Pointer to the initial position of the object
\param[in] pVel
	Pointer to the initial velocity of the object
\param[in] dir
	The initial direction of the object
\return
	Pointer to the instance
*************************************************************************/

GameObjInst* gameObjInstCreate(unsigned long type, float scale, AEVec2* pPos, AEVec2* pVel, float dir);

/*!***********************************************************************
\brief
	This function frees a dynamic object instance
\param[in] pInst
	Pointer to the object instance
*************************************************************************/
void gameObjInstDestroy(GameObjInst* pInst);

/*!***********************************************************************
\brief
	This function creates a static object instance
\param[in] type
	The type of object
\param[in] scale
	The scale of the object
\param[in] pPos
	Pointer to the initial position of the object
\param[in] dir
	The initial direction of the object
\return
	Pointer to the instance
*************************************************************************/
staticObjInst* staticObjInstCreate(unsigned long type, float scale, AEVec2* pPos, float dir);

/*!***********************************************************************
\brief
	This function frees a static object instance
\param[in] pInst
	Pointer to the object instance
*************************************************************************/
void staticObjInstDestroy(staticObjInst* pInst);