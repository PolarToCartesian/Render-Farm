#include "Triangle.h"

// Triangle Class

Triangle::Triangle() {}

Triangle::Triangle(const Vertex _vertices[3], const std::string& _material, const bool _isSmoothed) : isSmoothed(_isSmoothed), material(_material) {
	std::memcpy(this->vertices, _vertices, 3 * sizeof(Vertex));

	for (uint8_t i = 0; i < 3; i++)
		this->rotationMidPoint += this->vertices[i].position;

	this->rotationMidPoint /= 3;
}

Triangle::Triangle(const Vertex _vertices[3], const std::string& _material, const Vec3& _rotationMidPoint, const Vec3& _rotation, const bool _isSmoothed) : isSmoothed(_isSmoothed), material(_material), rotationMidPoint(_rotationMidPoint), rotation(_rotation) {
	std::memcpy(this->vertices, _vertices, 3 * sizeof(Vertex));
}

void Triangle::applyFunctionToEachVertex(const std::function<void(Vertex&)>& _function) {
	for (uint8_t i = 0; i < 3; i++) { _function(this->vertices[i]); }
}

void Triangle::translate(const Vec3& _deltaPosition) {
	this->applyFunctionToEachVertex([&_deltaPosition](Vertex& _vertex) { _vertex.position += _deltaPosition; });
	this->rotationMidPoint += _deltaPosition;
}

std::array<Vec3, 3> Triangle::getRotatedVertices(const Camera& _cam) const {
	std::array<Vec3, 3> rotatedVertices{
		this->vertices[0].position + Vec3(0, 0, 0, 1),
		this->vertices[1].position + Vec3(0, 0, 0, 1),
		this->vertices[2].position + Vec3(0, 0, 0, 1)
	};

	const Mat4x4 triangleRotationMatrix = Mat4x4::getRotationMatrix(this->rotation.x, this->rotation.y, this->rotation.z);

	// For Every Vertex
	for (uint8_t v = 0; v < 3; v++) {
		// Rotate Triangle Around Point
		rotatedVertices[v] -= this->rotationMidPoint;
		rotatedVertices[v] *= triangleRotationMatrix;
		rotatedVertices[v] += this->rotationMidPoint;

		// Apply Camera Rotation
		_cam.applyRotation(rotatedVertices[v]);
	}

	return rotatedVertices;
}

Vec3 Triangle::getSurfaceNormal(const std::array<Vec3, 3>& _points) {
	//https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal

	const Vec3 U = _points[1] - _points[0];
	const Vec3 V = _points[2] - _points[0];

	return Vec3::normalize(Vec3(U.y * V.z - U.z * V.y, 
								U.z * V.x - U.x * V.z,
								U.x * V.y - U.y * V.x));
}

std::array<Vec3, 3> Triangle::getTransformedVertices(const std::array<Vec3, 3>& _rotatedVertices, const Camera& _cam, const Mat4x4& _perspectiveMatrix, const uint16_t _width, const uint16_t _height) {
	std::array<Vec3, 3> transformedVertices = _rotatedVertices;

	// For Every Vertex
	for (uint8_t v = 0; v < 3; v++) {
		// Camera Translation
		transformedVertices[v] -= _cam.position;

		// Apply Perspective
		transformedVertices[v] = transformedVertices[v] * _perspectiveMatrix;

		if (transformedVertices[v].w < 0)
			continue;

		if (transformedVertices[v].w > 0)
			transformedVertices[v] /= transformedVertices[v].w;

		// -1 to 1    to    0-width
		transformedVertices[v].x = ((transformedVertices[v].x - 1.f) / -2.f) * _width;
		// 1  to -1   to    height-0
		transformedVertices[v].y = ((transformedVertices[v].y + 1.f) / +2.f) * _height;
	}

	return transformedVertices;
}