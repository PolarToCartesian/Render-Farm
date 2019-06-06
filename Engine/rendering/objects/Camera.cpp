#include "Camera.h"

// Camera Struct
bool Camera::isTriangleFacingCamera(Vec3 _vertices[3], const Vec3& _triangleSurfaceNormal) const {
	Vec3 triangleCenter   = (_vertices[0] + _vertices[1] + _vertices[2]) / 3.f;
	Vec3 triangleToCamera = EN::VECTOR3D::normalize(this->position - triangleCenter);

	return (EN::VECTOR3D::dotProduct(_triangleSurfaceNormal, triangleToCamera) > 0.f);
}