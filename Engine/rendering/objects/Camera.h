#pragma once

#include "../../math/Vertex.h"
#include "../../math/Mat4x4.h"

#include <array>

class Camera {
	public:
		Vec3 position;
		Vec3 rotation;

	private:
		Mat4x4 rotationXMatrix;
		Mat4x4 rotationYMatrix;
		Mat4x4 rotationZMatrix;

	public:
		void calculateRotationMatrices();

		void applyRotation(Vec3& _vertex) const;

		bool isTriangleFacingCamera(const std::array<Vec3, 3>& _points, const Vec3& _triangleSurfaceNormal) const;
};