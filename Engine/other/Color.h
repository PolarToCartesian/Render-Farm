#pragma once

#include "../math/Funcs.h"
#include "pch.h"

class Color {
	private:
		uint8_t getInBounds(const double _v);

	public:
		uint8_t r, g, b;

		Color();
		Color(const uint8_t _n);
		Color(const uint8_t _r, const uint8_t _g, const uint8_t _b);

		void operator+=(const Color& _c);
		void operator-=(const Color& _c);
		void operator*=(const Color& _c);
		void operator/=(const Color& _c);
		template <typename T>
		void operator+=(const T& _n);
		template <typename T>
		void operator-=(const T& _n);
		template <typename T>
		void operator*=(const T& _n);
		template <typename T>
		void operator/=(const T& _n);
};

Color operator+(const Color& _c, const double _n);
Color operator-(const Color& _c, const double _n);
Color operator/(const Color& _c, const double _n);
Color operator*(const Color& _c, const double _n);