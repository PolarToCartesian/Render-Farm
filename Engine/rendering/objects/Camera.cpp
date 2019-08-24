#include "Camera.h"

// Camera Struct
bool Camera::isTriangleFacingCamera(const std::array<Vec3, 3>& _points, const Vec3& _triangleSurfaceNormal) const
{
	Vec3 triangleCenter   = (_points[0] + _points[1] + _points[2]) / 3.f;
	Vec3 triangleToCamera = Vec3::normalize(this->position - triangleCenter);

	return (Vec3::dotProduct(_triangleSurfaceNormal, triangleToCamera) > 0.f);
}