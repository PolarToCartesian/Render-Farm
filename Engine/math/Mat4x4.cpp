#include "Mat4x4.h"

// Matrix Struct

Mat4x4::Mat4x4(const bool _isIdentityMatrix) {
	if (_isIdentityMatrix) {
		for (uint8_t i = 1; i < 17; i += 5)
			this->m[i - 1] = 1;
	}
}

Mat4x4::Mat4x4(const float _m[16]) { std::memcpy(this->m, _m, 16 * sizeof(float)); }

Mat4x4::Mat4x4(const Mat4x4& _m) { Mat4x4(_m.m); }

uint8_t Mat4x4::getIndex(const uint8_t _row, const uint8_t _col) const { return _row * 4 + _col; }

Mat4x4 Mat4x4::operator*(const Mat4x4& _m) const {
	Mat4x4 result(false);

	for (uint8_t row = 0; row < 4; row++) {
		const uint8_t rowIndex = row * 4;

		for (uint8_t col = 0; col < 4; col++) {
			const uint8_t cellIndex = rowIndex + col;

			uint8_t index2 = col;
			for (uint8_t k = 0; k < 4; k++) {
				result.m[cellIndex] += this->m[rowIndex + k] * _m.m[index2];
				index2 += 4;
			}
		}
	}

	return result;
}

float Mat4x4::operator()(const uint8_t i) const {
	return this->m[i];
}

float Mat4x4::operator()(const uint8_t _row, const uint8_t _col) const {
	return this->m[_row * 4 + _col];
}

Mat4x4 Mat4x4::getRotationXMatrix(const float _rotX) {
	const float sinX = std::sin(_rotX);
	const float cosX = std::cos(_rotX);

	float m[16] = {
		1, 0,     0,     0,
		0, +cosX, +sinX, 0,
		0, -sinX, +cosX, 0,
		0, 0,     0,     1
	};

	return Mat4x4(m);
}

Mat4x4 Mat4x4::getRotationYMatrix(const float _rotY) {
	const float sinY = std::sin(_rotY);
	const float cosY = std::cos(_rotY);

	float m[16] = {
		cosY, 0, -sinY, 0,
		0,    1, 0,     0,
		sinY, 0, cosY,  0,
		0,    0, 0,     1
	};

	return Mat4x4(m);
}

Mat4x4 Mat4x4::getRotationZMatrix(const float _rotZ) {
	const float sinZ = std::sin(_rotZ);
	const float cosZ = std::cos(_rotZ);

	float m[16] = {
		+cosZ, +sinZ, 0, 0,
		-sinZ, +cosZ, 0, 0,
		0,     0,     1, 0,
		0,     0,     0, 1
	};

	return Mat4x4(m);
}

Mat4x4 Mat4x4::getRotationMatrix(const float _rotX, const float _rotY, const float _rotZ) {
	return Mat4x4::getRotationXMatrix(_rotX) *
		   Mat4x4::getRotationYMatrix(_rotY) *
		   Mat4x4::getRotationZMatrix(_rotZ);
}

Mat4x4 Mat4x4::getPerspectiveMatrix(const unsigned int _width, const unsigned int _height, const float _fov, const float _zNear, const float _zFar) {
	float aspectRatio = _height / static_cast<float>(_width);
	float a = -std::abs(1.f / std::tan(MATH::degToRad(_fov / 2.f)));

	float m[16] = {
			aspectRatio * a, 0, 0,                                    0,
			0,               a, 0,                                    0,
			0,               0, _zFar / (_zFar - _zNear),             1,
			0,               0, (-_zFar * _zNear) / (_zFar - _zNear), 0
	};

	return Mat4x4(m);
}

// Mat4x4 Functions

/// Overloading

bool operator==(const Mat4x4& _a, const Mat4x4& _b) {
	bool isEqual = true;

	for (uint8_t i = 0; i < 16; i++) {
		if (_a.m[i] != _b.m[i]) {
			isEqual = false;
			break;
		}
	}

	return isEqual;
}

bool operator!=(const Mat4x4& _a, const Mat4x4& _b) { return !operator==(_a, _b); }

/// Printing

std::ostream& operator<<(std::ostream& _os, const Mat4x4& _m) {
	for (int i = 0; i < 16; i++) {
		_os << _m.m[i] << " ";

		if ((i + 1) % 4 == 0) _os << std::endl;
	}

	return _os;
}