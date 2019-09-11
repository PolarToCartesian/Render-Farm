#pragma once

#include "../../math/Vec3.h"
#include "../../math/Funcs.h"
#include "../../math/Vertex.h"
#include "../../cmd/Logging.h"
#include "../../other/Defines.h"
#include "../materials/Material.h"

#include <algorithm>

struct Light {
	Vec3 position;
	Color<> color;

	float intensity;

	Light(const Vec3& _position, const Color<>& _color, const float _intensity = 5.f);

	static Color<> getDiffusedLighting(const Vec3& _point,  const Color<>& _baseColor, const Vec3& _normal, const std::vector<Light>& _lights, const Vec3& _cameraPosition);
	static Color<> getSpecularLighting(const Vec3& _point,                             const Vec3& _normal, const std::vector<Light>& _lights, const Vec3& _cameraPosition, const Material& _material);
	static Color<> getColorWithLighting(const Vec3& _point, const Color<>& _baseColor, const Vec3& _normal, const std::vector<Light>& _lights, const Vec3& _cameraPosition, const Material& _material);
};