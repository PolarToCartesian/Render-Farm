#include "Box.h"

Box::Box(const Vec3& _center, const float sideLength, const std::string _materials[6], const Vec3& _rotationMidPoint, const Vec3& _rotation) {
	const float hsl = sideLength / 2.f;

	const Triangle trs[12] = {
		// Front Face
		Triangle(new Vertex[3] { Vertex(_center + Vec3(-hsl, +hsl, -hsl), Vec3(0, 0)), Vertex(_center + Vec3(+hsl, +hsl, -hsl), Vec3(1, 0)), Vertex(_center + Vec3(+hsl, -hsl, -hsl), Vec3(1, 1)) }, _materials[0], _rotationMidPoint, _rotation),
		Triangle(new Vertex[3] { Vertex(_center + Vec3(-hsl, +hsl, -hsl), Vec3(0, 0)), Vertex(_center + Vec3(+hsl, -hsl, -hsl), Vec3(1, 1)), Vertex(_center + Vec3(-hsl, -hsl, -hsl), Vec3(0, 1)) }, _materials[0], _rotationMidPoint, _rotation),

		// Back Face
		Triangle(new Vertex[3] { Vertex(_center + Vec3(+hsl, -hsl, +hsl), Vec3(1, 1)), Vertex(_center + Vec3(+hsl, +hsl, +hsl), Vec3(1, 0)), Vertex(_center + Vec3(-hsl, +hsl, +hsl), Vec3(0,0)) }, _materials[1], _rotationMidPoint, _rotation),
		Triangle(new Vertex[3] { Vertex(_center + Vec3(-hsl, -hsl, +hsl), Vec3(0, 1)), Vertex(_center + Vec3(+hsl, -hsl, +hsl), Vec3(1, 1)), Vertex(_center + Vec3(-hsl, +hsl, +hsl), Vec3(0,0)) }, _materials[1], _rotationMidPoint, _rotation),

		// Left Face
		Triangle(new Vertex[3] { Vertex(_center + Vec3(-hsl, +hsl, -hsl), Vec3(1, 0)), Vertex(_center + Vec3(-hsl, -hsl, -hsl), Vec3(1, 1)), Vertex(_center + Vec3(-hsl, -hsl, +hsl), Vec3(0, 1)) }, _materials[2], _rotationMidPoint, _rotation),
		Triangle(new Vertex[3] { Vertex(_center + Vec3(-hsl, +hsl, +hsl), Vec3(0, 0)), Vertex(_center + Vec3(-hsl, +hsl, -hsl), Vec3(1, 0)), Vertex(_center + Vec3(-hsl, -hsl, +hsl), Vec3(0, 1)) }, _materials[2], _rotationMidPoint, _rotation),

		// Right Face
		Triangle(new Vertex[3] { Vertex(_center + Vec3(+hsl, -hsl, +hsl), Vec3(1, 1)), Vertex(_center + Vec3(+hsl, -hsl, -hsl), Vec3(0, 1)), Vertex(_center + Vec3(+hsl, +hsl, -hsl), Vec3(0, 0)) }, _materials[3], _rotationMidPoint, _rotation),
		Triangle(new Vertex[3] { Vertex(_center + Vec3(+hsl, -hsl, +hsl), Vec3(1, 1)), Vertex(_center + Vec3(+hsl, +hsl, -hsl), Vec3(0, 0)), Vertex(_center + Vec3(+hsl, +hsl, +hsl), Vec3(1, 0)) }, _materials[3], _rotationMidPoint, _rotation),

		// Top Face
		Triangle(new Vertex[3] { Vertex(_center + Vec3(-hsl, +hsl, +hsl), Vec3(0, 0)), Vertex(_center + Vec3(+hsl, +hsl, +hsl), Vec3(1, 0)), Vertex(_center + Vec3(+hsl, +hsl, -hsl), Vec3(1, 1)) }, _materials[4], _rotationMidPoint, _rotation),
		Triangle(new Vertex[3] { Vertex(_center + Vec3(+hsl, +hsl, -hsl), Vec3(1, 1)), Vertex(_center + Vec3(-hsl, +hsl, -hsl), Vec3(0, 1)), Vertex(_center + Vec3(-hsl, +hsl, +hsl), Vec3(0, 0)) }, _materials[4], _rotationMidPoint, _rotation),

		// Bottom Face
		Triangle(new Vertex[3] { Vertex(_center + Vec3(+hsl, -hsl, -hsl), Vec3(1, 0)), Vertex(_center + Vec3(+hsl, -hsl, +hsl), Vec3(1, 1)), Vertex(_center + Vec3(-hsl, -hsl, +hsl), Vec3(0, 1)) }, _materials[5], _rotationMidPoint, _rotation),
		Triangle(new Vertex[3] { Vertex(_center + Vec3(-hsl, -hsl, +hsl), Vec3(0, 1)), Vertex(_center + Vec3(-hsl, -hsl, -hsl), Vec3(0, 0)), Vertex(_center + Vec3(+hsl, -hsl, -hsl), Vec3(1, 0)) }, _materials[5], _rotationMidPoint, _rotation),
	};

	this->triangles.reserve(12);

	for (uint8_t i = 0; i < 12; i++)
		this->triangles.push_back(trs[i]);
}

Box::Box(const Vec3& _center, const float sideLength, const std::string& _material, const Vec3& _rotationMidPoint, const Vec3& _rotation)
	: Box(_center, sideLength, new std::string[6] { _material, _material, _material, _material, _material, _material }, _rotationMidPoint, _rotation)
{}