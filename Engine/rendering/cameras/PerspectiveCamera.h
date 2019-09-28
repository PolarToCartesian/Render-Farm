#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera {
	private:
		Mat4x4 rotationXMatrix;
		Mat4x4 rotationYMatrix;
		Mat4x4 rotationZMatrix;

	public:
		PerspectiveCamera(const float _fov = 90.f, const float _zNear = 0.1f, const float _zFar = 1000.f) : Camera(_fov, _zNear, _zFar) {}

		void calculateRotationMatrices();

		void applyRotation(Vec3& _vertex) const;

		bool isTriangleFacingCamera(const std::array<Vec3, 3> & _points, const Vec3& _triangleSurfaceNormal) const;
};