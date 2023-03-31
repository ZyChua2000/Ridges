
/******************************************************************************/
/*!
\file		AEVecOverloads.cpp
\author 	Chua Zheng Yang
\par    	email: c.zhengyang\@digipen.edu
\date   	March 06, 2023
\brief		This source file contains operator overloads for AEVec2 class

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "AEVecOverloads.h"

/*!***********************************************************************
	\brief
		This function adds 2 vectors together
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand
	\return
		Resultant vector of 2 vectors added together
*************************************************************************/
AEVec2 operator+ (AEVec2 root, AEVec2 branch) {
	return AEVec2{ root.x + branch.x, root.y + branch.y };
}

/*!***********************************************************************
	\brief
		This function deducts one vector from another
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand (subtractor)
	\return
		Resultant vector of the subtraction
*************************************************************************/
AEVec2 operator- (AEVec2 root, AEVec2 branch) {
	return AEVec2{ root.x - branch.x, root.y - branch.y };
}

/*!***********************************************************************
	\brief
		This function adds 2 vectors together and assigns the value to
		the LHS operand
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand
	\return
		Resultant vector of 2 vectors added together
*************************************************************************/
AEVec2 operator+= (AEVec2& root, AEVec2 branch) {
	root = root + branch;
	return root;
}

/*!***********************************************************************
	\brief
		This function deducts one vector from another and assigns the
		value to the LHS operand
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand (subtractor)
	\return
		Resultant vector of the subtraction
*************************************************************************/
AEVec2 operator-= (AEVec2& root, AEVec2 branch) {
	root = root - branch;
	return root;
}

/*!***********************************************************************
	\brief
		This function divides one vector from another
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand (divisor)
	\return
		Resultant vector of the divison
*************************************************************************/
AEVec2 operator/ (AEVec2 root, AEVec2 branch) {
	return AEVec2{ root.x / branch.x, root.y / branch.y };
}

/*!***********************************************************************
	\brief
		This function multiplies 2 vectors together
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand
	\return
		Resultant vector of the multiplication
*************************************************************************/
AEVec2 operator* (AEVec2 root, AEVec2 branch) {
	return AEVec2{ root.x * branch.x, root.y * branch.y };
}

/*!***********************************************************************
	\brief
		This function divides one vector from another and assigns the
		value to the LHS operand
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand (divisor)
	\return
		Resultant vector of the subtraction
*************************************************************************/
AEVec2 operator/= (AEVec2& root, AEVec2 branch) {
	root = root / branch;
	return root;
}

/*!***********************************************************************
	\brief
		This function multiplies 2 vectors and assigns the
		value to the LHS operand
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand
	\return
		Resultant vector of the multiplication
*************************************************************************/
AEVec2 operator*= (AEVec2& root, AEVec2 branch) {
	root = root * branch;
	return root;
}

/*!***********************************************************************
	\brief
		This function checks if 2 vectors are identical
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand
	\return
		true if identical, false if not
*************************************************************************/
bool operator== (AEVec2& root, AEVec2& branch) {
	if (root.x == branch.x && root.y == branch.y) {
		return true;
	}
	else {
		return false;
	}
}

/*!***********************************************************************
	\brief
		This function checks if 2 vectors are non identical
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand
	\return
		true if non identical, false if not
*************************************************************************/
bool operator!= (AEVec2& root, AEVec2& branch) {
	if (root.x != branch.x || root.y != branch.y) {
		return false;
	}
	else {
		return true;
	}
}

/*!***********************************************************************
	\brief
		This function adds a float and a vector
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand
	\return
		Resultant vector of adding float to both values of vector
*************************************************************************/
AEVec2 operator+ (AEVec2 root, float branch) {
	return AEVec2{ root.x + branch, root.y + branch };
}

/*!***********************************************************************
	\brief
		This function subtracts a float from a vector
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand
	\return
		Resultant vector of subtracting float from both values of vector
*************************************************************************/
AEVec2 operator- (AEVec2 root, float branch) {
	return AEVec2{ root.x - branch, root.y - branch };
}

/*!***********************************************************************
	\brief
		This function adds a float and a vector and assigns the value
		to the LHS operand
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand
	\return
		Resultant vector of adding float to both values of vector
*************************************************************************/
AEVec2 operator+= (AEVec2& root, float branch) {
	root = root + branch;
	return root;
}

/*!***********************************************************************
	\brief
		This function subtracts a float from a vector and assigns
		the value to the LHS operand
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand
	\return
		Resultant vector of subtracting float from both values of vector
*************************************************************************/
AEVec2 operator-= (AEVec2& root, float branch) {
	root = root - branch;
	return root;
}

/*!***********************************************************************
	\brief
		This function divides one vector by a float
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand (divisor)
	\return
		Resultant vector of the divison
*************************************************************************/
AEVec2 operator/ (AEVec2 root, float branch) {
	return AEVec2{ root.x / branch, root.y / branch };
}

/*!***********************************************************************
	\brief
		This function multiplies one vector with a float
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand (divisor)
	\return
		Resultant vector of the multiplciation
*************************************************************************/
AEVec2 operator* (AEVec2 root, float branch) {
	return AEVec2{ root.x * branch, root.y * branch };
}

/*!***********************************************************************
	\brief
		This function divides one vector by a float and assigns the value
		to the LHS operand
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand (divisor)
	\return
		Resultant vector of the divison
*************************************************************************/
AEVec2 operator/= (AEVec2& root, float branch) {
	root = root / branch;
	return root;
}

/*!***********************************************************************
	\brief
		This function multiplies one vector with a float and assigns the
		value to the LHS operand
	\param[in] root
		LHS operand
	\param[in] branch
		RHS operand (divisor)
	\return
		Resultant vector of the multiplciation
*************************************************************************/
AEVec2 operator*= (AEVec2& root, float branch) {
	root = root * branch;
	return root;
}


