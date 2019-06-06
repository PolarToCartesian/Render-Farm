#pragma once

#include "../other/pch.h"
#include "Mat4x4.h"

struct Vec3 { // 4th Component is not manipulated, just stored (modified if requested)
	union {
		struct { double x, y, z, w; };
		struct { double r, g, b, a; };
	};

	// CONSTRUCTORS

	Vec3() : x(0), y(0), z(0), w(0) {}
	Vec3(const double& _n) : x(_n), y(_n), z(_n), w(_n) {}
	Vec3(const double& _x, const double& _y) : x(_x), y(_y), z(0), w(0) {}
	Vec3(const double& _x, const double& _y, const double& _z) : x(_x), y(_y), z(_z), w(0) {}
	Vec3(const double& _x, const double& _y, const double& _z, const double& _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vec3(const Vec3& _v) : x(_v.x), y(_v.y), z(_v.z), w(_v.w) {  }

	// ADD

	void add(const double& _x, const double& _y, const double& _z, const double& _w, const bool& _useW = false);
	void add(const double& _x, const double& _y, const double& _z);
	void add(const double& _x, const double& _y);
	void add(const Vec3& _v, const bool& _useW = false);
	void add(const double& _n, const bool& _useW = false);
	void operator+=(const Vec3& _a);
	void operator+=(const double& _a);
	Vec3 operator+(const Vec3& _a) const;
	Vec3 operator+(const double& _a) const;

	// SUBSTRACT

	void sub(const double& _x, const double& _y, const double& _z, const double& _w, const bool& _useW = false);
	void sub(const double& _x, const double& _y, const double& _z);
	void sub(const double& _x, const double& _y);
	void sub(const Vec3& _v, const bool& _useW = false);
	void sub(const double& _n, const bool& _useW = false);
	void operator-=(const Vec3& _a);
	void operator-=(const double& _a);
	Vec3 operator-(const Vec3& _a) const;
	Vec3 operator-(const double& _a) const;

	// MULTIPLY

	void mul(const double& _x, const double& _y, const double& _z, const double& _w, const bool& _useW = false);
	void mul(const double& _x, const double& _y, const double& _z);
	void mul(const double& _x, const double& _y);
	void mul(const Vec3& _v, const bool& _useW = false);
	void mul(const double& _n, const bool& _useW = false);
	void operator*=(const Vec3& _a);
	void operator*=(const double& _a);
	Vec3 operator*(const Vec3& _a) const;
	Vec3 operator*(const double& _a) const;

	// DIVIDE

	void div(const double& _x, const double& _y, const double& _z, const double& _w, const bool& _useW = false);
	void div(const double& _x, const double& _y, const double& _z);
	void div(const double& _x, const double& _y);
	void div(const Vec3& _v, const bool& _useW = false);
	void div(const double& _n, const bool& _useW = false);
	void operator/=(const Vec3& _a);
	void operator/=(const double& _a);
	Vec3 operator/(const Vec3& _a) const;
	Vec3 operator/(const double& _a) const;

	// MISC

	void normalize();
	void setLength(const double& _l);

	Vec3 copy() const;

	double getLength() const;

	void constrain(const double& _min, const double& _max);

	void constrainW(const double& _min, const double& _max);

	void intify();

	void intifyW();

	void colorify();
	void colorifyW();

	Vec3 operator*(const Mat4x4 & _m) const;

	void operator*=(const Mat4x4 & _m);
};

// Engine Namespace

namespace EN {
	namespace VECTOR3D {
		Vec3 normalize(const Vec3& _v);

		Vec3 intify(const Vec3& _v);

		double dotProduct(const Vec3& _a, const Vec3& _b);
	};
};

/// typedef

typedef Vec3 Point;
typedef Vec3 Color;
typedef Vec3 Vector4D;

/// Operator Overloading

bool operator==(const Vec3& _a, const Vec3& _b);

bool operator!=(const Vec3& _a, const Vec3& _b);

/// Printing

std::ostream& operator<<(std::ostream& _os, const Vec3& _v);
