#include "Vec3.h"

// Vector Object

Vec3::Vec3() : x(0), y(0), z(0), w(0) {}
Vec3::Vec3(const float _n) : x(_n), y(_n), z(_n), w(_n) {}
Vec3::Vec3(const float _x, const float _y) : x(_x), y(_y), z(0), w(0) {}
Vec3::Vec3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z), w(0) {}
Vec3::Vec3(const float _x, const float _y, const float _z, const float _w) : x(_x), y(_y), z(_z), w(_w) {}
Vec3::Vec3(const Vec3& _v) : x(_v.x), y(_v.y), z(_v.z), w(_v.w) {}

void Vec3::operator+=(const Vec3& _v) { this->x += _v.x; this->y += _v.y; this->z += _v.z; }
void Vec3::operator+=(const float _n) { this->x += _n;   this->y += _n;   this->z += _n;   }
Vec3 Vec3::operator+(const Vec3& _v) const { return Vec3( this->x + _v.x, this->y + _v.y, this->z + _v.z ); }
Vec3 Vec3::operator+(const float _n) const { return Vec3( this->x + _n,   this->y + _n,   this->z + _n   ); }

void Vec3::operator-=(const Vec3& _v) { this->x -= _v.x; this->y -= _v.y; this->z -= _v.z; }
void Vec3::operator-=(const float _n) { this->x -= _n;   this->y -= _n;   this->z -= _n; }
Vec3 Vec3::operator-(const Vec3& _v) const { return Vec3( this->x - _v.x, this->y - _v.y, this->z - _v.z); }
Vec3 Vec3::operator-(const float _n) const { return Vec3( this->x - _n,   this->y - _n,   this->z - _n  ); }

void Vec3::operator*=(const Vec3& _v) { this->x *= _v.x; this->y *= _v.y; this->z *= _v.z; }
void Vec3::operator*=(const float _n) { this->x *= _n;   this->y *= _n;   this->z *= _n; }
Vec3 Vec3::operator*(const Vec3& _v) const { return Vec3( this->x * _v.x, this->y * _v.y, this->z * _v.z); }
Vec3 Vec3::operator*(const float _n) const { return Vec3( this->x * _n,   this->y * _n,   this->z * _n  ); }

void Vec3::operator/=(const Vec3& _v) { this->x /= _v.x; this->y /= _v.y; this->z /= _v.z; }
void Vec3::operator/=(const float _n) { this->x /= _n;   this->y /= _n;   this->z /= _n; }
Vec3 Vec3::operator/(const Vec3& _v) const { return Vec3( this->x / _v.x, this->y / _v.y, this->z / _v.z); }
Vec3 Vec3::operator/(const float _n) const { return Vec3( this->x / _n,   this->y / _n,   this->z / _n  ); }

void Vec3::normalize() { this->operator/=(this->getLength()); }
void Vec3::setLength(const float _l) { this->normalize(); this->operator*=(_l); }

Vec3 Vec3::copy() const { return Vec3(this->x, this->y, this->z, this->w); }

float Vec3::getLength() const { return std::sqrt(std::pow(this->x, 2) + std::pow(this->y, 2) + std::pow(this->z, 2)); }

void Vec3::constrain(const float _min, const float _max) {
	if (this->x < _min) { this->x = _min; }
	else if (this->x > _max) { this->x = _max; }

	if (this->y < _min) { this->y = _min; }
	else if (this->y > _max) { this->y = _max; }

	if (this->z < _min) { this->z = _min; }
	else if (this->z > _max) { this->z = _max; }
}

void Vec3::constrainW(const float _min, const float _max) {
	this->constrain(_min, _max);

	if (this->w < _min) { this->w = _min; }
	else if (this->w > _max) { this->w = _max; }
}

void Vec3::intify() {
	this->x = static_cast<float>(static_cast<int>(this->x));
	this->y = static_cast<float>(static_cast<int>(this->y));
	this->z = static_cast<float>(static_cast<int>(this->z));
}

void Vec3::intifyW() {
	this->intify();

	this->w = static_cast<float>(static_cast<int>(this->w));
}

Vec3 Vec3::operator*(const Mat4x4& _m) const {
	return Vec3(
		this->x * _m.m[0] + this->y * _m.m[4] + this->z * _m.m[8]  + this->w * _m.m[12],
		this->x * _m.m[1] + this->y * _m.m[5] + this->z * _m.m[9]  + this->w * _m.m[13],
		this->x * _m.m[2] + this->y * _m.m[6] + this->z * _m.m[10] + this->w * _m.m[14],
		this->x * _m.m[3] + this->y * _m.m[7] + this->z * _m.m[11] + this->w * _m.m[15]
	);
}

void Vec3::operator*=(const Mat4x4& _m) {
	const Vec3 c(*this); // copy
	this->x = c.x * _m.m[0] + c.y * _m.m[4] + c.z * _m.m[8]  + c.w * _m.m[12];
	this->y = c.x * _m.m[1] + c.y * _m.m[5] + c.z * _m.m[9]  + c.w * _m.m[13];
	this->z = c.x * _m.m[2] + c.y * _m.m[6] + c.z * _m.m[10] + c.w * _m.m[14];
	this->w = c.x * _m.m[3] + c.y * _m.m[7] + c.z * _m.m[11] + c.w * _m.m[15];
}

Vec3 Vec3::intify(const Vec3& _v) {
	Vec3 result = _v.copy();
	result.intify();

	return result;
}

Vec3 Vec3::normalize(const Vec3& _v) {
	Vec3 result = _v.copy();
	result.normalize();

	return result;
}

std::string Vec3::hashVec3(const Vec3& _v) {
	return std::to_string(_v.x) + std::to_string(_v.y) + std::to_string(_v.z);
};

float Vec3::dotProduct(const Vec3& _a, const Vec3& _b) {
	return _a.x * _b.x + _a.y * _b.y + _a.z * _b.z;
}

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