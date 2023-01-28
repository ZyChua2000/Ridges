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

}