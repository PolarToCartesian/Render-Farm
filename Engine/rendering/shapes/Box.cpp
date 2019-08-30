#include "Box.h"

Box::Box(const Vec3& _center, const float sideLength, const uint64_t _materialIndex, const Vec3& _rotationMidPoint, const Vec3& _rotation, const float _reflectivity) {
	const float hsl = sideLength / 2.f;

	const Vertex vertices[8] = {
		Vertex(_center + Vec3(-hsl, +hsl, -hsl)), // 0
		Vertex(_center + Vec3(+hsl, +hsl, -hsl)), // 1
		Vertex(_center + Vec3(+hsl, -hsl, -hsl)), // 2
		Vertex(_center + Vec3(-hsl, -hsl, -hsl)), // 3
		Vertex(_center + Vec3(+hsl, -hsl, +hsl)), // 4
		Vertex(_center + Vec3(+hsl, +hsl, +hsl)), // 5
		Vertex(_center + Vec3(-hsl, +hsl, +hsl)), // 6
		Vertex(_center + Vec3(-hsl, -hsl, +hsl)), // 7
	};

	const Triangle trs[12] = {
		// Front Face
		Triangle(new Vertex[3] { vertices[0], vertices[1], vertices[2] }, _materialIndex, _rotationMidPoint, _rotation),
		Triangle(new Vertex[3] { vertices[0], vertices[2], vertices[3] }, _materialIndex, _rotationMidPoint, _rotation),

		// Back Face
		Triangle(new Vertex[3] { vertices[4], vertices[5], vertices[6] }, _materialIndex, _rotationMidPoint, _rotation),
		Triangle(new Vertex[3] { vertices[7], vertices[4], vertices[6] }, _materialIndex, _rotationMidPoint, _rotation),

		// Left Face
		Triangle(new Vertex[3] { vertices[0], vertices[3], vertices[7] }, _materialIndex, _rotationMidPoint, _rotation),
		Triangle(new Vertex[3] { vertices[6], vertices[0], vertices[7] }, _materialIndex, _rotationMidPoint, _rotation),
	
		// Right Face
		Triangle(new Vertex[3] { vertices[4], vertices[2], vertices[1] }, _materialIndex, _rotationMidPoint, _rotation),
		Triangle(new Vertex[3] { vertices[4], vertices[1], vertices[5] }, _materialIndex, _rotationMidPoint, _rotation),

		// Top Face
		Triangle(new Vertex[3] { vertices[6], vertices[5], vertices[1] }, _materialIndex, _rotationMidPoint, _rotation),
		Triangle(new Vertex[3] { vertices[1], vertices[0], vertices[6] }, _materialIndex, _rotationMidPoint, _rotation),

		// Bottom Face
		Triangle(new Vertex[3] { vertices[2], vertices[4], vertices[7] }, _materialIndex, _rotationMidPoint, _rotation),
		Triangle(new Vertex[3] { vertices[7], vertices[3], vertices[2] }, _materialIndex, _rotationMidPoint, _rotation),
	};

	this->triangles.reserve(12);

	for (uint8_t i = 0; i < 12; i++)
		this->triangles.push_back(trs[i]);
}