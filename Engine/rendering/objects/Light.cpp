#include "Light.h"

// Light Struct

Light::Light(const Vec3& _position, const Color<>& _color, const float _intensity) : position(_position), color(_color), intensity(_intensity) {}

// Engine Namespace

Color<> Light::getColorWithLighting(const Vec3& _point, const Color<>& _baseColor, const Vec3& _normal, const std::vector<Light>& _lights, const Vec3& _cameraPosition, const float _reflectivity) {
	Color<float> colorWithSpecularLighting;

	for (const Light& light : _lights) {
		const Vec3 pointToLight = Vec3::normalize(light.position - _point);

		float diffuseDotProduct = Vec3::dotProduct(pointToLight, _normal);
		if (diffuseDotProduct < 0) diffuseDotProduct = 0;

		const float brightness = diffuseDotProduct * light.intensity;

		const Color<float> diffuse = (Color<float>(light.color) / 255.f) * brightness;

		colorWithSpecularLighting += diffuse * (Color<float>(_baseColor) / 255.f);

		// Specular (https://www.youtube.com/watch?v=Is6D5rnWEvs&list=PL_w_qWAQZtAZhtzPI5pkAtcUVgmzdAP8g&index=11&t=1512s)

		const Vec3 lightToPoint = Vec3::normalize(_point - light.position);
		const Vec3 pointToCamera = Vec3::normalize(_cameraPosition - _point);

		// https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
		const float dotLightToPointAndSurfaceNormal = Vec3::dotProduct(_normal, lightToPoint);

		const Vec3 reflectedLight = Vec3::normalize(lightToPoint - (_normal * dotLightToPointAndSurfaceNormal * 2));

		float specularDotProduct = Vec3::dotProduct(pointToCamera, reflectedLight);
		if (specularDotProduct < 0) specularDotProduct = 0;

		colorWithSpecularLighting += (Color<float>(light.color) / 255.f) * _reflectivity * std::pow(specularDotProduct, 5);
	}

	for (uint8_t v = 0; v < 3; v++)
		colorWithSpecularLighting.constrain(0, 1);

	return Color<>(colorWithSpecularLighting * 255.f);
}