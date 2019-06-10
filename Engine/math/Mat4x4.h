#pragma once

#include "../other/Defines.h"
#include "../math/Funcs.h"
#include "../other/pch.h"

struct Mat4x4 {
	double m[16] = { 0 }; // r * c

	Mat4x4(const bool& _isIdentityMatrix = false);

	Mat4x4(const double _m[16]);

	Mat4x4(const Mat4x4& _m);

	unsigned char getIndex(const int& _row, const int& _col) const;

	Mat4x4 operator*(const Mat4x4& _m) const;

	static Mat4x4 getRotationXMatrix(const double& _rotX);
	static Mat4x4 getRotationYMatrix(const double& _rotY);
	static Mat4x4 getRotationZMatrix(const double& _rotZ);
	static Mat4x4 getPerspectiveMatrix(const unsigned int& _width, const unsigned int& _height, const double& _fov, const double& _zNear, const double& _zFar);
};

// Mat4x4 Functions

/// Overloading

bool operator==(const Mat4x4& _a, const Mat4x4& _b);

bool operator!=(const Mat4x4& _a, const Mat4x4& _b);

/// Printing

std::ostream& operator<<(std::ostream& _os, const Mat4x4& _m);