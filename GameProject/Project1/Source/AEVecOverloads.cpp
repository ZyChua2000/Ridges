#include "AEVecOverloads.h"

AEVec2 operator+ (AEVec2 root, AEVec2 branch) {
	return AEVec2{ root.x + branch.x, root.y + branch.y };
}

AEVec2 operator- (AEVec2 root, AEVec2 branch) {
	return AEVec2{ root.x - branch.x, root.y - branch.y };
}

AEVec2 operator+= (AEVec2& root, AEVec2 branch) {
	root = root + branch;
	return root;
}

AEVec2 operator-= (AEVec2& root, AEVec2 branch) {
	root = root - branch;
	return root;
}

AEVec2 operator/ (AEVec2 root, AEVec2 branch) {
	return AEVec2{ root.x / branch.x, root.y / branch.y };
}

AEVec2 operator* (AEVec2 root, AEVec2 branch) {
	return AEVec2{ root.x * branch.x, root.y * branch.y };
}

AEVec2 operator/= (AEVec2& root, AEVec2 branch) {
	root = root / branch;
	return root;
}

AEVec2 operator*= (AEVec2& root, AEVec2 branch) {
	root = root * branch;
	return root;
}

bool operator== (AEVec2& root, AEVec2& branch) {
	if (root.x == branch.x && root.y == branch.y) {
		return true;
	}
	else {
		return false;
	}
}
bool operator!= (AEVec2& root, AEVec2& branch) {
	if (root.x != branch.x || root.y != branch.y) {
		return false;
	}
	else {
		return true;
	}
}

AEVec2 operator+ (AEVec2 root, float branch) {
	return AEVec2{ root.x + branch, root.y + branch };
}


AEVec2 operator- (AEVec2 root, float branch) {
	return AEVec2{ root.x - branch, root.y - branch };
}

AEVec2 operator+= (AEVec2& root, float branch) {
	root = root + branch;
	return root;
}

AEVec2 operator-= (AEVec2& root, float branch) {
	root = root - branch;
	return root;
}

AEVec2 operator/ (AEVec2 root, float branch) {
	return AEVec2{ root.x / branch, root.y / branch };
}

AEVec2 operator* (AEVec2 root, float branch) {
	return AEVec2{ root.x * branch, root.y * branch };
}

AEVec2 operator/= (AEVec2& root, float branch) {
	root = root / branch;
	return root;
}

AEVec2 operator*= (AEVec2& root, float branch) {
	root = root * branch;
	return root;
}


