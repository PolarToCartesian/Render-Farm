#pragma once

#include "Vector3D.h"

// Vector Object

void Vector3D::add(const TYPE& _x, const TYPE& _y, const TYPE& _z, const TYPE& _w, const bool& _useW) {
	this->x += _x; this->y += _y; this->z += _z;

	if (_useW) this->w += _w;
}

void Vector3D::add(const TYPE& _x, const TYPE& _y, const TYPE& _z) { this->x += _x; this->y += _y; this->z += _z; }
void Vector3D::add(const TYPE& _x, const TYPE& _y) { this->x += _x; this->y += _y; }
void Vector3D::add(const Vector3D& _v, const bool& _useW) { this->add(_v.x, _v.y, _v.z, _v.w, _useW); }
void Vector3D::add(const TYPE& _n, const bool& _useW) { this->add(_n, _n, _n, _n, _useW); }
void Vector3D::operator+=(const Vector3D& _a) { this->add(_a); }
void Vector3D::operator+=(const TYPE& _a) { this->add(_a); }
Vector3D Vector3D::operator+(const Vector3D& _a) const { Vector3D result = this->copy(); result.add(_a); return result; }
Vector3D Vector3D::operator+(const TYPE& _a)   const { Vector3D result = this->copy(); result.add(_a); return result; }



void Vector3D::sub(const TYPE& _x, const TYPE& _y, const TYPE& _z, const TYPE& _w, const bool& _useW) {
	this->x -= _x; this->y -= _y; this->z -= _z;

	if (_useW) this->w -= _w;
}

void Vector3D::sub(const TYPE& _x, const TYPE& _y, const TYPE& _z) { this->x -= _x; this->y -= _y; this->z -= _z; }
void Vector3D::sub(const TYPE& _x, const TYPE& _y) { this->x -= _x; this->y -= _y; }
void Vector3D::sub(const Vector3D& _v, const bool& _useW) { this->sub(_v.x, _v.y, _v.z, _v.w, _useW); }
void Vector3D::sub(const TYPE& _n, const bool& _useW) { this->sub(_n, _n, _n, _n, _useW); }
void Vector3D::operator-=(const Vector3D& _a) { this->sub(_a); }
void Vector3D::operator-=(const TYPE& _a) { this->sub(_a); }
Vector3D Vector3D::operator-(const Vector3D& _a) const { Vector3D result = this->copy(); result.sub(_a); return result; }
Vector3D Vector3D::operator-(const TYPE& _a)   const { Vector3D result = this->copy(); result.sub(_a); return result; }


void Vector3D::mul(const TYPE& _x, const TYPE& _y, const TYPE& _z, const TYPE& _w, const bool& _useW) {
	this->x *= _x; this->y *= _y; this->z *= _z;

	if (_useW) this->w *= _w;
}

void Vector3D::mul(const TYPE& _x, const TYPE& _y, const TYPE& _z) { this->x *= _x; this->y *= _y; this->z *= _z; }
void Vector3D::mul(const TYPE& _x, const TYPE& _y) { this->x *= _x; this->y *= _y; }
void Vector3D::mul(const Vector3D& _v, const bool& _useW) { this->mul(_v.x, _v.y, _v.z, _v.w, _useW); }
void Vector3D::mul(const TYPE& _n, const bool& _useW) { this->mul(_n, _n, _n, _n, _useW); }
void Vector3D::operator*=(const Vector3D& _a) { this->mul(_a); }
void Vector3D::operator*=(const TYPE& _a) { this->mul(_a); }
Vector3D Vector3D::operator*(const Vector3D& _a) const { Vector3D result = this->copy(); result.mul(_a); return result; }
Vector3D Vector3D::operator*(const TYPE& _a)   const { Vector3D result = this->copy(); result.mul(_a); return result; }



void Vector3D::div(const TYPE& _x, const TYPE& _y, const TYPE& _z, const TYPE& _w, const bool& _useW) {
	this->x /= _x; this->y /= _y; this->z /= _z;

	if (_useW) this->w /= _w;
}

void Vector3D::div(const TYPE& _x, const TYPE& _y, const TYPE& _z) { this->x /= _x; this->y /= _y; this->z /= _z; }
void Vector3D::div(const TYPE& _x, const TYPE& _y) { this->x /= _x; this->y /= _y; }
void Vector3D::div(const Vector3D& _v, const bool& _useW) { this->div(_v.x, _v.y, _v.z, _v.w, _useW); }
void Vector3D::div(const TYPE& _n, const bool& _useW) { this->div(_n, _n, _n, _n, _useW); }
void Vector3D::operator/=(const Vector3D& _a) { this->div(_a); }
void Vector3D::operator/=(const TYPE& _a) { this->div(_a); }
Vector3D Vector3D::operator/(const Vector3D& _a) const { Vector3D result = this->copy(); result.div(_a); return result; }
Vector3D Vector3D::operator/(const TYPE& _a)   const { Vector3D result = this->copy(); result.div(_a); return result; }


void Vector3D::normalize() { this->div(this->getLength()); }
void Vector3D::setLength(const TYPE& _l) { this->normalize(); this->mul(_l); }

Vector3D Vector3D::copy() const { return Vector3D(this->x, this->y, this->z, this->w); }

TYPE Vector3D::getLength() const { return std::sqrt(std::pow(this->x, 2) + std::pow(this->y, 2) + std::pow(this->z, 2)); }

void Vector3D::constrain(const TYPE& _min, const TYPE& _max) {
	if (this->x < _min) { this->x = _min; }
	else if (this->x > _max) { this->x = _max; }

	if (this->y < _min) { this->y = _min; }
	else if (this->y > _max) { this->y = _max; }

	if (this->z < _min) { this->z = _min; }
	else if (this->z > _max) { this->z = _max; }
}

void Vector3D::constrainW(const TYPE& _min, const TYPE& _max) {
	this->constrain(_min, _max);

	if (this->w < _min) { this->w = _min; }
	else if (this->w > _max) { this->w = _max; }
}

void Vector3D::intify() {
	this->x = static_cast<int>(this->x);
	this->y = static_cast<int>(this->y);
	this->z = static_cast<int>(this->z);
}

void Vector3D::intifyW() {
	this->intify();

	this->w = static_cast<int>(this->w);
}

Vector3D Vector3D::operator*(const Matrix4x4 & _m) const {
	return Vector3D(
		this->x * _m.m[_m.getIndex(0, 0)] + this->y * _m.m[_m.getIndex(1, 0)] + this->z * _m.m[_m.getIndex(2, 0)] + this->w * _m.m[_m.getIndex(3, 0)],
		this->x * _m.m[_m.getIndex(0, 1)] + this->y * _m.m[_m.getIndex(1, 1)] + this->z * _m.m[_m.getIndex(2, 1)] + this->w * _m.m[_m.getIndex(3, 1)],
		this->x * _m.m[_m.getIndex(0, 2)] + this->y * _m.m[_m.getIndex(1, 2)] + this->z * _m.m[_m.getIndex(2, 2)] + this->w * _m.m[_m.getIndex(3, 2)],
		this->x * _m.m[_m.getIndex(0, 3)] + this->y * _m.m[_m.getIndex(1, 3)] + this->z * _m.m[_m.getIndex(2, 3)] + this->w * _m.m[_m.getIndex(3, 3)]
	);
}

void Vector3D::operator*=(const Matrix4x4 & _m) {
	Vector3D r = this->operator*(_m);

	this->x = r.x;
	this->y = r.y;
	this->z = r.z;
}

// Engine Namespace

namespace EN {
	namespace VECTOR3D {
		Vector3D normalize(const Vector3D& _v) {
			Vector3D result = _v.copy();
			result.normalize();

			return result;
		}

		Vector3D intify(const Vector3D& _v) {
			Vector3D result = _v.copy();
			result.intify();

			return result;
		}

		TYPE dotProduct(const Vector3D& _a, const Vector3D& _b) {
			return _a.x* _b.x + _a.y * _b.y + _a.z * _b.z;
		}
	};
};

/// Operator Overloading

bool operator==(const Vector3D& _a, const Vector3D& _b) {
	return (_a.x == _b.x && _a.y == _b.y && _a.z == _b.z);
}

bool operator!=(const Vector3D & _a, const Vector3D & _b) {
	return !operator==(_a, _b);
}

/// Printing

std::ostream& operator<<(std::ostream& _os, const Vector3D& _v) {
	return _os << "X : " << _v.x << ", Y : " << _v.y << ", Z : " << _v.z << ", W : " << _v.w;
}