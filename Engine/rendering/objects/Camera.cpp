#include "Camera.h"


void Camera::calculateRotationMatrices() {
	this->rotationXMatrix = Mat4x4::getRotationXMatrix(-this->rotation.x);
	this->rotationYMatrix = Mat4x4::getRotationYMatrix(-this->rotation.y);
	this->rotationZMatrix = Mat4x4::getRotationZMatrix(-this->rotation.z);
}

void Camera::applyRotation(Vec3& _vertex) const {
	_vertex -= this->position;
	_vertex += _vertex * (-3)
		    +  _vertex * this->rotationXMatrix
		    +  _vertex * this->rotationYMatrix
		    +  _vertex * this->rotationZMatrix;
		
	_vertex += this->position;
}

bool Camera::isTriangleFacingCamera(const std::array<Vec3, 3>& _points, const Vec3& _triangleSurfaceNormal) const {
	Vec3 triangleCenter   = (_points[0] + _points[1] + _points[2]) / 3.f;
	Vec3 triangleToCamera = Vec3::normalize(this->position - triangleCenter);

	return (Vec3::dotProduct(_triangleSurfaceNormal, triangleToCamera) > 0.f);
}