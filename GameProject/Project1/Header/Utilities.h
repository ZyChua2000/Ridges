#pragma once
#include "GameObjs.h" // for game objects
#include "Globals.h"
#include <string>


// functions to create/destroy a game object instance
GameObjInst* gameObjInstCreate(unsigned long type, float scale,
	AEVec2* pPos, AEVec2* pVel, float dir);

void			gameObjInstDestroy(GameObjInst* pInst);

// functions to create/destroy a game object instance
staticObjInst* staticObjInstCreate(unsigned long type, float scale,
	AEVec2* pPos, float dir);

void			staticObjInstDestroy(staticObjInst* pInst);

enum {
	notActivated,
	Activated
};


struct saveData {
	int playerHealth = 0;
	AEVec2 playerPosition = { 0,0 };
	//int playerItems;

	// Array of mobs
	int mobsNum = 0;

	int chestNum = 0;

	int leverNum = 0;

	//int puzzleCompleted[4];

	//float elapsedTime;
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

	void aeDrawQuadMesh(f32 vertice1, f32 vertice2, f32 vertice3, f32 vertice4, u32 color);

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
		This function checks if the mouse click is within the circle
	\param[in] radius
		radius of the circle
	\param[in] click_x
		X coordinate of the mouse click
	\param[in] click_y
		Y coordinate of the mouse click
	\param[in] center_x
		X coordinate of the circle
	\param[in] center_y
		Y coordinate of the circle
	\return
		0 or 1
	*************************************************************************/
	int WithinCircle(float radius, float click_x, float click_y, float center_x, float center_y);


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
		This function exports the map texture onto a text file
	\param[in] MAP_CELL_HEIGHT
		Number of cells for the height
	\param[in] MAP_CELL_WIDTH
		Number of cells for the width
	\param[in] MapObjInstList
		2D array of the map object instances
	\param[in] filename
		Filename to export texture to. Default parameter is mapText.txt
	*************************************************************************/
	void exportMapTexture(int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, AEVec2* MapObjInstList, std::string filename = "mapText.txt");

	/*!***********************************************************************
	\brief
		This function exports the map binary collision onto a text file
	\param[in] MAP_CELL_HEIGHT
		Number of cells for the height
	\param[in] MAP_CELL_WIDTH
		Number of cells for the width
	\param[in] MapObjInstList
		2D array of the map binary
	\param[in] filename
		Filename to export texture to. Default parameter is mapBin.txt
	*************************************************************************/
	void exportMapBinary(int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, AEVec2* MapObjInstList, std::string filename = "mapBin.txt");

	/*!***********************************************************************
	\brief
		This function imports the map binary collision from a text file
	\param[in] MAP_CELL_HEIGHT
		Number of cells for the height
	\param[in] MAP_CELL_WIDTH
		Number of cells for the width
	\param[in] MapObjInstList
		2D array of the map binary.
	\param[in] filename
		Filename to import texture from.
	*************************************************************************/
	/*void importMapBinary(int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, int* MapObjInstList, std::string filename);*/

	void decreaseTime(float& input);

	void initBullet(AEVec2& position, AEVec2& velocity, staticObjInst Tower);

	void changeMapObj(float mouseX, float mouseY, int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, AEVec2* MapObjInstList, staticObjInst mapeditorObj);
}

