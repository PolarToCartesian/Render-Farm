#pragma once

#include "pch.h"

class Color {
	private:
		unsigned char getInBounds(const double& _v);

	public:
		unsigned char r, g, b;

		Color();
		Color(const unsigned char& _n);
		Color(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b);

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

Color operator+(const Color& _c, const double& _n);
Color operator-(const Color& _c, const double& _n);
Color operator/(const Color& _c, const double& _n);
Color operator*(const Color& _c, const double& _n);