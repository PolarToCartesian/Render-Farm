#include "Mat4x4.h"

// Matrix Struct

Mat4x4::Mat4x4(const bool _isIdentityMatrix) {
	if (_isIdentityMatrix) {
		for (uint8_t i = 1; i < 17; i += 5) {
			this->m[i - 1] = 1;
		}
	}
}

Mat4x4::Mat4x4(const double _m[16]) { std::memcpy(this->m, _m, 16 * sizeof(double)); }

Mat4x4::Mat4x4(const Mat4x4& _m) { Mat4x4(_m.m); }

uint8_t Mat4x4::getIndex(const uint8_t _row, const uint8_t _col) const { return _row * 4 + _col; }

Mat4x4 Mat4x4::operator*(const Mat4x4& _m) const {
	Mat4x4 result(false);

	for (uint8_t row = 0; row < 4; row++) {
		for (uint8_t col = 0; col < 4; col++) {
			for (uint8_t k = 0; k < 4; k++) {
				result.m[getIndex(row, col)] += this->m[getIndex(row, k)] * _m.m[getIndex(k, col)];
			}
		}
	}

	return result;
}

Mat4x4 Mat4x4::getRotationXMatrix(const double _rotX) {
	const double sinX = std::sin(_rotX);
	const double cosX = std::cos(_rotX);

	double m[16] = {
		1, 0,     0,     0,
		0, +cosX, +sinX, 0,
		0, -sinX, +cosX, 0,
		0, 0,     0,     1
	};

	return Mat4x4(m);
}

Mat4x4 Mat4x4::getRotationYMatrix(const double _rotY) {
	const double sinY = std::sin(_rotY);
	const double cosY = std::cos(_rotY);

	double m[16] = {
		cosY, 0, -sinY, 0,
		0,    1, 0,     0,
		sinY, 0, cosY,  0,
		0,    0, 0,     1
	};

	return Mat4x4(m);
}

Mat4x4 Mat4x4::getRotationZMatrix(const double _rotZ) {
	const double sinZ = std::sin(_rotZ);
	const double cosZ = std::cos(_rotZ);

	double m[16] = {
		+cosZ, +sinZ, 0, 0,
		-sinZ, +cosZ, 0, 0,
		0,     0,     1, 0,
		0,     0,     0, 1
	};

	return Mat4x4(m);
}

Mat4x4 Mat4x4::getPerspectiveMatrix(const unsigned int _width, const unsigned int _height, const double _fov, const double _zNear, const double _zFar) {
	double aspectRatio = _height / static_cast<double>(_width);
	double a = -std::abs(1.f / std::tan(MATH::degToRad(_fov / 2.f)));

	double m[16] = {
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