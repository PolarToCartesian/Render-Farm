#include "Camera.h"

// Camera Struct
bool Camera::isTriangleFacingCamera(Vector3D _vertices[3], const Vector3D& _triangleSurfaceNormal) const {
	Vector3D triangleCenter   = (_vertices[0] + _vertices[1] + _vertices[2]) / 3.f;
	Vector3D triangleToCamera = EN::VECTOR3D::normalize(this->position - triangleCenter);

	return (EN::VECTOR3D::dotProduct(_triangleSurfaceNormal, triangleToCamera) > 0.f);
}