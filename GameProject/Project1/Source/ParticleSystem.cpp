/******************************************************************************/
/*!
\file		ParticleSystem.cpp
\author 	Jaasmeet Singh
\par    	email: jaasmeet.s@digipen.edu
\date   	March 01, 2023
\brief		File containing particle system

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "ParticleSystem.h"

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/

//similar to system youre currently making for GAM150


const unsigned int	PARTICLE_OBJ_NUM_MAX = 16;			//The total number of different objects (Shapes)
const unsigned int	PARTICLE_OBJ_INST_NUM_MAX = 516;	//The total number of different game object instances


const unsigned int	FLAG_INACTIVE = 0x00000000;			//Inactive flag

const float PARTICLE_GRAVITY = 0.01f;

float internalTimer = 0;

enum TYPE_PARTICLE
{
	TYPE_DEFAULT = 0
};

/******************************************************************************/
/*!
	Struct Definitions
*/
/******************************************************************************/
struct ParticleObj
{
	unsigned int type;									//particle type
	AEGfxVertexList* pMesh;								//object's mesh 
};

struct ParticleColor
{
	float red = 255, green = 255, blue = 255, alpha = 255;
};

struct ParticleObjInst
{
	ParticleObj* pObject{};								//pointer to particle obj struct
	unsigned int flag{};								//status of instance
	float scale{};										//obj scale

	AEVec2 posCurr{};									//obj current position
	AEVec2 velCurr{};									//obj current velocity
	float dirCurr{};									//obj current direction

	AEMtx33 transform{};								//obj drawing matrix

	ParticleColor color{};								//for lifetime(alpha) and tint
	float lifeTime{};									//particle instance lifetime 
};

//GETTING WARNINGS ABOUT THIS BEING UNINITIALIZED NEED FIX ^^^


/******************************************************************************/
/*!
	File globals
*/
/******************************************************************************/
// list of original objects
static ParticleObj sParticleObjList[PARTICLE_OBJ_NUM_MAX];
static unsigned int sParticleObjNum;

// list of object instances
static ParticleObjInst sParticleObjInstList[PARTICLE_OBJ_INST_NUM_MAX];
static unsigned int		sParticleObjInstNum;

// functions to create and destroy a particle object instance 
ParticleObjInst* ParticleObjInstCreate(unsigned int type, float scale, AEVec2* pPos, AEVec2* pVel, float dir, float life);
void ParticleObjInstDestroy(ParticleObjInst* pInst);


/******************************************************************************/
/*!
	"Load" function of this system
*/
/******************************************************************************/
void ParticleSystemLoad()
{
	//allocate memory for particle obj and particle obj instances
	sParticleObjNum = 0;

	ParticleObj* pObj;

	//create mesh for default particle
	pObj = sParticleObjList + sParticleObjNum++;
	pObj->type = TYPE_DEFAULT;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create object!!");

}

/******************************************************************************/
/*!
	"Init" function of this system
*/
/******************************************************************************/
void ParticleSystemInit()
{
	sParticleObjInstNum = 0;
	ParticleSystemFree();
}

/******************************************************************************/
/*!
	"Update" function of this system
*/
/******************************************************************************/
void ParticleSystemUpdate()
{
	//iterate through all particles to check
	for (size_t i = 0; i < PARTICLE_OBJ_INST_NUM_MAX; ++i)
	{
		ParticleObjInst* pInst = sParticleObjInstList + i;


		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE)) continue;

		//reduce particle's lifetime 
		pInst->lifeTime -= g_dt;

		//if particle is past its lifetime, destroy 
		if (pInst->lifeTime <= 0)
		{
			ParticleObjInstDestroy(pInst);
			continue;
		}

		if (pInst->pObject->type == TYPE_DEFAULT)
		{
			pInst->velCurr.y -= PARTICLE_GRAVITY * g_dt * 3;
		}

		//get new position of particle
		AEVec2 pos;
		AEVec2Scale(&pos, &pInst->velCurr, g_dt);
		AEVec2Add(&pInst->posCurr, &pos, &pInst->posCurr);

		//calculate matrixes for tranformation matrix
		AEMtx33 scale, rot, trans;
		pInst = sParticleObjInstList + i;

		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
		AEMtx33Rot(&rot, pInst->dirCurr);
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);

		// Concatenate the 3 matrices to form the transform matrix
		AEMtx33Concat(&pInst->transform, &rot, &scale);
		AEMtx33Concat(&pInst->transform, &trans, &pInst->transform);
	}
}

/******************************************************************************/
/*!
	"Draw" function of this system
*/
/******************************************************************************/
void ParticleSystemDraw(AEMtx33* localTransform)
{
	for (size_t i = 0; i < PARTICLE_OBJ_INST_NUM_MAX; ++i)
	{
		ParticleObjInst* pInst = sParticleObjInstList + i;
		AEMtx33 temp;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE)) continue;

		AEMtx33 scale, rot, trans;

		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
		AEMtx33Rot(&rot, pInst->dirCurr);
		AEMtx33Trans(&trans, pInst->posCurr.x * 80, pInst->posCurr.y * 80);

	
		AEMtx33Concat(localTransform, &rot, &scale);
		AEMtx33Concat(localTransform, &trans, localTransform);

		if (pInst->pObject->type == TYPE_DEFAULT)
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxTextureSet(nullptr, 0, 0);
			AEGfxSetTintColor(0.72f, 0.52f, 0.32f, 1.0f);
		}
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxSetTransform(localTransform->m);
		/*AEMtx33 test = { 80,0,960,
						0,80,-640,
						0,0,1 };
		AEGfxSetTransform(test.m);*/
		//temp.m = { 80,0,100 };
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

/******************************************************************************/
/*!
	"Free" function of this system
*/
/******************************************************************************/
void ParticleSystemFree()
{
	for (size_t i = 0; i < PARTICLE_OBJ_INST_NUM_MAX; ++i)
		ParticleObjInstDestroy(sParticleObjInstList + i);
}

/******************************************************************************/
/*!
	"Unload" function of this system
*/
/******************************************************************************/
void ParticleSystemUnload()
{
	for (unsigned int i = 0; i < sParticleObjNum; ++i)
		AEGfxMeshFree(sParticleObjList[i].pMesh);
}

/******************************************************************************/
/*!
	Requests the particle system to create a defined amount of particle instances
*/
/******************************************************************************/
void ParticleSystemRequest(unsigned int type, float scale, AEVec2* pos, AEVec2* vel, float dir, float life, unsigned int amount)
{
	while (amount--)
	{
		AEVec2 velocity{AERandFloat() * vel->x, AERandFloat() * vel->y };
		ParticleObjInstCreate(type, AERandFloat() * scale, pos, &velocity, dir, life);
	}
}

/******************************************************************************/
/*!
	Initiates and sets active a particle object instance
*/
/******************************************************************************/
ParticleObjInst* ParticleObjInstCreate(unsigned int type, float scale, AEVec2* pPos, AEVec2* pVel, float dir, float life)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	AE_ASSERT_PARM(type < sParticleObjNum);

	// loop through the object instance list to find a non-used object instance
	for (unsigned int i = 0; i < PARTICLE_OBJ_INST_NUM_MAX; i++)
	{
		ParticleObjInst* pInst = sParticleObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = sParticleObjList + type;
			pInst->flag = FLAG_ACTIVE;
			pInst->scale = scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;
			pInst->lifeTime = life;

			// return the newly created instance
			return pInst;
		}
	}
	return nullptr; //if particle creation fails
}

/******************************************************************************/
/*!
	Sets a particle object instance to inactive
*/
/******************************************************************************/
void ParticleObjInstDestroy(ParticleObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0) return;

	// zero out the flag
	pInst->flag = 0;
}