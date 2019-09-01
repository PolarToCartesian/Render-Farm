#pragma once

#include "../../other/pch.h"

#include "../../math/Vertex.h"

struct Camera {
	Vec3 position;
	Vec3 rotation;

	bool isTriangleFacingCamera(const std::array<Vec3, 3>& _points, const Vec3& _triangleSurfaceNormal) const;
};