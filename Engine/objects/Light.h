#pragma once

#include "../pch.h"

#include "Vector3D.h"
#include "../other/Util.h"

struct Light {
	Vector3D position;
	Color color;

	TYPE intensity = 1;

	Light(const Vector3D& _position, const Color& _color, const TYPE& _intensity = 5.f);
};

// Engine Namespace

namespace EN {
	namespace LIGHT {
		void applyLightingToVertices(const Vector3D* _vertices, const Color* _colorOfVertices, const Vector3D& _triangleSurfaceNormal, std::vector<Light>& _lights, Color _output[3]);
	};
};