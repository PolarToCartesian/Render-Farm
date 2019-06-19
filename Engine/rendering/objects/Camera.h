#pragma once

#include "../../math/Vertex.h"
#include "../../other/pch.h"

struct Camera {
	Vec3 position;
	Vec3 rotation;

	bool isTriangleFacingCamera(Vec3 _points[3], const Vec3& _triangleSurfaceNormal) const;
};