#pragma once

#include "../pch.h"

#include "../other/Util.h"

struct Matrix4x4 {
	TYPE m[16] = { 0 }; // r * c

	Matrix4x4(const bool& _isIdentityMatrix = false);

	Matrix4x4(const TYPE _m[16]);

	Matrix4x4(const Matrix4x4& _m);

	unsigned char getIndex(const int& _row, const int& _col) const;

	Matrix4x4 operator*(const Matrix4x4& _m) const;
};

// Matrix4x4 Functions

/// Overloading

bool operator==(const Matrix4x4& _a, const Matrix4x4& _b);

bool operator!=(const Matrix4x4& _a, const Matrix4x4& _b);

/// Printing

std::ostream& operator<<(std::ostream& _os, const Matrix4x4& _m);

// Object Functions Namespace

namespace EN {
	namespace MATRIX4X4 {
		Matrix4x4 getRotationXMatrix(const TYPE& _rotX);
		Matrix4x4 getRotationYMatrix(const TYPE& _rotY);
		Matrix4x4 getRotationZMatrix(const TYPE& _rotZ);
		Matrix4x4 getPerspectiveMatrix(const unsigned int& _width, const unsigned int& _height, const TYPE& _fov, const TYPE& _zNear, const TYPE& _zFar);
	};
};