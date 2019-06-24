#include "Light.h"

// Light Struct

Light::Light(const Vec3& _position, const Color<>& _color, const double _intensity) : position(_position), color(_color), intensity(_intensity) {}

// Engine Namespace

std::array<Color<>, 3> Light::applyLightingToVertices(const Vec3 _points[3], const Color<> _colors[3], const Vec3& _triangleSurfaceNormal, const std::vector<Light>& _lights) {
	std::array<Color<>, 3> output{ { Color<>(0), Color<>(0), Color<>(0) } };

	for (uint8_t v = 0; v < 3; v++) {
		double totalVertexBrightness = 0.f;

		for (unsigned int lightIndex = 0; lightIndex < _lights.size(); lightIndex++) {
			Vec3 vertexToLight = _lights[lightIndex].position - _points[v];

			double dotProduct = Vec3::dotProduct(vertexToLight, _triangleSurfaceNormal);

			if (dotProduct < 0) dotProduct = 0;

			totalVertexBrightness += (dotProduct * _lights[lightIndex].intensity) / std::pow(vertexToLight.getLength(), 2);
		}

		totalVertexBrightness = MATH::constrain(totalVertexBrightness, 0, 1);

		output[v] = (_colors[v] * totalVertexBrightness);
	}

	return output;
}