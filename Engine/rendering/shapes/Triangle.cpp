#include "Triangle.h"

// Triangle Class

Triangle::Triangle() {}

Triangle::Triangle(const Vertex _vertices[3], const float _reflectivity, const bool _isSmoothed) : isSmoothed(_isSmoothed), reflectivity(_reflectivity) {
	std::memcpy(this->vertices, _vertices, 3 * sizeof(Vertex));

	for (uint8_t i = 0; i < 3; i++)
		this->rotationMidPoint += this->vertices[i].position;

	this->rotationMidPoint /= 3;
}

Triangle::Triangle(const Vertex _vertices[3], const Vec3& _rotationMidPoint, const Vec3& _rotation, const float _reflectivity, const bool _isSmoothed) : isSmoothed(_isSmoothed), reflectivity(_reflectivity), rotationMidPoint(_rotationMidPoint), rotation(_rotation) {
	std::memcpy(this->vertices, _vertices, 3 * sizeof(Vertex));
}

void Triangle::applyFunctionToEachVertex(const std::function<void(Vertex&)>& _function) {
	for (uint8_t i = 0; i < 3; i++) { _function(this->vertices[i]); }
}

void Triangle::translate(const Vec3& _deltaPosition) {
	this->applyFunctionToEachVertex([&_deltaPosition](Vertex& _vertex) { _vertex.position += _deltaPosition; });
	this->rotationMidPoint += _deltaPosition;
}

Vec3 Triangle::getSurfaceNormal(const std::array<Vec3, 3>& _points) {
	//https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal

	Vec3 U = _points[1] - _points[0];
	Vec3 V = _points[2] - _points[0];

	float nX = U.y * V.z - U.z * V.y;
	float nY = U.z * V.x - U.x * V.z;
	float nZ = U.x * V.y - U.y * V.x;

	return Vec3::normalize(Vec3(nX, nY, nZ));
}