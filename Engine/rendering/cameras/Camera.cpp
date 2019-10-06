#include "Camera.h"

Camera::Camera(const uint16_t _width, const uint16_t _height, const float _fov, const float _zNear, const float _zFar)
	: width(_width), height(_height), fov(_fov), zNear(_zNear), zFar(_zFar) {}

Vec3 Camera::getPosition()   const { return this->position; }
Vec3 Camera::getRotation()   const { return this->rotation; }
float Camera::getFov()       const { return this->fov; }
float Camera::getZNear()     const { return this->zNear; }
float Camera::getZFar()      const { return this->zFar; }
uint16_t Camera::getWidth()  const { return this->width; }
uint16_t Camera::getHeight() const { return this->height; }

void Camera::setPosition(const Vec3& _position) { this->wasModified = true; this->position = _position; }
void Camera::setRotation(const Vec3& _rotation) { this->wasModified = true; this->rotation = _rotation; }
void Camera::setFov(const float _fov) { this->wasModified = true; this->fov = _fov; }
void Camera::setZNear(const float _zNear) { this->wasModified = true; this->zNear = _zNear; }
void Camera::setZFar(const float _zFar) { this->wasModified = true; this->zFar = _zFar; }
void Camera::setWidth(const uint16_t _width) { this->wasModified = true; this->width = _width; }
void Camera::setHeight(const uint16_t _height) { this->wasModified = true; this->height = _height; }

void Camera::applyTranslation(Vec3& _vertex) const {
	_vertex -= this->position;
}

void Camera::applyRotation(Vec3& _vertex) const {
	_vertex -= this->position;

	_vertex += _vertex * (-3)
		+ _vertex * this->rotationXMatrix
		+ _vertex * this->rotationYMatrix
		+ _vertex * this->rotationZMatrix;

	_vertex += this->position;
}

bool Camera::isTriangleFacingCamera(const std::array<Vec3, 3> & _points, const Vec3& _triangleSurfaceNormal) const {
	const Vec3 triangleCenter = (_points[0] + _points[1] + _points[2]) / 3.f;
	const Vec3 triangleToCamera = Vec3::normalize(this->position - triangleCenter);

	return (Vec3::dotProduct(_triangleSurfaceNormal, triangleToCamera) > 0.f);
}