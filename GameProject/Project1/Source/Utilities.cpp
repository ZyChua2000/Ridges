/******************************************************************************/
/*!
\file		Utilities.cpp
\co-author  Chua Zheng Yang, Liu Chengrong, Alvin Woo Jia Hao
\par    	email: cd.zhengyang\@digipen.edu, chengrong.liu\@digipen.edu, a.woo\@digipen.edu
\date   	March 31, 2023
\brief		This source file contains the function definition of different functions
			that will be called and use to run the game

			- updatePlayerUI and bossBarTransMatrix done by Liu Chengrong (20%)
			- moveHelpScreen done by Alvin Woo Jia Hao (10%)
			- The rest done by Chua Zheng Yang (70%)

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include <fstream> //For printing
#include "Main.h" // For externs


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
	int rectbuttonClicked_AlignCtr(float rect_x, float rect_y, float rect_width, float rect_height) {
		s32 mouse_x, mouse_y;
		AEInputGetCursorPosition(&mouse_x, &mouse_y);
		rect_width /= 2;
		rect_height /= 2;
		 
			if (mouse_x >= (rect_x - rect_width) && mouse_x <= (rect_x + rect_width)) {
				if (mouse_y >= (rect_y - rect_height) && mouse_y <= (rect_y + rect_height)) {
					return 1;
				} return 0;
			} return 0;
		
	}

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
	int rectbuttonClicked_AlignCorner(float rect_x, float rect_y, float rect_width, float rect_height) {
		s32 mouse_x, mouse_y;
		AEInputGetCursorPosition(&mouse_x, &mouse_y);
		if (AEInputCheckTriggered(AEVK_LBUTTON) == 1) {
			if (mouse_x >= rect_x && mouse_x <= (mouse_x + rect_width)) {
				if (mouse_y >= rect_y && mouse_y <= (mouse_y + rect_height)) {
					return 1;
				} return 0;
			} return 0;
		} return 0;
	}

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
	float getAngle(float x1, float y1, float x2, float y2) {
		return AEACos((x1 - x2) / (float)sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2))));
	}

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
	bool checkWithinCam(AEVec2 Pos, f32 camX, f32 camY) {
		if (Pos.x - 1 > camX + CAM_CELL_WIDTH / 2 ||
			Pos.x + 1 < camX - CAM_CELL_WIDTH / 2 ||
			Pos.y - 1 > camY + CAM_CELL_HEIGHT / 2 ||
			Pos.y + 1 < camY - CAM_CELL_HEIGHT / 2) {
			return true;
		}
		else {
			return false;
		}
	}

	/*!***********************************************************************
	\brief
		This function decrease the input value using delta time
	\param[in] input
		The value that will be decrease from
	*************************************************************************/
	void decreaseTime(float& input) {
		input -= g_dt;
		if (input < 0) {
			input = 0;
		}
	}

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
	void snapCamPos(AEVec2 playerPos, float& camX, float& camY, int MAP_CELL_WIDTH, int MAP_CELL_HEIGHT) {
		if (MAP_CELL_WIDTH - CAM_CELL_WIDTH / 2 - 0.5 > playerPos.x &&
			CAM_CELL_WIDTH / 2 + 0.5 < playerPos.x) {
			camX = playerPos.x;
		}
		if (MAP_CELL_HEIGHT - CAM_CELL_HEIGHT / 2 - 0.5 > -playerPos.y &&
			CAM_CELL_HEIGHT / 2 + 0.5 < -playerPos.y) {
			camY = playerPos.y;
		}
	}

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
	void unlockGate(int gateNum, AEVec2* MapObjInstList, int* binaryMap, AEVec2 Gates[], int MAP_CELL_HEIGHT) {
		for (int i = static_cast<int>(Gates[gateNum * 2].x); i < static_cast<int>(Gates[gateNum * 2 + 1].x) + 1; i++) {
			for (int j = static_cast<int>(Gates[gateNum * 2].y); j < static_cast<int>(Gates[gateNum * 2 + 1].y) + 1; j++) {
				*(MapObjInstList + i * MAP_CELL_HEIGHT + j) = TEXTURE_FLOOR;
				*(binaryMap + i * MAP_CELL_HEIGHT + j) = 0;
			}
		}
	}

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
	void updatePlayerUI(staticObjInst* Health[3], staticObjInst* MenuObj[3], staticObjInst* NumObj[3], const Inventory& Backpack, const int& playerHealth, const float& camX, const float& camY) {

		MenuObj[0]->posCurr = { camX - 9.0f, camY + 5.0f };
		NumObj[0]->posCurr = { camX - 8.0f, camY + 5.0f };

		MenuObj[1]->posCurr = { camX - 6.0f, camY + 5.0f };
		NumObj[1]->posCurr = { camX - 5.0f, camY + 5.0f };

		//player health following viewport
		Health[0]->posCurr = { camX + 7.0f , camY + 5.0f };
		Health[1]->posCurr = { camX + 8.0f , camY + 5.0f };
		Health[2]->posCurr = { camX + 9.0f , camY + 5.0f };

		switch (playerHealth)
		{
		case 0:
			Health[0]->TextureMap = TEXTURE_DEADHEART;
			Health[1]->TextureMap = TEXTURE_DEADHEART;
			Health[2]->TextureMap = TEXTURE_DEADHEART;
			break;
		case 1:
			Health[0]->TextureMap = TEXTURE_DEADHEART;
			Health[1]->TextureMap = TEXTURE_DEADHEART;
			Health[2]->TextureMap = TEXTURE_FULLHEART;
			break;
		case 2:
			Health[0]->TextureMap = TEXTURE_DEADHEART;
			Health[1]->TextureMap = TEXTURE_FULLHEART;
			Health[2]->TextureMap = TEXTURE_FULLHEART;
			break;
		}
		NumObj[0]->TextureMap = TEXTURE_NUMBERS[Backpack.Potion];
		NumObj[1]->TextureMap = TEXTURE_NUMBERS[Backpack.Key];
	}

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
	void completeLevel(int levelCompleted, GameObjInst* Player, Inventory& Backpack) {
		levelCleared[levelCompleted] = true;
		gGameStateNext = GS_WORLD;
		Backpack.Key++;
		loadState = true;
		// save data

		std::ifstream prevFile{ "Assets/save.txt" };
		std::ofstream currFile{ "Assets/saveBuffer.txt" };

		int buffer;
		for (int i = 0; i < 3; i++) {
			prevFile >> buffer;
		}

		currFile << Player->health << std::endl;
		currFile << Backpack.Key << std::endl;
		currFile << Backpack.Potion;
	
		std::string stringBuffer;

		while (std::getline(prevFile, stringBuffer)) {
			
			currFile << stringBuffer << std::endl;
		}

		currFile.close();
		prevFile.close();

		prevFile.open("Assets/saveBuffer.txt");
		currFile.open("Assets/save.txt");

		while (std::getline(prevFile, stringBuffer)) {
			currFile << stringBuffer << std::endl;
		}
	}

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
	bool inRange(GameObjInst* Player, const AEVec2 min, const AEVec2 max) {
		if (Player->posCurr.x > min.x && Player->posCurr.x < max.x) {
			if (Player->posCurr.y > min.y && Player->posCurr.y < max.y) {
				return true;
			}
		}
		return false;
	}

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
	void loadMeshNTexture(GameObj*& Obj, AEGfxVertexList* Mesh, AEGfxTexture* Texture, int type) {
		Obj = sGameObjList + type;
		Obj ->pMesh = Mesh;
		Obj ->pTexture = Texture;
		Obj ->type = type;
	}

	/*!***********************************************************************
	\brief
		This function calculates a transformation matrix for the boss's health bar
		print the health bar of the boss
	\param[in\out] boss
		Accessing the bosshp variable details
	\param[out] hpbartransform
		Calculation of a matrix for boss health bar
	\param[out] bossHeart
		Game obj of boss heart
	\param[in] camX
		Camera X position
	\param[in] camY
		Camera Y position
	*************************************************************************/
	void bossBarTransMatrix(bosshp& boss, AEMtx33 &hpbartransform, staticObjInst& bossHeart, float camX, float camY) {
		float barLength = 12.f; // Length of bar
		float yOffset = -4.5f;// Offset of Y from Camera for UI
		float xOffset = 0.25f;// Offset of X from Camera for UI

		camX += xOffset;
		camY += yOffset;

		boss.damagetaken = boss.maxhp - *boss.currenthp;
		boss.width = static_cast<float>(SPRITE_SCALE) * barLength * *boss.currenthp / boss.maxhp;
		boss.height = static_cast<float>(SPRITE_SCALE)/2;


		// Boss Heart X = CamX - 1/2 of max boss bar
		bossHeart.posCurr = { camX - (barLength / 2 ), camY };

		//scale, rot, trans for health bar
		AEMtx33 bar_scale, bar_trans, bar_rot;
		AEMtx33Scale(&bar_scale, boss.width, boss.height);
		AEMtx33Rot(&bar_rot, 0);
		// Trans X = (camX - (Amount of damage taken * 1/2 unit length of boss hp)) * SPRITE_SCALE
		// unit length of boss hp = Width / Max HP
		AEMtx33Trans(&bar_trans, (camX - (boss.damagetaken * (barLength / boss.maxhp) / 2)) * SPRITE_SCALE, camY * SPRITE_SCALE);
		AEMtx33Concat(&hpbartransform, &bar_scale, &bar_rot);
		AEMtx33Concat(&hpbartransform, &bar_trans, &hpbartransform);
	}

	void moveHelpScreen(staticObjInst& PauseObj, int textureID) {
		if (AEInputCheckTriggered(AEVK_H) && cycle == 0) {
			cycle = 1;
		}
		if (cycle != 0 && AEInputCheckTriggered(AEVK_RIGHT)) {
			cycle++;
		}
		if (cycle == 4) {
			cycle = 0;
		}

		PauseObj.pObject->pTexture = textureList[textureID + cycle];

		if (cycle == 0) {
			if (AEInputCheckReleased(AEVK_LBUTTON)) {
				if (utilities::rectbuttonClicked_AlignCtr(800.f, 445.f, 245.f, 85.f) == 1)//width 245 height 85
				{
					pause = false;
				}

				if (utilities::rectbuttonClicked_AlignCtr(800.f, 585.f, 245.f, 85.f) == 1)//width 245 height 85
				{
					gGameStateNext = GS_MAINMENU;
				}
			}
		}

	}

	/*!***********************************************************************
	\brief
		This function makes a dialog box pop up upon trying to quit game to
		confirm the quitting of game.
	*************************************************************************/
	void quitGame() {
		int result = MessageBox(NULL, "Do you want to Exit the Game?", "Confirmation", MB_YESNO | MB_ICONQUESTION);

		if (result == IDYES)
		{
			gGameStateNext = GS_QUIT;
			return;// user clicked yes
		}
		else
		{
			return;// user clicked no
		}

	}
}
