#pragma once

#include "../pch.h"
#include "Matrix4x4.h"

struct Vector3D { // 4th Component is not manipulated, just stored (modified if requested)
	union {
		struct { double x, y, z, w; };
		struct { double r, g, b, a; };
	};

	// CONSTRUCTORS

	Vector3D() : x(0), y(0), z(0), w(0) {}
	Vector3D(const double& _n) : x(_n), y(_n), z(_n), w(_n) {}
	Vector3D(const double& _x, const double& _y) : x(_x), y(_y), z(0), w(0) {}
	Vector3D(const double& _x, const double& _y, const double& _z) : x(_x), y(_y), z(_z), w(0) {}
	Vector3D(const double& _x, const double& _y, const double& _z, const double& _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vector3D(const Vector3D& _v) : x(_v.x), y(_v.y), z(_v.z), w(_v.w) {  }

	// ADD

	void add(const double& _x, const double& _y, const double& _z, const double& _w, const bool& _useW = false);
	void add(const double& _x, const double& _y, const double& _z);
	void add(const double& _x, const double& _y);
	void add(const Vector3D& _v, const bool& _useW = false);
	void add(const double& _n, const bool& _useW = false);
	void operator+=(const Vector3D& _a);
	void operator+=(const double& _a);
	Vector3D operator+(const Vector3D& _a) const;
	Vector3D operator+(const double& _a) const;

	// SUBSTRACT

	void sub(const double& _x, const double& _y, const double& _z, const double& _w, const bool& _useW = false);
	void sub(const double& _x, const double& _y, const double& _z);
	void sub(const double& _x, const double& _y);
	void sub(const Vector3D& _v, const bool& _useW = false);
	void sub(const double& _n, const bool& _useW = false);
	void operator-=(const Vector3D& _a);
	void operator-=(const double& _a);
	Vector3D operator-(const Vector3D& _a) const;
	Vector3D operator-(const double& _a) const;

	// MULTIPLY

	void mul(const double& _x, const double& _y, const double& _z, const double& _w, const bool& _useW = false);
	void mul(const double& _x, const double& _y, const double& _z);
	void mul(const double& _x, const double& _y);
	void mul(const Vector3D& _v, const bool& _useW = false);
	void mul(const double& _n, const bool& _useW = false);
	void operator*=(const Vector3D& _a);
	void operator*=(const double& _a);
	Vector3D operator*(const Vector3D& _a) const;
	Vector3D operator*(const double& _a) const;

	// DIVIDE

	void div(const double& _x, const double& _y, const double& _z, const double& _w, const bool& _useW = false);
	void div(const double& _x, const double& _y, const double& _z);
	void div(const double& _x, const double& _y);
	void div(const Vector3D& _v, const bool& _useW = false);
	void div(const double& _n, const bool& _useW = false);
	void operator/=(const Vector3D& _a);
	void operator/=(const double& _a);
	Vector3D operator/(const Vector3D& _a) const;
	Vector3D operator/(const double& _a) const;

	// MISC

	void normalize();
	void setLength(const double& _l);

	Vector3D copy() const;

	double getLength() const;

	void constrain(const double& _min, const double& _max);

	void constrainW(const double& _min, const double& _max);

	void intify();

	void intifyW();

	Vector3D operator*(const Matrix4x4 & _m) const;

	void operator*=(const Matrix4x4 & _m);
};

// Engine Namespace

namespace EN {
	namespace VECTOR3D {
		Vector3D normalize(const Vector3D& _v);

		Vector3D intify(const Vector3D& _v);

		double dotProduct(const Vector3D& _a, const Vector3D& _b);
	};
};

/// Typedef

typedef Vector3D Point;
typedef Vector3D Color;
typedef Vector3D Vector4D;

/// Operator Overloading

bool operator==(const Vector3D& _a, const Vector3D& _b);

bool operator!=(const Vector3D& _a, const Vector3D& _b);

/// Printing

std::ostream& operator<<(std::ostream& _os, const Vector3D& _v);