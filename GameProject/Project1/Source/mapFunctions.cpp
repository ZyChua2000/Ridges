#include "main.h"
#include <fstream>

namespace utilities {
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