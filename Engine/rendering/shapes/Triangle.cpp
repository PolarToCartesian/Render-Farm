#include "Triangle.h"

// Triangle Class

Triangle::Triangle() {}

Triangle::Triangle(const Vec3 _vertices[3], const Color& _color, const bool _useRandomColors) {
	std::memcpy(this->vertices, _vertices, 3 * sizeof(Vec3));

	using MATH::randomInt;

	for (uint8_t i = 0; i < 3; i++) {
		this->colors[i] = (_useRandomColors == true) ? Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)) : _color;
		this->rotationMidPoint += this->vertices[i];
	}

	this->rotationMidPoint /= 3;
}

Triangle::Triangle(const Vec3 _vertices[3], const Color _colors[3]) {
	std::memcpy(this->vertices, _vertices, 3 * sizeof(Vec3));
	std::memcpy(this->colors,   _colors,   3 * sizeof(Color));

	for (uint8_t i = 0; i < 3; i++) this->rotationMidPoint += this->vertices[i];

	this->rotationMidPoint /= 3;
}

Triangle::Triangle(const Vec3 _vertices[3], const Color& _flatColor, const Vec3& _rotationMidPoint, const Vec3& _rotation, const bool _useRandomColors) 
	: Triangle(_vertices, new Color[3] { _flatColor, _flatColor, _flatColor }, _rotationMidPoint, _rotation, _useRandomColors) {}

Triangle::Triangle(const Vec3 _vertices[3], const Color _colors[3], const Vec3& _rotationMidPoint, const Vec3& _rotation, const bool _useRandomColors) {
	std::memcpy(this->vertices, _vertices, 3 * sizeof(Vec3));
	std::memcpy(this->colors, _colors, 3 * sizeof(Color));

	this->rotationMidPoint = _rotationMidPoint;
	this->rotation = _rotation;

	if (_useRandomColors) {
		using MATH::randomInt;

		for (uint8_t i = 0; i < 3; i++) {
			this->colors[i] = Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255));
		}
	}
}

void Triangle::applyFunctionToEachVertex(const std::function<void(Vec3&)>& _function) {
	for (uint8_t i = 0; i < 3; i++) { _function(this->vertices[i]); }
}

void Triangle::translate(const Vec3& _deltaPosition) {
	this->applyFunctionToEachVertex([&_deltaPosition](Vec3& _vertex) { _vertex += _deltaPosition; });
	this->rotationMidPoint += _deltaPosition;
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