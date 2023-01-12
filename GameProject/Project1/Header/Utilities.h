#pragma once

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
}