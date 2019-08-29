#include "Light.h"

// Light Struct

Light::Light(const Vec3& _position, const Color<>& _color, const float _intensity) : position(_position), color(_color), intensity(_intensity) {}

// Engine Namespace

Color<> Light::getDiffusedLighting(const Vec3& _point, const Color<>& _baseColor, const Vec3& _normal, const std::vector<Light>& _lights, const Vec3& _cameraPosition) {
	Color<float> outColor;

	for (const Light& light : _lights) {
		const Vec3 pointToLight = Vec3::normalize(light.position - _point);

		const float diffuseDotProduct = std::max(Vec3::dotProduct(pointToLight, _normal), 0.f);

		const float brightness = diffuseDotProduct * light.intensity;

		const Color<float> diffuse = (Color<float>(light.color) / 255.f) * brightness;

		outColor += diffuse * (Color<float>(_baseColor) / 255.f);
	}

	outColor.constrain(0, 1);

	return Color<>(outColor * 255.f);
}

Color<> Light::getSpecularLighting(const Vec3& _point, const Vec3& _normal, const std::vector<Light>& _lights, const Vec3& _cameraPosition, const float _reflectivity) {
	Color<float> outColor;

	for (const Light& light : _lights) {
		// Specular (https://www.youtube.com/watch?v=Is6D5rnWEvs&list=PL_w_qWAQZtAZhtzPI5pkAtcUVgmzdAP8g&index=11&t=1512s)

		const Vec3 lightToPoint = Vec3::normalize(_point - light.position);
		const Vec3 pointToCamera = Vec3::normalize(_cameraPosition - _point);

		// https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
		const float dotLightToPointAndSurfaceNormal = Vec3::dotProduct(_normal, lightToPoint);

		const Vec3 reflectedLight = Vec3::normalize(lightToPoint - (_normal * dotLightToPointAndSurfaceNormal * 2));

		const float specularDotProduct = std::max(Vec3::dotProduct(pointToCamera, reflectedLight), 0.f);

		outColor += (Color<float>(light.color) / 255.f) * _reflectivity * std::pow(specularDotProduct, 5);
	}

	outColor.constrain(0, 1);

	return Color<>(outColor * 255.f);
}

Color<> Light::getColorWithLighting(const Vec3& _point, const Color<>& _baseColor, const Vec3& _normal, const std::vector<Light>& _lights, const Vec3& _cameraPosition, const float _reflectivity) {
	Color<float> outColor;

	outColor += Color<float>(Light::getDiffusedLighting(_point, _baseColor, _normal, _lights, _cameraPosition)) / 255.f;
	outColor += Color<float>(Light::getSpecularLighting(_point, _normal, _lights, _cameraPosition, _reflectivity)) / 255.f;

	outColor.constrain(0, 1);

	return Color<>(outColor * 255.f);
}