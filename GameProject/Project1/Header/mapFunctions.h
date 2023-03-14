#pragma once
#include "main.h"

namespace utilities {
	/*!***********************************************************************
	\brief
		This function exports the map texture onto a text file
	\param[in] MAP_CELL_HEIGHT
		Number of cells for the height
	\param[in] MAP_CELL_WIDTH
		Number of cells for the width
	\param[in] MapObjInstList
		2D array of the map object instances
	\param[out] filename
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
	\param[out] filename
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
	\param[out] MapObjInstList
		2D array of the map binary.
	\param[in] filename
		Filename to import texture from.
	*************************************************************************/
	void importMapBinary(int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, int* MapObjInstList, std::string filename);

	/*!***********************************************************************
	\brief
		This function imports the texture map from a text file
	\param[in] MAP_CELL_HEIGHT
		Number of cells for the height
	\param[in] MAP_CELL_WIDTH
		Number of cells for the width
	\param[out] MapObjInstList
		2D array of the texture map
	\param[in] filename
		Filename to import texture from.
	*************************************************************************/
	void importMapTexture(int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, AEVec2* MapObjInstList, std::string filename);

	/*!***********************************************************************
	\brief
		This function changes the texture in the level to that of what the
		map editor object is currently selecting at the location the mouse
		is hovering over
	\param[in] mouseX
		The mouseX position
	\param[in] mouseY
		The mouseY position
	\param[out] MapObjInstList
		2D array of the texture map
	\param[in] mapeditorObj
		The reference object for the map editor
	*************************************************************************/
	void changeMapObj(float mouseX, float mouseY, int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, AEVec2* MapObjInstList, staticObjInst mapeditorObj);

	/*!***********************************************************************
	\brief
		This function loads the position of objects into a vector from a text
		file as well as the total number of that object into an int.
	\param[out] Positions
		The array of vectors to load the positions into
	\param[out] num
		The int to load the total number of objects into
	\param[in] filename
		The file to load the data from
	\warning
		This function uses dynamic memory allocation, unload function is required
		to prevent memory leak.
	*************************************************************************/
	void loadObjs(AEVec2*& Positions, int& num, std::string filename);

	/*!***********************************************************************
	\brief
		This function frees the memory that was used to load the positions of
		the object
	\param[in] Position
		The array of vectors that was previously loaded into
	*************************************************************************/
	void unloadObjs(AEVec2* Position);
}