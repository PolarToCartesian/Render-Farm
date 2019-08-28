#include "Box.h"

Box::Box(const Vec3& _center, const float sideLength, const bool _randomColor, const Color<>& _color, const Vec3& _rotationMidPoint, const Vec3& _rotation, const float _reflectivity) {
	const float hsl = sideLength / 2.f;

	const Vertex vertices[8] = {
		Vertex(_center + Vec3(-hsl, +hsl, -hsl), _randomColor, _color), // 0
		Vertex(_center + Vec3(+hsl, +hsl, -hsl), _randomColor, _color), // 1
		Vertex(_center + Vec3(+hsl, -hsl, -hsl), _randomColor, _color), // 2
		Vertex(_center + Vec3(-hsl, -hsl, -hsl), _randomColor, _color), // 3
		Vertex(_center + Vec3(+hsl, -hsl, +hsl), _randomColor, _color), // 4
		Vertex(_center + Vec3(+hsl, +hsl, +hsl), _randomColor, _color), // 5
		Vertex(_center + Vec3(-hsl, +hsl, +hsl), _randomColor, _color), // 6
		Vertex(_center + Vec3(-hsl, -hsl, +hsl), _randomColor, _color), // 7
	};

	const Triangle trs[12] = {
		// Front Face
		Triangle(new Vertex[3] { vertices[0], vertices[1], vertices[2] }, _rotationMidPoint, _rotation, _reflectivity),
		Triangle(new Vertex[3] { vertices[0], vertices[2], vertices[3] }, _rotationMidPoint, _rotation, _reflectivity),

		// Back Face
		Triangle(new Vertex[3] { vertices[4], vertices[5], vertices[6] }, _rotationMidPoint, _rotation, _reflectivity),
		Triangle(new Vertex[3] { vertices[7], vertices[4], vertices[6] }, _rotationMidPoint, _rotation, _reflectivity),

		// Left Face
		Triangle(new Vertex[3] { vertices[0], vertices[3], vertices[7] }, _rotationMidPoint, _rotation, _reflectivity),
		Triangle(new Vertex[3] { vertices[6], vertices[0], vertices[7] }, _rotationMidPoint, _rotation, _reflectivity),
	
		// Right Face
		Triangle(new Vertex[3] { vertices[4], vertices[2], vertices[1] }, _rotationMidPoint, _rotation, _reflectivity),
		Triangle(new Vertex[3] { vertices[4], vertices[1], vertices[5] }, _rotationMidPoint, _rotation, _reflectivity),

		// Top Face
		Triangle(new Vertex[3] { vertices[6], vertices[5], vertices[1] }, _rotationMidPoint, _rotation, _reflectivity),
		Triangle(new Vertex[3] { vertices[1], vertices[0], vertices[6] }, _rotationMidPoint, _rotation, _reflectivity),

		// Bottom Face
		Triangle(new Vertex[3] { vertices[2], vertices[4], vertices[7] }, _rotationMidPoint, _rotation, _reflectivity),
		Triangle(new Vertex[3] { vertices[7], vertices[3], vertices[2] }, _rotationMidPoint, _rotation, _reflectivity),
	};

	this->triangles.reserve(12);

	for (uint8_t i = 0; i < 12; i++)
		this->triangles.push_back(trs[i]);
}