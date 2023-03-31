/******************************************************************************/
/*!
\file		Utilities.hpp
\author
\par    	email:
\date   	February 02, 2023
\brief		This source file contains the function declaration of different functions
			that will be called and use to run the game

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "GameObjs.h" // for game objects
#include "Globals.h"  
#include <string>


enum {
	notActivated,
	Activated
};

//Defining of struct to create the boss health bar
struct bosshp
{
	int maxhp;
	int* currenthp;
	f32 width;
	f32 height;
	int damagetaken;
};

//Defining of struct for saving data
struct saveData {
	int playerHealth = 0;
	AEVec2 playerPosition = { 0,0 };

	// Array of mobs
	int mobsNum = 0;

	int chestNum = 0;

	int leverNum = 0;
};

namespace utilities {

	/*!***********************************************************************
	\brief
		This function checks if a rectangular button is clicked with left mouse
		for rectangles aligned to center.
	\param[in] rect_x
		X coordinate of the rectangle
	\param[in] rect_y
		Y coordinate of the rectangle
	\param[in] rect_width
		Width length of the rectangle
	\param[in] rect_height
		Height length of the rectangle
	\return
		If mouse clicked inside button, returns 1, if not returns 0
	*************************************************************************/
	int rectbuttonClicked_AlignCtr(float rect_x, float rect_y, float rect_width, float rect_height);


	/*!***********************************************************************
	\brief
		This function checks if a rectangular button is clicked with left mouse
		for rectangles aligned to top left corner.
	\param[in] rect_x
		X coordinate of the rectangle
	\param[in] rect_y
		Y coordinate of the rectangle
	\param[in] rect_width
		Width length of the rectangle
	\param[in] rect_height
		Height length of the rectangle
	\return
		If mouse clicked inside button, returns 1, if not returns 0
	*************************************************************************/
	int rectbuttonClicked_AlignCorner(float rect_x, float rect_y, float rect_width, float rect_height);


	/*!***********************************************************************
	\brief
		This function checks if a rectangular button is clicked with left mouse
		for rectangles aligned to top left corner.
	\param[in] x1
		X coordinate of the base object
	\param[in] y1
		Y coordinate of the base object
	\param[in] x2
		X coordinate of the reference object
	\param[in] y2
		Y coordinate of the reference object
	\return
		Radian Angle relative to X axis and the base object
	*************************************************************************/
	float getAngle(float x1, float y1, float x2, float y2);


	/*!***********************************************************************
	\brief
		This function checks if the object is within the viewport
	\param[in] Pos
		Vector position of the object
	\param[in] camX
		Camera X coordinate
	\param[in] camY
		Camera Y coordinate
	\return
		true or false
	*************************************************************************/
	bool checkWithinCam(AEVec2 Pos, f32 camX, f32 camY);


	/*!***********************************************************************
	\brief
		This function decrease the input value using delta time
	\param[in] input
		The value that will be decrease from
	*************************************************************************/
	void decreaseTime(float& input);

	/*!***********************************************************************
	\brief
		This function snaps the position of the camera position to the player position
		within certain bounds
	\param[in] playerpos
		A vector that contain the current player position
	\param[in] camX
		Camera X coordinate
	\param[in] camY
		Camera Y coordinate
	\param[in] MAP_CELL_WIDTH
		Total number of cell widths
	\param[in] MAP_CELL_HEIGHT
		Total number of cell heights
	*************************************************************************/
	void snapCamPos(AEVec2 playerPos, float& camX, float& camY, int MAP_CELL_WIDTH, int MAP_CELL_HEIGHT);


	/*!***********************************************************************
	\brief
		This function unlock the gate when player interact with a lever
	\param[in] gateNum
		The gate number of which that will be open
	\param[in] MapObjInstList
		2D array of each map tile object
	\param[in] BinaryMap
		2D array of binary collision mapping
	\param[in] Gate[]
		The positioning of the gate stored in vector
	\param[in] MAP_CELL_HEIGHT
		Total number of cell height
	*************************************************************************/
	void unlockGate(int gateNum, AEVec2* MapObjInstList, int* binaryMap, AEVec2 Gates[], int MAP_CELL_HEIGHT);

	/*!***********************************************************************
	\brief
		This function print the user interface at the top of the viewport
	\param[in] Health
		The number of health texture that will be printed
	\param[in] NumObj
		The number of item texture that will be printed
	\param[in] Backpack
		The number of item that is currently stored
	\param[in] playerHealth
		Current number of player health
	\param[in] camX
		Camera X coordinate
	\param[in] camY
		Camera Y coordinate
	*************************************************************************/
	void updatePlayerUI(staticObjInst* Health[3], staticObjInst* MenuObj[3], staticObjInst* NumObj[3], const Inventory& Backpack, const int& playerHealth, const float& camX, const float& camY);

	/*!***********************************************************************
	\brief
		This function record the level that the player had completed, send the player
		back to the main world while saving the numbers of items collected as well as
		the player health
	\param[in] levelCompleted
		The id number of which the level had been clear
	\param[in] Player
		Pointer to the "Player" game object instance
	\param[in] Backpack
		The number of item that is currently stored
	*************************************************************************/
	void completeLevel(int levelCompleted, GameObjInst* Player, Inventory& Backpack);

	/*!***********************************************************************
	\brief
		This function checks if the player is within the range of the door to
		transit to another level
	\param[in] Player
		Pointer to the "Player" game object instance
	\param[in] min
		A vector containing the minimum coordinate of the door location
	\param[in] max
		A vector containing the maximum coordinate of the door location
	\return
		true or false
	*************************************************************************/
	bool inRange(GameObjInst* Player, const AEVec2 min, const AEVec2 max);

	/*!***********************************************************************
	\brief
		This function load the mesh and texture into the game object
	\param[in] Obj
		Pointer to the "Obj" game object instance
	\param[in] Mesh
		Pointer to the mesh object
	\param[in] Texture
		Pointer to the texture object
	\param[in] type
		Type of the object
	*************************************************************************/
	void loadMeshNTexture(GameObj*& Obj, AEGfxVertexList* Mesh, AEGfxTexture* Texture, int type);

	/*!***********************************************************************
	\brief
		This function calculates a transformation matrix for the boss's health bar
		print the health bar of the boss
	\param[in] boss
		Accessing the boss details
	\param[in] hpbartransform
		Calculation of a matrix for boss health bar
	*************************************************************************/
	void bossBarTransMatrix(bosshp& boss, AEMtx33& hpbartransform);