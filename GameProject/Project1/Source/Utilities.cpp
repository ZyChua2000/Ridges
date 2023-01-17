#include "AEEngine.h"
#include "Utilities.h"

namespace utilities {

	int rectbuttonClicked_AlignCtr(float rect_x, float rect_y, float rect_width, float rect_height) {
		s32 mouse_x, mouse_y;
		AEInputGetCursorPosition(&mouse_x, &mouse_y);
		rect_width /= 2;
		rect_height /= 2;
		if (AEInputCheckTriggered(AEVK_LBUTTON) == 1) {
			if (mouse_x >= (rect_x - rect_width) && mouse_x <= (rect_x + rect_width)) {
				if (mouse_y >= (rect_y - rect_height) && mouse_y <= (rect_y + rect_height)) {
					return 1;
				} return 0;
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
		return AEACos((x1 - x2) / sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2))));
	}
}