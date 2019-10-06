#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(const uint16_t _width, const uint16_t _height, const float _fov, const float _zNear, const float _zFar)
	: Camera(_width, _height, _fov, _zNear, zFar) {}

void PerspectiveCamera::calculateMatrices() {
	if (this->wasModified) {
		this->rotationXMatrix = Mat4x4::getRotationXMatrix(-this->rotation.x);
		this->rotationYMatrix = Mat4x4::getRotationYMatrix(-this->rotation.y);
		this->rotationZMatrix = Mat4x4::getRotationZMatrix(-this->rotation.z);

		this->projectionMatrix = Mat4x4::getPerspectiveMatrix(this->width, this->height, this->fov, this->zNear, this->zFar);

		this->wasModified = false;
	}
}

void PerspectiveCamera::applyProjection(Vec3& _vertex) const {
	_vertex *= this->projectionMatrix;

	if (_vertex.w <= 0)
		return;

	_vertex /= _vertex.w;

	// -1 to 1    to    0-width
	_vertex.x = ((_vertex.x - 1.f) / -2.f) * this->width;
	// 1  to -1   to    height-0
	_vertex.y = ((_vertex.y + 1.f) / +2.f) * this->height;
}