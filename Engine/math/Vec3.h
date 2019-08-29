#pragma once

#include "../other/pch.h"
#include "Mat4x4.h"

struct Vec3 { // 4th Component is not manipulated, just stored (modified if requested)
	union {
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
	};

	// CONSTRUCTORS

	Vec3();
	Vec3(const float _n);
	Vec3(const float _x, const float _y);
	Vec3(const float _x, const float _y, const float _z);
	Vec3(const float _x, const float _y, const float _z, const float _w);
	Vec3(const Vec3& _v);

	// ADD

	void add(const float _x, const float _y, const float _z, const float _w, const bool _useW = false);
	void add(const float _x, const float _y, const float _z);
	void add(const float _x, const float _y);
	void add(const Vec3& _v, const bool _useW = false);
	void add(const float _n, const bool _useW = false);
	void operator+=(const Vec3& _a);
	void operator+=(const float _a);
	Vec3 operator+(const Vec3& _a) const;
	Vec3 operator+(const float _a) const;
	template <typename T>
	friend Vec3 operator+(const Vec3& _a, T _b);

	// SUBSTRACT

	void sub(const float _x, const float _y, const float _z, const float _w, const bool _useW = false);
	void sub(const float _x, const float _y, const float _z);
	void sub(const float _x, const float _y);
	void sub(const Vec3& _v, const bool _useW = false);
	void sub(const float _n, const bool _useW = false);
	void operator-=(const Vec3& _a);
	void operator-=(const float _a);
	Vec3 operator-(const Vec3& _a) const;
	Vec3 operator-(const float _a) const;
	template <typename T>
	friend Vec3 operator-(const Vec3& _a, T _b);

	// MULTIPLY

	void mul(const float _x, const float _y, const float _z, const float _w, const bool _useW = false);
	void mul(const float _x, const float _y, const float _z);
	void mul(const float _x, const float _y);
	void mul(const Vec3& _v, const bool _useW = false);
	void mul(const float _n, const bool _useW = false);
	void operator*=(const Vec3& _a);
	void operator*=(const float _a);
	Vec3 operator*(const Vec3& _a) const;
	Vec3 operator*(const float _a) const;

	// DIVIDE

	void div(const float _x, const float _y, const float _z, const float _w, const bool _useW = false);
	void div(const float _x, const float _y, const float _z);
	void div(const float _x, const float _y);
	void div(const Vec3& _v, const bool _useW = false);
	void div(const float _n, const bool _useW = false);
	void operator/=(const Vec3& _a);
	void operator/=(const float _a);
	Vec3 operator/(const Vec3& _a) const;
	Vec3 operator/(const float _a) const;
	template <typename T>
	friend Vec3 operator/(const Vec3& _a, T _b);

	// MISC

	void normalize();
	void setLength(const float _l);

	Vec3 copy() const;

	float getLength() const;

	void constrain(const float _min, const float _max);

	void constrainW(const float _min, const float _max);

	void intify();

	void intifyW();

	Vec3 operator*(const Mat4x4& _m) const;

	void operator*=(const Mat4x4& _m);
	

	static Vec3 intify(const Vec3& _v);
	static Vec3 normalize(const Vec3& _v);
	static float dotProduct(const Vec3& _a, const Vec3& _b);
};

/// typedef

typedef Vec3 Point;

/// Operator Overloading

bool operator==(const Vec3& _a, const Vec3& _b);

bool operator!=(const Vec3& _a, const Vec3& _b);

/// Printing

std::ostream& operator<<(std::ostream& _os, const Vec3& _v);