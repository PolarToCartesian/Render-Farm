#pragma once

#include "../../math/Vec3.h"

#include <array>

class Camera {
	public:
		Vec3 position;
		Vec3 rotation;

		float fov, zNear, zFar;
	public:
		Camera(const float _fov = 90.f, const float _zNear = 0.1f, const float _zFar = 1000.f) : fov(_fov), zNear(_zNear), zFar(_zFar) {}

		virtual void calculateRotationMatrices() = 0;

		virtual void applyRotation(Vec3& _vertex) const = 0;

		virtual bool isTriangleFacingCamera(const std::array<Vec3, 3>& _points, const Vec3& _triangleSurfaceNormal) const = 0;
};