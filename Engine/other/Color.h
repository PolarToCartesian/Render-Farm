#pragma once

#include "../math/Funcs.h"
#include "pch.h"

template <typename T = uint8_t>
class Color {
	private:
		T getInBounds(const double _v) {
			return static_cast<T>((_v > std::numeric_limits<T>::max()) ? std::numeric_limits<T>::max() : ((_v < 0) ? 0 : _v));
		}

	public:
		T r, g, b;

		Color() : r(0), g(0), b(0) {}
		Color(const T _n) : r(_n), g(_n), b(_n) {}
		Color(const T _r, const T _g, const T _b) : r(_r), g(_g), b(_b) {}

		template <typename T2>
		void operator+=(const Color<T2>& _c) {
			this->r = getInBounds(static_cast<double>(this->r) + _c.r);
			this->g = getInBounds(static_cast<double>(this->g) + _c.g);
			this->b = getInBounds(static_cast<double>(this->b) + _c.b);
		}

		template <typename T2>
		void operator-=(const Color<T2>& _c) {
			this->r = getInBounds(static_cast<double>(this->r) - _c.r);
			this->g = getInBounds(static_cast<double>(this->g) - _c.g);
			this->b = getInBounds(static_cast<double>(this->b) - _c.b);
		}

		template <typename T2>
		void operator*=(const Color<T2>& _c) {
			this->r = getInBounds(static_cast<double>(this->r) * _c.r);
			this->g = getInBounds(static_cast<double>(this->g) * _c.g);
			this->b = getInBounds(static_cast<double>(this->b) * _c.b);
		}

		template <typename T2>
		void operator/=(const Color<T2>& _c) {
			this->r = getInBounds(static_cast<double>(this->r) / _c.r);
			this->g = getInBounds(static_cast<double>(this->g) / _c.g);
			this->b = getInBounds(static_cast<double>(this->b) / _c.b);
		}

		template <typename T2>
		void operator+=(const T2 _n) {
			this->r = getInBounds(static_cast<double>(this->r) + _n);
			this->g = getInBounds(static_cast<double>(this->g) + _n);
			this->b = getInBounds(static_cast<double>(this->b) + _n);
		}

		template <typename T2>
		void operator-=(const T2 _n) {
			this->r = getInBounds(static_cast<double>(this->r) - _n);
			this->g = getInBounds(static_cast<double>(this->g) - _n);
			this->b = getInBounds(static_cast<double>(this->b) - _n);
		}

		template <typename T2>
		void operator*=(const T2 _n) {
			this->r = getInBounds(static_cast<double>(this->r) * _n);
			this->g = getInBounds(static_cast<double>(this->g) * _n);
			this->b = getInBounds(static_cast<double>(this->b) * _n);
		}

		template <typename T2>
		void operator/=(const T2 _n) {
			this->r = getInBounds(static_cast<double>(this->r) / _n);
			this->g = getInBounds(static_cast<double>(this->g) / _n);
			this->b = getInBounds(static_cast<double>(this->b) / _n);
		}
};

template <typename T>
Color<T> operator+(const Color<T>& _c, const double _n) {
	Color<T> r(_c.r, _c.g, _c.b);

	r += _n;

	return r;
}

template <typename T>
Color<T> operator-(const Color<T>& _c, const double _n) {
	Color<T> r(_c.r, _c.g, _c.b);

	r -= _n;

	return r;
}

template <typename T>
Color<T> operator/(const Color<T>& _c, const double _n) {
	Color<T> r(_c.r, _c.g, _c.b);

	r /= _n;

	return r;
}

template <typename T>
Color<T> operator*(const Color<T>& _c, const double _n) {
	Color<T> r(_c.r, _c.g, _c.b);

	r *= _n;

	return r;
}

/// Printing

template <typename T>
std::ostream& operator<<(std::ostream& _os, const Color<T>& _c) {
	return _os << "R : " << std::to_string(_c.r) << ", G : " << std::to_string(_c.g) << ", B : " << std::to_string(_c.b);
}