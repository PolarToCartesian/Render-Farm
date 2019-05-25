#pragma once

#include "../other/pch.h"

#include "Vector3D.h"

struct Camera {
	Vector3D position;
	Vector3D rotation;

	bool isTriangleFacingCamera(Vector3D _vertices[3], const Vector3D& _triangleSurfaceNormal) const;
};