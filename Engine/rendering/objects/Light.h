#pragma once


#include "../../other/Defines.h"
#include "../../other/Color.h"
#include "../../math/Funcs.h"
#include "../../math/Vec3.h"
#include "../../other/pch.h"

struct Light {
	Vec3 position;
	Color color;

	double intensity = 1;

	Light(const Vec3& _position, const Color& _color, const double& _intensity = 5.f);
};

// Engine Namespace

namespace EN {
	namespace LIGHT {
		std::array<Color, 3> applyLightingToVertices(const Vec3 * _vertices, const Color * _colorOfVertices, const Vec3 & _triangleSurfaceNormal, std::vector<Light> & _lights);
	};
};