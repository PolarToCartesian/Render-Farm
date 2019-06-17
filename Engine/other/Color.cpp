#include "Color.h"

uint8_t Color::getInBounds(const double _v) {
	return static_cast<uint8_t>((_v > std::numeric_limits<uint8_t>::max()) ? std::numeric_limits<uint8_t>::max() : ( (_v < 0) ? 0 : _v ));
}

Color::Color() : r(0), g(0), b(0) {}
Color::Color(const uint8_t _n) : r(_n), g(_n), b(_n) {}
Color::Color(const uint8_t _r, const uint8_t _g, const uint8_t _b) : r(_r), g(_g), b(_b) {}

void Color::operator+=(const Color& _c) {
	this->r = getInBounds(static_cast<double>(this->r) + _c.r);
	this->g = getInBounds(static_cast<double>(this->g) + _c.g);
	this->b = getInBounds(static_cast<double>(this->b) + _c.b);
}

void Color::operator-=(const Color& _c) {
	this->r = getInBounds(static_cast<double>(this->r) - _c.r);
	this->g = getInBounds(static_cast<double>(this->g) - _c.g);
	this->b = getInBounds(static_cast<double>(this->b) - _c.b);
}

void Color::operator*=(const Color& _c) {
	this->r = getInBounds(static_cast<double>(this->r) * _c.r);
	this->g = getInBounds(static_cast<double>(this->g) * _c.g);
	this->b = getInBounds(static_cast<double>(this->b) * _c.b);
}

void Color::operator/=(const Color& _c) {
	this->r = getInBounds(static_cast<double>(this->r) / _c.r);
	this->g = getInBounds(static_cast<double>(this->g) / _c.g);
	this->b = getInBounds(static_cast<double>(this->b) / _c.b);
}

template <typename T>
void Color::operator+=(const T& _n) {
	this->r = getInBounds(static_cast<double>(this->r) + _n);
	this->g = getInBounds(static_cast<double>(this->g) + _n);
	this->b = getInBounds(static_cast<double>(this->b) + _n);
}

template <typename T>
void Color::operator-=(const T& _n) {
	this->r = getInBounds(static_cast<double>(this->r) - _n);
	this->g = getInBounds(static_cast<double>(this->g) - _n);
	this->b = getInBounds(static_cast<double>(this->b) - _n);
}

template <typename T>
void Color::operator*=(const T& _n) {
	this->r = getInBounds(static_cast<double>(this->r) * _n);
	this->g = getInBounds(static_cast<double>(this->g) * _n);
	this->b = getInBounds(static_cast<double>(this->b) * _n);
}

template <typename T>
void Color::operator/=(const T& _n) {
	this->r = getInBounds(static_cast<double>(this->r) / _n);
	this->g = getInBounds(static_cast<double>(this->g) / _n);
	this->b = getInBounds(static_cast<double>(this->b) / _n);
}

Color operator+(const Color& _c, const double _n) {
	Color r(_c.r, _c.g, _c.b);

	r += _n;

	return r;
}
Color operator-(const Color& _c, const double _n) {
	Color r(_c.r, _c.g, _c.b);

	r -= _n;

	return r;
}
Color operator/(const Color& _c, const double _n) {
	Color r(_c.r, _c.g, _c.b);

	r /= _n;

	return r;
}

Color operator*(const Color& _c, const double _n) { 
	Color r(_c.r, _c.g, _c.b);

	r *= _n;

	return r;
}