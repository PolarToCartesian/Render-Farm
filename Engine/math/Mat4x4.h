#pragma once

#include "../other/pch.h"

#include "../other/Defines.h"
#include "../math/Funcs.h"

struct Mat4x4 {
	float m[16] = { 0 }; // r * c

	Mat4x4(const bool _isIdentityMatrix = false);

	Mat4x4(const float _m[16]);

	Mat4x4(const Mat4x4& _m);

	uint8_t getIndex(const uint8_t _row, const uint8_t _col) const;

	Mat4x4 operator*(const Mat4x4& _m) const;
	float  operator()(const uint8_t _i) const;
	float  operator()(const uint8_t _x, const uint8_t _y) const;

	static Mat4x4 getRotationXMatrix(const float _rotX);
	static Mat4x4 getRotationYMatrix(const float _rotY);
	static Mat4x4 getRotationZMatrix(const float _rotZ);
	static Mat4x4 getRotationMatrix(const float _rotX, const float _rotY, const float _rotZ);
	static Mat4x4 getPerspectiveMatrix(const unsigned int _width, const unsigned int _height, const float _fov, const float _zNear, const float _zFar);
};

// Mat4x4 Functions

/// Overloading

bool operator==(const Mat4x4& _a, const Mat4x4& _b);

bool operator!=(const Mat4x4& _a, const Mat4x4& _b);

/// Printing

std::ostream& operator<<(std::ostream& _os, const Mat4x4& _m);