#pragma once

#include "Vec3.h"

// Thanks to : https://codeplea.com/triangular-interpolation

class BarycentricInterpolation {
	private:
		Vec3 vertices[3] = { 0.f };
		float precalculated[4] = { 0.f };
		float precalculated2[2] = { 0.f };
		float denominator;

	public:
		BarycentricInterpolation(const Vec3 _v[3]) {
			std::memcpy(this->vertices, _v, 3 * sizeof(Vec3));

			this->denominator = (_v[1].y - _v[2].y) * (_v[0].x - _v[2].x) +
				(_v[2].x - _v[1].x) * (_v[0].y - _v[2].y);

			this->precalculated[0] = (_v[1].y - _v[2].y);
			this->precalculated[1] = (_v[2].x - _v[1].x);
			this->precalculated[2] = (_v[2].y - _v[0].y);
			this->precalculated[3] = (_v[0].x - _v[2].x);
		}

		void precalculateValuesForPosition(const uint16_t _x, const uint16_t _y) {
			precalculated2[0] = (_y - this->vertices[2].y);
			precalculated2[1] = (_x - this->vertices[2].x);
		}

		template <typename T>
		T interpolateWPrecalc(const T _attribs[3]) const {
			float weights[3] = { (this->precalculated[0] * precalculated2[1] + this->precalculated[1] * precalculated2[0]) / this->denominator,
								 (this->precalculated[2] * precalculated2[1] + this->precalculated[3] * precalculated2[0]) / this->denominator,
								  0.f };

			weights[2] = 1 - weights[0] - weights[1];

			const float weightSum = weights[0] + weights[1] + weights[2];

			// Use unique_ptr so that the memory of out is always initialized
			std::unique_ptr<T> out = std::make_unique<T>();

			for (uint8_t c = 0; c < 3; c++)
				*(out.get()) += _attribs[c] * weights[c];

			*(out.get()) /= weightSum;

			return *(out.get());
		}

		template <typename T>
		T interpolate(const uint16_t _x, const uint16_t _y, const T _attribs[3]) {
			precalculateValuesForPosition(_x, _y);

			return interpolateWithPrecalculatedValuesForPosition(_attribs);
		}
};