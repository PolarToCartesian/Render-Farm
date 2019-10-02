#pragma once

#include "../math/Funcs.h"

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
			this->r = static_cast<T>(this->r + _c.r);
			this->g = static_cast<T>(this->g + _c.g);
			this->b = static_cast<T>(this->b + _c.b);
		}

		template <typename T2>
		void operator-=(const Color<T2>& _c) {
			this->r = static_cast<T>(this->r - _c.r);
			this->g = static_cast<T>(this->g - _c.g);
			this->b = static_cast<T>(this->b - _c.b);
		}

		template <typename T2>
		void operator*=(const Color<T2>& _c) {
			this->r = static_cast<T>(this->r * _c.r);
			this->g = static_cast<T>(this->g * _c.g);
			this->b = static_cast<T>(this->b * _c.b);
		}

		template <typename T2>
		void operator/=(const Color<T2>& _c) {
			this->r = static_cast<T>(this->r / _c.r);
			this->g = static_cast<T>(this->g / _c.g);
			this->b = static_cast<T>(this->b / _c.b);
		}

		template <typename T2>
		void operator+=(const T2 _n) {
			this->r = static_cast<T>(this->r + _n);
			this->g = static_cast<T>(this->g + _n);
			this->b = static_cast<T>(this->b + _n);
		}

		template <typename T2>
		void operator-=(const T2 _n) {
			this->r = static_cast<T>(this->r - _n);
			this->g = static_cast<T>(this->g - _n);
			this->b = static_cast<T>(this->b - _n);
		}

		template <typename T2>
		void operator*=(const T2 _n) {
			this->r = static_cast<T>(this->r * _n);
			this->g = static_cast<T>(this->g * _n);
			this->b = static_cast<T>(this->b * _n);
		}

		template <typename T2>
		void operator/=(const T2 _n) {
			this->r = static_cast<T>(this->r / _n);
			this->g = static_cast<T>(this->g / _n);
			this->b = static_cast<T>(this->b / _n);
		}

		template <typename T2>
		void constrain(const T2 _min, const T2 _max) {
			const T min = static_cast<T>(_min);
			const T max = static_cast<T>(_max);

			this->r = (this->r < min) ? min : ((this->r > max) ? max : this->r);
			this->g = (this->g < min) ? min : ((this->g > max) ? max : this->g);
			this->b = (this->b < min) ? min : ((this->b > max) ? max : this->b);
		}

		template <typename T2>
		static Color<T2> constrain(const Color<T2>& _c, const T2 _min, const T2 _max) {
			Color<T2> result = _c;
			result.constrain(_min, _max);

			return result;
		}
};

template <typename T, typename T2>
Color<T> operator+(const Color<T>& _c, const T2& _n) {
	Color<T> r(_c);

	r += _n;

	return r;
}

template <typename T, typename T2>
Color<T> operator-(const Color<T>& _c, const T2& _n) {
	Color<T> r(_c);

	r -= _n;

	return r;
}

template <typename T, typename T2>
Color<T> operator*(const Color<T>& _c, const T2& _n) {
	Color<T> r(_c);

	r *= _n;

	return r;
}

template <typename T, typename T2>
Color<T> operator/(const Color<T>& _c, const T2& _n) {
	Color<T> r(_c);

	r /= _n;

	return r;
}

/// Printing

template <typename T>
std::ostream& operator<<(std::ostream& _os, const Color<T>& _c) {
	return _os << "R : " << std::to_string(_c.r) << ", G : " << std::to_string(_c.g) << ", B : " << std::to_string(_c.b);
}