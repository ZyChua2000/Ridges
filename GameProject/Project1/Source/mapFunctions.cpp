#include "main.h"
#include <fstream>
/******************************************************************************/
/*!
\file		mapFunctions.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	February 02, 2023
\brief		This source file contains definitions of map related functions


Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
namespace utilities {

	/*!***********************************************************************
	\brief
		This function imports the binary map from text file
	\param[in] MAP_CELL_HEIGHT
		The number of cells along the height in the level
	\param[in] MAP_CELL_WIDTH
		The number of cells along the width in the level
	\param[in] MapObjInstList
		The binary array to store the binary
	\param[in] filename
		The filename of the text file
	*************************************************************************/
	void importMapBinary(int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, int* MapObjInstList, std::string filename) {
		filename = "Assets/" + filename;
		std::ifstream mapInput{ filename };
		for (int j = 0; j < MAP_CELL_HEIGHT; j++) {
			for (int i = 0; i < MAP_CELL_WIDTH; i++) {
				mapInput >> *(MapObjInstList + i * MAP_CELL_HEIGHT + j);
			}
		}
		mapInput.close();
	}

	/*!***********************************************************************
	\brief
		This function imports the texture map from text file
	\param[in] MAP_CELL_HEIGHT
		The number of cells along the height in the level
	\param[in] MAP_CELL_WIDTH
		The number of cells along the width in the level
	\param[in] MapObjInstList
		The texture array to store the textures
	\param[in] filename
		The filename of the text file
	*************************************************************************/
	void importMapTexture(int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, AEVec2* MapObjInstList, std::string filename) {
		filename = "Assets/" + filename;
		std::ifstream mapInput{ filename };
		for (int j = 0; j < MAP_CELL_HEIGHT; j++) {
			for (int i = 0; i < MAP_CELL_WIDTH; i++) {
				mapInput >> (MapObjInstList + i * MAP_CELL_HEIGHT + j)->x;
				mapInput >> (MapObjInstList + i * MAP_CELL_HEIGHT + j)->y;
			}
		}
		mapInput.close();
	}

	/*!***********************************************************************
	\brief
		This function exports the texture map to a text file
	\param[in] MAP_CELL_HEIGHT
		The number of cells along the height in the level
	\param[in] MAP_CELL_WIDTH
		The number of cells along the width in the level
	\param[in] MapObjInstList
		The texture array that stores the texture
	\param[in] filename
		The filename of the text file
	*************************************************************************/
	void exportMapTexture(int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, AEVec2* MapObjInstList, std::string filename) {
		filename = "Assets/" + filename;
		std::ofstream mapOutput{ filename };
		for (int j = 0; j < MAP_CELL_HEIGHT; j++) {
			for (int i = 0; i < MAP_CELL_WIDTH; i++) {
				mapOutput << (MapObjInstList + i * MAP_CELL_HEIGHT + j)->x << " ";
				mapOutput << (MapObjInstList + i * MAP_CELL_HEIGHT + j)->y << " ";

				if (i == MAP_CELL_WIDTH - 1) {
					mapOutput << "\n";
				}
			}
		}
		mapOutput.close();
	}

	/*!***********************************************************************
	\brief
		This function exports the binary map to a text file
	\param[in] MAP_CELL_HEIGHT
		The number of cells along the height in the level
	\param[in] MAP_CELL_WIDTH
		The number of cells along the width in the level
	\param[in] MapObjInstList
		The binary array that stores the binary
	\param[in] filename
		The filename of the text file
	*************************************************************************/
	void exportMapBinary(int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, AEVec2* MapObjInstList, std::string filename) {
		filename = "Assets/" + filename;
		std::ofstream mapOutput{ filename };
		for (int j = 0; j < MAP_CELL_HEIGHT; j++) {
			for (int i = 0; i < MAP_CELL_WIDTH; i++) {
				int x = static_cast<int>((MapObjInstList + i * MAP_CELL_HEIGHT + j)->x);
				int y = static_cast<int>((MapObjInstList + i * MAP_CELL_HEIGHT + j)->y);

				if ((x < 6 && y == 4) || (x < 4 && y == 3) || (x == 6 && y == 2) || (x == 6 && y == 3) || (x == 5 && y == 3) || (x > 9 && y == 1))
					mapOutput << "0" << " ";
				else
					mapOutput << "1" << " ";



				if (i == MAP_CELL_WIDTH - 1) {
					mapOutput << "\n";
				}
			}
		}
		mapOutput.close();
	}

	/*!***********************************************************************
	\brief
		This function loads the object data to prepare for creation in level
	\param[out] Positions
		Dynamic array of positions of the objects being read
	\param[out] num
		The total number of objects to be created
	\param[in] filename
		The text file to read from
	*************************************************************************/
	void loadObjs(AEVec2*& Positions, int& num, std::string filename) {
		filename = "Assets/" + filename;
		std::ifstream fileInput{ filename };
		fileInput >> num;
		Positions = new AEVec2[num];

		for (int i = 0; i < num; i++) {
			fileInput >> Positions[i].x;
			fileInput >> Positions[i].y;
		}

		fileInput.close();
	}

	/*!***********************************************************************
	\brief
		Frees the dynamic memory of the object data loaded
	\param[in] Position
		Pointer to the dynamic memory
	*************************************************************************/
	void unloadObjs(AEVec2* Position) {
		delete[] Position;
	}

	/*!***********************************************************************
	\brief
		This function changes the tile that the map editor is selecting
	\param[in] mouseX
		The mouse X position
	\param[in] mouseY
		The mouse Y position
	\param[in] MAP_CELL_HEIGHT
		The number of cells along the height in the level
	\param[in] MAP_CELL_WIDTH
		The number of cells along the width in the level
	\param[in] MapObjInstList
		The binary array to store the binary
	\param[in] mapeditorObj
		The map editor object that displays the tile being selected
	*************************************************************************/
	void changeMapObj(float mouseX, float mouseY, int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, AEVec2* MapObjInstList, staticObjInst mapeditorObj) {
		for (int j = 0; j < MAP_CELL_HEIGHT; j++) {
			for (int i = 0; i < MAP_CELL_WIDTH; i++) {
				if (mouseX >= i &&
					mouseX <= i + 1 &&
					-mouseY >= j &&
					-mouseY <= j + 1
					&& AEInputCheckCurr(AEVK_LBUTTON)) {
					*(MapObjInstList + i * MAP_CELL_HEIGHT + j) = mapeditorObj.TextureMap;
				}
			}
		}
	}
} //end of namespace utilities

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
void staticObjInst::mapEditorObjectSpawn(float mouseX, float mouseY, float camX, float camY) {
	scale = 0.7f;
	posCurr = { mouseX + camX + 0.3f, mouseY + camY + 0.3f };
	if (AEInputCheckTriggered(AEVK_K) && TextureMap.y < TEXTURE_MAXHEIGHT / TEXTURE_CELLSIZE) {
		TextureMap.y += 1;
	}
	if (AEInputCheckTriggered(AEVK_I) && TextureMap.y > 0) {
		TextureMap.y -= 1;
	}
	if (AEInputCheckTriggered(AEVK_J) && TextureMap.x > 0) {
		TextureMap.x -= 1;
	}
	if (AEInputCheckTriggered(AEVK_L) && TextureMap.x < TEXTURE_MAXWIDTH / TEXTURE_CELLSIZE) {
		TextureMap.x += 1;
	}
}