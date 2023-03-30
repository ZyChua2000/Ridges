#include <fstream> //For printing
#include "Main.h" // For externs


namespace utilities {

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


	float getAngle(float x1, float y1, float x2, float y2) {
		return AEACos((x1 - x2) / (float)sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2))));
	}



	int WithinCircle(float radius, float click_x, float click_y, float center_x, float center_y)
	{

		if (((click_x - center_x) * (click_x - center_x) + (click_y - center_y) * (click_y - center_y)) < radius * radius)
		{
			return 1;
		}
		return 0;
	}
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

	void decreaseTime(float& input) {
		input -= g_dt;
		if (input < 0) {
			input = 0;
		}
	}

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

	void unlockGate(int gateNum, AEVec2* MapObjInstList, int* binaryMap, AEVec2 Gates[], int MAP_CELL_HEIGHT) {
		for (int i = static_cast<int>(Gates[gateNum * 2].x); i < static_cast<int>(Gates[gateNum * 2 + 1].x) + 1; i++) {
			for (int j = static_cast<int>(Gates[gateNum * 2].y); j < static_cast<int>(Gates[gateNum * 2 + 1].y) + 1; j++) {
				*(MapObjInstList + i * MAP_CELL_HEIGHT + j) = TEXTURE_FLOOR;
				*(binaryMap + i * MAP_CELL_HEIGHT + j) = 0;
			}
		}
	}

	void updatePlayerUI(staticObjInst* Health[3], staticObjInst* Key, staticObjInst* Potion, Inventory Backpack, int playerHealth) {
		switch (playerHealth)
		{
		case 0:
			Health[2]->TextureMap = TEXTURE_DEADHEART;
			break;
		case 1:
			Health[1]->TextureMap = TEXTURE_DEADHEART;
			break;
		case 2:
			Health[0]->TextureMap = TEXTURE_DEADHEART;
		}
	}

	void completeLevel(int levelCompleted, GameObjInst* Player, Inventory Backpack) {
		levelCleared[levelCompleted] = true;
		gGameStateNext = GS_WORLD;
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

	bool inRange(GameObjInst* Player, const AEVec2 min, const AEVec2 max) {
		if (Player->posCurr.x > min.x && Player->posCurr.x < max.x) {
			if (Player->posCurr.y > min.y && Player->posCurr.y < max.y) {
				return true;
			}
		}
		return false;
	}

	void loadMeshNTexture(GameObj*& Obj, AEGfxVertexList* Mesh, AEGfxTexture* Texture, int type) {
		Obj = sGameObjList + type;
		Obj ->pMesh = Mesh;
		Obj ->pTexture = Texture;
		Obj ->type = type;
	}
}
