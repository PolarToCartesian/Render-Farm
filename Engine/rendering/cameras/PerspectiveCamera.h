#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(const uint16_t _width, const uint16_t _height, const float _fov = 90.f, const float _zNear = 0.1f, const float _zFar = 1000.f);

		void calculateMatrices();
		void applyProjection(Vec3& _vertex) const;
};