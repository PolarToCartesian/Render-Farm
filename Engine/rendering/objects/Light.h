#pragma once


#include "../../other/Defines.h"
#include "../../cmd/Logging.h"
#include "../../math/Vertex.h"
#include "../../math/Funcs.h"
#include "../../other/pch.h"

struct Light {
	Vec3 position;
	Color<> color;

	float intensity;

	Light(const Vec3& _position, const Color<>& _color, const float _intensity = 5.f);

	static Color<> getDiffusedLighting(const Vec3& _point, const Color<>& _baseColor, const Vec3& _normal, const std::vector<Light>& _lights, const Vec3& _cameraPosition);
	static Color<> getSpecularLighting(const Vec3& _point, const Vec3& _normal, const std::vector<Light>& _lights, const Vec3& _cameraPosition, const float _reflectivity);
	static Color<> getColorWithLighting(const Vec3& _point, const Color<>& _baseColor, const Vec3& _normal, const std::vector<Light>& _lights, const Vec3& _cameraPosition, const float _reflectivity);
};