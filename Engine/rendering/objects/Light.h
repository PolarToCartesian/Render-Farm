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

	static std::array<Color<>, 3> applyLightingToVertices(const Vec3 _points[3], const Color<> _colors[3], const Vec3& _triangleSurfaceNormal, const std::vector<Light>& _lights, const Vec3& _cameraPosition, const float _reflectivity);
};