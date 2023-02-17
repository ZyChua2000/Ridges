#pragma once
#include "AEEngine.h"
struct GameObj
{
	unsigned long	type;		// object type
	AEGfxVertexList* pMesh;		// This will hold the triangles which will form the shape of the object
	AEGfxTexture* pTexture;
	bool refMesh;
	bool refTexture;
};

struct staticObjInst
{
	GameObj* pObject;	// pointer to the 'original' shape
	unsigned long		flag;		// bit flag or-ed together
	float				scale;		// scaling value of the object instance
	AEVec2				posCurr;	// object current position
	AEMtx33				transform;	// object transformation matrix: Each frame, 
	AEVec2				TextureMap;
	float				dirCurr;
	float				timetracker;
	float				Alpha;
};

struct AABB
{
	AEVec2 min;
	AEVec2 max;
};

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
	AEVec2				TextureMap;
	// calculate the object instance's transformation matrix and save it here
};
enum TYPE
{
	// list of game object types
	TYPE_CHARACTER = 0,
	TYPE_NPCS,
	TYPE_ITEMS,
	TYPE_MAP,
	TYPE_EFFECTS,
	TYPE_SLASH,
	TYPE_REF,
	TYPE_HEALTH,

	TYPE_NUM
};