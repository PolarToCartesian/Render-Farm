#include "Triangle.h"

// Triangle Class

Triangle::Triangle() {  }

Triangle::Triangle(const Vector3D _vertices[3], const Color _colors[3]) {
	std::memcpy(vertices, _vertices, 3 * sizeof(Vector3D));
	std::memcpy(colors, _colors, 3 * sizeof(Vector3D));
}

Triangle::Triangle(const Vector3D _vertices[3], const Color _colors[3], const Vector3D _rotationMidPoint, const Vector3D _rotation) {
	std::memcpy(this->vertices, _vertices, 3 * sizeof(Vector3D));
	std::memcpy(this->colors, _colors, 3 * sizeof(Vector3D));

	this->rotationMidPoint = _rotationMidPoint;
	this->rotation = _rotation;
}

void Triangle::applyFunctionToEachVertex(const std::function<void(Vector3D&)>& _function) {
	for (unsigned int i = 0; i < 3; i++) { _function(this->vertices[i]); }
}

void Triangle::translate(const Vector3D& _deltaPosition) {
	this->applyFunctionToEachVertex([&_deltaPosition](Vector3D& _vertex) { _vertex += _deltaPosition; });
}

// Engine Namespace

namespace EN {
	namespace TRIANGLE {
		Vector3D getSurfaceNormal(const Vector3D _vertices[3]) {
			//https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal

			Vector3D U = _vertices[1] - _vertices[0];
			Vector3D V = _vertices[2] - _vertices[0];

			TYPE nX = U.y * V.z - U.z * V.y;
			TYPE nY = U.z * V.x - U.x * V.z;
			TYPE nZ = U.x * V.y - U.y * V.x;

			return EN::VECTOR3D::normalize(Vector3D(nX, nY, nZ));
		}
	};
};