#pragma once

#include "../math/Funcs.h"
#include "pch.h"

template <typename T = uint8_t>
class Color {
	public:
		T r, g, b;

		Color() : r(0), g(0), b(0) {}
		Color(const T _n) : r(_n), g(_n), b(_n) {}
		Color(const T _r, const T _g, const T _b) : r(_r), g(_g), b(_b) {}

		template <typename T2>
		Color(const Color<T2>& _color) {
			this->r = static_cast<T>(_color.r);
			this->g = static_cast<T>(_color.g);
			this->b = static_cast<T>(_color.b);
		}

		template <typename T2>
		void operator+=(const Color<T2>& _c) {
			this->r = static_cast<T2>(this->r + _c.r);
			this->g = static_cast<T2>(this->g + _c.g);
			this->b = static_cast<T2>(this->b + _c.b);
		}

		template <typename T2>
		void operator-=(const Color<T2>& _c) {
			this->r = static_cast<T2>(this->r - _c.r);
			this->g = static_cast<T2>(this->g - _c.g);
			this->b = static_cast<T2>(this->b - _c.b);
		}

		template <typename T2>
		void operator*=(const Color<T2>& _c) {
			this->r = static_cast<T2>(this->r * _c.r);
			this->g = static_cast<T2>(this->g * _c.g);
			this->b = static_cast<T2>(this->b * _c.b);
		}

		template <typename T2>
		void operator/=(const Color<T2>& _c) {
			this->r = static_cast<T2>(this->r / _c.r);
			this->g = static_cast<T2>(this->g / _c.g);
			this->b = static_cast<T2>(this->b / _c.b);
		}

		template <typename T2>
		void operator+=(const T2 _n) {
			this->r = static_cast<T2>(this->r + _n);
			this->g = static_cast<T2>(this->g + _n);
			this->b = static_cast<T2>(this->b + _n);
		}

		template <typename T2>
		void operator-=(const T2 _n) {
			this->r = static_cast<T2>(this->r - _n);
			this->g = static_cast<T2>(this->g - _n);
			this->b = static_cast<T2>(this->b - _n);
		}

		template <typename T2>
		void operator*=(const T2 _n) {
			this->r = static_cast<T2>(this->r * _n);
			this->g = static_cast<T2>(this->g * _n);
			this->b = static_cast<T2>(this->b * _n);
		}

		template <typename T2>
		void operator/=(const T2 _n) {
			this->r = static_cast<T2>(this->r / _n);
			this->g = static_cast<T2>(this->g / _n);
			this->b = static_cast<T2>(this->b / _n);
		}

		template <typename T2>
		void constrain(const T2 _min, const T2 _max) {
			this->r = (this->r < _min) ? _min : (this->r > _max) ? _max : this->r;
			this->g = (this->g < _min) ? _min : (this->g > _max) ? _max : this->g;
			this->b = (this->b < _min) ? _min : (this->b > _max) ? _max : this->b;
		}
};

template <typename T>
Color<T> operator+(const Color<T>& _c, const double _n) {
	Color<T> r(_c);

	r += _n;

	return r;
}

template <typename T>
Color<T> operator-(const Color<T>& _c, const double _n) {
	Color<T> r(_c);

	r -= _n;

	return r;
}

template <typename T>
Color<T> operator/(const Color<T>& _c, const double _n) {
	Color<T> r(_c);

	r /= _n;

	return r;
}

template <typename T>
Color<T> operator*(const Color<T>& _c, const double _n) {
	Color<T> r(_c);

	r *= _n;

	return r;
}

/// Printing

template <typename T>
std::ostream& operator<<(std::ostream& _os, const Color<T>& _c) {
	return _os << "R : " << std::to_string(_c.r) << ", G : " << std::to_string(_c.g) << ", B : " << std::to_string(_c.b);
}