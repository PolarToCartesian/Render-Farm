#include "Vec3.h"

// Vector Object

void Vec3::add(const double& _x, const double& _y, const double& _z, const double& _w, const bool& _useW) {
	this->x += _x; this->y += _y; this->z += _z;

	if (_useW) this->w += _w;
}

void Vec3::add(const double& _x, const double& _y, const double& _z) { this->x += _x; this->y += _y; this->z += _z; }
void Vec3::add(const double& _x, const double& _y) { this->x += _x; this->y += _y; }
void Vec3::add(const Vec3& _v, const bool& _useW) { this->add(_v.x, _v.y, _v.z, _v.w, _useW); }
void Vec3::add(const double& _n, const bool& _useW) { this->add(_n, _n, _n, _n, _useW); }
void Vec3::operator+=(const Vec3& _a) { this->add(_a); }
void Vec3::operator+=(const double& _a) { this->add(_a); }
Vec3 Vec3::operator+(const Vec3& _a) const { Vec3 result = this->copy(); result.add(_a); return result; }
Vec3 Vec3::operator+(const double& _a)   const { Vec3 result = this->copy(); result.add(_a); return result; }



void Vec3::sub(const double& _x, const double& _y, const double& _z, const double& _w, const bool& _useW) {
	this->x -= _x; this->y -= _y; this->z -= _z;

	if (_useW) this->w -= _w;
}

void Vec3::sub(const double& _x, const double& _y, const double& _z) { this->x -= _x; this->y -= _y; this->z -= _z; }
void Vec3::sub(const double& _x, const double& _y) { this->x -= _x; this->y -= _y; }
void Vec3::sub(const Vec3& _v, const bool& _useW) { this->sub(_v.x, _v.y, _v.z, _v.w, _useW); }
void Vec3::sub(const double& _n, const bool& _useW) { this->sub(_n, _n, _n, _n, _useW); }
void Vec3::operator-=(const Vec3& _a) { this->sub(_a); }
void Vec3::operator-=(const double& _a) { this->sub(_a); }
Vec3 Vec3::operator-(const Vec3& _a) const { Vec3 result = this->copy(); result.sub(_a); return result; }
Vec3 Vec3::operator-(const double& _a)   const { Vec3 result = this->copy(); result.sub(_a); return result; }


void Vec3::mul(const double& _x, const double& _y, const double& _z, const double& _w, const bool& _useW) {
	this->x *= _x; this->y *= _y; this->z *= _z;

	if (_useW) this->w *= _w;
}

void Vec3::mul(const double& _x, const double& _y, const double& _z) { this->x *= _x; this->y *= _y; this->z *= _z; }
void Vec3::mul(const double& _x, const double& _y) { this->x *= _x; this->y *= _y; }
void Vec3::mul(const Vec3& _v, const bool& _useW) { this->mul(_v.x, _v.y, _v.z, _v.w, _useW); }
void Vec3::mul(const double& _n, const bool& _useW) { this->mul(_n, _n, _n, _n, _useW); }
void Vec3::operator*=(const Vec3& _a) { this->mul(_a); }
void Vec3::operator*=(const double& _a) { this->mul(_a); }
Vec3 Vec3::operator*(const Vec3& _a) const { Vec3 result = this->copy(); result.mul(_a); return result; }
Vec3 Vec3::operator*(const double& _a)   const { Vec3 result = this->copy(); result.mul(_a); return result; }



void Vec3::div(const double& _x, const double& _y, const double& _z, const double& _w, const bool& _useW) {
	this->x /= _x; this->y /= _y; this->z /= _z;

	if (_useW) this->w /= _w;
}

void Vec3::div(const double& _x, const double& _y, const double& _z) { this->x /= _x; this->y /= _y; this->z /= _z; }
void Vec3::div(const double& _x, const double& _y) { this->x /= _x; this->y /= _y; }
void Vec3::div(const Vec3& _v, const bool& _useW) { this->div(_v.x, _v.y, _v.z, _v.w, _useW); }
void Vec3::div(const double& _n, const bool& _useW) { this->div(_n, _n, _n, _n, _useW); }
void Vec3::operator/=(const Vec3& _a) { this->div(_a); }
void Vec3::operator/=(const double& _a) { this->div(_a); }
Vec3 Vec3::operator/(const Vec3& _a) const { Vec3 result = this->copy(); result.div(_a); return result; }
Vec3 Vec3::operator/(const double& _a)   const { Vec3 result = this->copy(); result.div(_a); return result; }


void Vec3::normalize() { this->div(this->getLength()); }
void Vec3::setLength(const double& _l) { this->normalize(); this->mul(_l); }

Vec3 Vec3::copy() const { return Vec3(this->x, this->y, this->z, this->w); }

double Vec3::getLength() const { return std::sqrt(std::pow(this->x, 2) + std::pow(this->y, 2) + std::pow(this->z, 2)); }

void Vec3::constrain(const double& _min, const double& _max) {
	if (this->x < _min) { this->x = _min; }
	else if (this->x > _max) { this->x = _max; }

	if (this->y < _min) { this->y = _min; }
	else if (this->y > _max) { this->y = _max; }

	if (this->z < _min) { this->z = _min; }
	else if (this->z > _max) { this->z = _max; }
}

void Vec3::constrainW(const double& _min, const double& _max) {
	this->constrain(_min, _max);

	if (this->w < _min) { this->w = _min; }
	else if (this->w > _max) { this->w = _max; }
}

void Vec3::intify() {
	this->x = static_cast<int>(this->x);
	this->y = static_cast<int>(this->y);
	this->z = static_cast<int>(this->z);
}

void Vec3::intifyW() {
	this->intify();

	this->w = static_cast<int>(this->w);
}

void Vec3::colorify() {
	this->intify();
	this->constrain(0, 255);
}

void Vec3::colorifyW() {
	this->intifyW();
	this->constrainW(0, 255);
}

Vec3 Vec3::operator*(const Mat4x4& _m) const {
	return Vec3(
		this->x * _m.m[_m.getIndex(0, 0)] + this->y * _m.m[_m.getIndex(1, 0)] + this->z * _m.m[_m.getIndex(2, 0)] + this->w * _m.m[_m.getIndex(3, 0)],
		this->x * _m.m[_m.getIndex(0, 1)] + this->y * _m.m[_m.getIndex(1, 1)] + this->z * _m.m[_m.getIndex(2, 1)] + this->w * _m.m[_m.getIndex(3, 1)],
		this->x * _m.m[_m.getIndex(0, 2)] + this->y * _m.m[_m.getIndex(1, 2)] + this->z * _m.m[_m.getIndex(2, 2)] + this->w * _m.m[_m.getIndex(3, 2)],
		this->x * _m.m[_m.getIndex(0, 3)] + this->y * _m.m[_m.getIndex(1, 3)] + this->z * _m.m[_m.getIndex(2, 3)] + this->w * _m.m[_m.getIndex(3, 3)]
	);
}

void Vec3::operator*=(const Mat4x4 & _m) {
	Vec3 r = this->operator*(_m);

	this->x = r.x;
	this->y = r.y;
	this->z = r.z;
}

// Engine Namespace

namespace EN {
	namespace VECTOR3D {
		Vec3 normalize(const Vec3& _v) {
			Vec3 result = _v.copy();
			result.normalize();

			return result;
		}

		Vec3 intify(const Vec3& _v) {
			Vec3 result = _v.copy();
			result.intify();

			return result;
		}

		double dotProduct(const Vec3& _a, const Vec3& _b) {
			return _a.x* _b.x + _a.y * _b.y + _a.z * _b.z;
		}
	};
};

/// Operator Overloading

bool operator==(const Vec3& _a, const Vec3& _b) {
	return (_a.x == _b.x && _a.y == _b.y && _a.z == _b.z);
}

bool operator!=(const Vec3 & _a, const Vec3 & _b) {
	return !operator==(_a, _b);
}

/// Printing

std::ostream& operator<<(std::ostream& _os, const Vec3& _v) {
	return _os << "X : " << _v.x << ", Y : " << _v.y << ", Z : " << _v.z << ", W : " << _v.w;
}