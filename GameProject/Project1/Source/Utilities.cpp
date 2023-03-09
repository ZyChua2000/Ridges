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

	void aeDrawQuadMesh(f32 vertice1, f32 vertice2, f32 vertice3, f32 vertice4, u32 color) {

		AEGfxTriAdd(vertice1, vertice1, color, vertice3, vertice3,
			vertice2, vertice1, color, vertice4, vertice3,
			vertice1, vertice2, color, vertice3, vertice4);

		AEGfxTriAdd(vertice2, vertice1, color, vertice4, vertice3,
			vertice2, vertice2, color, vertice4, vertice4,
			vertice1, vertice2, color, vertice3, vertice4);
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

	void exportMapBinary(int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, AEVec2* MapObjInstList, std::string filename) {
		filename = "Assets/" + filename;
		std::ofstream mapOutput{ filename };
		for (int j = 0; j < MAP_CELL_HEIGHT; j++) {
			for (int i = 0; i < MAP_CELL_WIDTH; i++) {
				int x = (MapObjInstList + i * MAP_CELL_HEIGHT + j)->x;
				int y = (MapObjInstList + i * MAP_CELL_HEIGHT + j)->y;

				if ((x < 6 && y == 4) || (x < 4 && y == 3) || (x==6 && y ==2) || (x==6 && y == 3) || (x == 5 && y == 3))
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

	/*void importMapBinary(int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, int* MapObjInstList, std::string filename) {
		filename = "Assets/" + filename;
		std::ifstream mapInput{ filename };
		for (int j = 0; j < MAP_CELL_HEIGHT; j++) {
			for (int i = 0; i < MAP_CELL_WIDTH; i++) {
				mapInput >> *(MapObjInstList + j * MAP_CELL_WIDTH + i);
			}
		}
		mapInput.close();
	}*/

	void decreaseTime(float& input) {
		input -= g_dt;
		if (input < 0) {
			input = 0;
		}
	}

	void initBullet(AEVec2& position, AEVec2& velocity, staticObjInst Tower) {
		switch ((int)(Tower.dirCurr * 57)) {
		case 0: // facing down
			velocity = { 0, -1 };
			position.y -= 0.6f;
			break;
		case 89: // facing right
			velocity = { 1, 0 };
			position.x += 0.6f;
			break;
		case 179: // facing up
			velocity = { 0, 1 };
			position.y += 0.6f;
			break;
		case -89: // facing left
			velocity = { -1, 0 };
			position.x -= 0.6f;
			break;
		default:
			break;
		}
	}

	void changeMapObj(float mouseX, float mouseY, int MAP_CELL_HEIGHT, int MAP_CELL_WIDTH, AEVec2* MapObjInstList, staticObjInst mapeditorObj) {
		for (int j = 0; j < MAP_CELL_HEIGHT; j++) {
			for (int i = 0; i < MAP_CELL_WIDTH; i++) {
				if (mouseX>= i &&
					mouseX <= i + 1 &&
					-mouseY >= j &&
					-mouseY  <= j + 1
					&& AEInputCheckCurr(AEVK_LBUTTON)) {
					*(MapObjInstList + i * MAP_CELL_HEIGHT + j) = mapeditorObj.TextureMap;
				}
			}
		}
	}

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

	void unloadObjs(AEVec2* Position) {
		delete[] Position;
	}
}
