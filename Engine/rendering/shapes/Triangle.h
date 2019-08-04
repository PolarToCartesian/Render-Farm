#pragma once

#include "../../files/Image.h"
#include "../../math/Vertex.h"
#include "../../other/pch.h"

struct Triangle {
	Vertex vertices[3];
	Vec3 rotation;
	Vec3 rotationMidPoint;

	Triangle();
	Triangle(const Vertex _vertices[3]);
	Triangle(const Vertex _vertices[3], const Vec3& _rotationMidPoint = Vec3(), const Vec3& _rotation = Vec3());

	void applyFunctionToEachVertex(const std::function<void(Vertex&)>& _function);

	void translate(const Vec3& _deltaPosition);

	static Vec3 getSurfaceNormal(const Vec3 _points[3]);
};