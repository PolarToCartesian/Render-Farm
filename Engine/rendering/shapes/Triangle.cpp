#include "Triangle.h"

// Triangle Class

Triangle::Triangle() {  }

Triangle::Triangle(const Vec3 _vertices[3], const Color _colors[3]) {
	std::memcpy(vertices, _vertices, 3 * sizeof(Vec3));
	std::memcpy(colors, _colors, 3 * sizeof(Color));
}

Triangle::Triangle(const Vec3 _vertices[3], const Color _colors[3], const Vec3 _rotationMidPoint, const Vec3 _rotation) {
	std::memcpy(this->vertices, _vertices, 3 * sizeof(Vec3));
	std::memcpy(this->colors, _colors, 3 * sizeof(Color));

	this->rotationMidPoint = _rotationMidPoint;
	this->rotation = _rotation;
}

void Triangle::applyFunctionToEachVertex(const std::function<void(Vec3&)>& _function) {
	for (unsigned int i = 0; i < 3; i++) { _function(this->vertices[i]); }
}

void Triangle::translate(const Vec3& _deltaPosition) {
	this->applyFunctionToEachVertex([&_deltaPosition](Vec3& _vertex) { _vertex += _deltaPosition; });
}

Vec3 Triangle::getSurfaceNormal(const Vec3 _vertices[3]) {
	//https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal

	Vec3 U = _vertices[1] - _vertices[0];
	Vec3 V = _vertices[2] - _vertices[0];

	double nX = U.y * V.z - U.z * V.y;
	double nY = U.z * V.x - U.x * V.z;
	double nZ = U.x * V.y - U.y * V.x;

	return Vec3::normalize(Vec3(nX, nY, nZ));
}