#include "Matrix4x4.h"

// Matrix Struct

Matrix4x4::Matrix4x4(const bool& _isIdentityMatrix) {
	if (_isIdentityMatrix) {
		for (unsigned char i = 1; i < 17; i += 5) {
			this->m[i - 1] = 1;
		}
	}
}

Matrix4x4::Matrix4x4(const TYPE _m[16]) { std::memcpy(this->m, _m, 16 * sizeof(TYPE)); }

Matrix4x4::Matrix4x4(const Matrix4x4& _m) { Matrix4x4(_m.m); }

unsigned char Matrix4x4::getIndex(const int& _row, const int& _col) const { return _row * 4 + _col; }

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& _m) const {
	Matrix4x4 result(false);

	for (unsigned char row = 0; row < 4; row++) {
		for (unsigned char col = 0; col < 4; col++) {
			for (unsigned char k = 0; k < 4; k++) {
				result.m[getIndex(row, col)] += this->m[getIndex(row, k)] * _m.m[getIndex(k, col)];
			}
		}
	}

	return result;
}

// Matrix4x4 Functions

/// Overloading

bool operator==(const Matrix4x4& _a, const Matrix4x4& _b) {
	bool isEqual = true;

	for (unsigned char i = 0; i < 16; i++) {
		if (_a.m[i] != _b.m[i]) {
			isEqual = false;
			break;
		}
	}

	return isEqual;
}

bool operator!=(const Matrix4x4& _a, const Matrix4x4& _b) { return !operator==(_a, _b); }

/// Printing

std::ostream& operator<<(std::ostream& _os, const Matrix4x4& _m) {
	for (int i = 0; i < 16; i++) {
		_os << _m.m[i] << " ";

		if ((i + 1) % 4 == 0) _os << std::endl;
	}

	return _os;
}

// Object Functions Namespace

namespace EN {
	namespace MATRIX4X4 {
		Matrix4x4 getRotationXMatrix(const TYPE& _rotX) {
			const TYPE sinX = std::sin(_rotX);
			const TYPE cosX = std::cos(_rotX);

			TYPE m[16] = {
				1, 0,     0,     0,
				0, +cosX, +sinX, 0,
				0, -sinX, +cosX, 0,
				0, 0,     0,     1
			};

			return Matrix4x4(m);
		}

		Matrix4x4 getRotationYMatrix(const TYPE& _rotY) {
			const TYPE sinY = std::sin(_rotY);
			const TYPE cosY = std::cos(_rotY);

			TYPE m[16] = {
				cosY, 0, -sinY, 0,
				0,    1, 0,     0,
				sinY, 0, cosY,  0,
				0,    0, 0,     1
			};

			return Matrix4x4(m);
		}

		Matrix4x4 getRotationZMatrix(const TYPE& _rotZ) {
			const TYPE sinZ = std::sin(_rotZ);
			const TYPE cosZ = std::cos(_rotZ);

			TYPE m[16] = {
				+cosZ, +sinZ, 0, 0,
				-sinZ, +cosZ, 0, 0,
				0,     0,     1, 0,
				0,     0,     0, 1
			};

			return Matrix4x4(m);
		}

		Matrix4x4 getPerspectiveMatrix(const unsigned int& _width, const unsigned int& _height, const TYPE& _fov, const TYPE& _zNear, const TYPE& _zFar) {
			TYPE aspectRatio = _height / static_cast<TYPE>(_width);
			TYPE a = -std::abs(1.f / std::tan(EN::UTIL::degToRad(_fov / 2.f)));

			TYPE m[16] = {
					aspectRatio * a, 0, 0,                                    0,
					0,               a, 0,                                    0,
					0,               0, _zFar / (_zFar - _zNear),             1,
					0,               0, (-_zFar * _zNear) / (_zFar - _zNear), 0
			};

			return Matrix4x4(m);
		}
	};
};