#include "Light.h"

// Light Struct

Light::Light(const Vec3& _position, const Color<>& _color, const double _intensity) : position(_position), color(_color), intensity(_intensity) {}

// Engine Namespace

std::array<Color<>, 3> Light::applyLightingToVertices(const Vec3 _points[3], const Color<> _colors[3], const Vec3& _triangleSurfaceNormal, const std::vector<Light>& _lights)
{
	Color<float> vertexColorsWithLighting[3];

	for (const Light& light : _lights)
	{
		for (uint8_t v = 0; v < 3; v++)
		{
			const Vec3 vertexToLight = light.position - _points[v];
			
			const double dotProduct = MATH::constrain(Vec3::dotProduct(vertexToLight, _triangleSurfaceNormal), 0, 1);

			const double brightness = MATH::constrain((dotProduct * light.intensity) / std::pow(vertexToLight.getLength(), 2), 0, 1);

			vertexColorsWithLighting[v] += (Color<float>(light.color) / 255.f) * (Color<float>(_colors[v]) / 255.f) * brightness;
		}
	}

	for (uint8_t v = 0; v < 3; v++)
		vertexColorsWithLighting[v].constrain(0, 1);

	std::array<Color<>, 3> output = { Color<>(vertexColorsWithLighting[0] * 255), Color<>(vertexColorsWithLighting[1] * 255), Color<>(vertexColorsWithLighting[2] * 255) };

	return output;
}