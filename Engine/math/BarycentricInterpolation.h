#pragma once

#include "Vec3.h"

// Thanks to : https://codeplea.com/triangular-interpolation

struct BarycentricInterpolation {
	Vec3 vertices[3] = { 0.f };
	float precalculated[4] = { 0.f };
	float denominator;

	BarycentricInterpolation(const Vec3 _v[3]) {
		std::memcpy(this->vertices, _v, 3 * sizeof(Vec3));

		this->denominator = (_v[1].y - _v[2].y) * (_v[0].x - _v[2].x) +
							(_v[2].x - _v[1].x) * (_v[0].y - _v[2].y);

		this->precalculated[0] = (_v[1].y - _v[2].y);
		this->precalculated[1] = (_v[2].x - _v[1].x);
		this->precalculated[2] = (_v[2].y - _v[0].y);
		this->precalculated[3] = (_v[0].x - _v[2].x);
	}

	template <typename T>
	T interpolate(const uint16_t _x, const uint16_t _y, const T _attribs[3]) const {
		const float a = (_y - this->vertices[2].y), b = (_x - this->vertices[2].x);

		float weights[3] = { (this->precalculated[0] * b + this->precalculated[1] * a) / this->denominator,
							 (this->precalculated[2] * b + this->precalculated[3] * a) / this->denominator,
							 0.f };

		weights[2] = 1 - weights[0] - weights[1];

		const float weightSum = weights[0] + weights[1] + weights[2];

		T out;
		if constexpr (std::is_same_v<T, float> || std::is_same_v<T, int>)
			out = 0;

		for (uint8_t c = 0; c < 3; c++)
			out += _attribs[c] * weights[c];

		out /= weightSum;

		return out;
	}
};