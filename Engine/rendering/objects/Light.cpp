#include "Light.h"

// Light Struct

Light::Light(const Vec3& _position, const Color<>& _color, const float _intensity) : position(_position), color(_color), intensity(_intensity) {}

// Engine Namespace

std::array<Color<>, 3> Light::applyLightingToVertices(const Vec3 _points[3], const Color<> _colors[3], const Vec3& _triangleSurfaceNormal, const std::vector<Light>& _lights, const Vec3& _cameraPosition, const float _reflectivity)
{
	Color<float> vertexColorsWithLighting[3];

	for (const Light& light : _lights) {
		for (uint8_t v = 0; v < 3; v++) {
			const Vec3 vertexToLight = Vec3::normalize(light.position - _points[v]);

			float diffuseDotProduct = Vec3::dotProduct(vertexToLight, _triangleSurfaceNormal);
			if (diffuseDotProduct   < 0) diffuseDotProduct = 0;

			const float brightness = diffuseDotProduct * light.intensity;

			const Color<float> diffuse = (Color<float>(light.color) / 255.f) * brightness;

			vertexColorsWithLighting[v] += diffuse * (Color<float>(_colors[v]) / 255.f);
			
			// Specular (https://www.youtube.com/watch?v=Is6D5rnWEvs&list=PL_w_qWAQZtAZhtzPI5pkAtcUVgmzdAP8g&index=11&t=1512s)

			const Vec3 lightToVertex = Vec3::normalize(vertexToLight * (-1));
			const Vec3 vertexToCamera = Vec3::normalize(_cameraPosition - _points[v]);

			// https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
			const float dotLightToVertexAndSurfaceNormal = Vec3::dotProduct(_triangleSurfaceNormal, lightToVertex);

			const Vec3 reflectedLight = Vec3::normalize(lightToVertex - (_triangleSurfaceNormal * dotLightToVertexAndSurfaceNormal * 2));

			float specularDotProduct = Vec3::dotProduct(vertexToCamera, reflectedLight);
			if (specularDotProduct   < 0) specularDotProduct = 0;

			vertexColorsWithLighting[v] += (Color<float>(light.color) / 255.f) * _reflectivity * std::pow(specularDotProduct, 5);
		}
	}

	for (uint8_t v = 0; v < 3; v++)
		vertexColorsWithLighting[v].constrain(0, 1);

	std::array<Color<>, 3> output = { Color<>(vertexColorsWithLighting[0] * 255), Color<>(vertexColorsWithLighting[1] * 255), Color<>(vertexColorsWithLighting[2] * 255) };

	return output;
}