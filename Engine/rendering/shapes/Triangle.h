#pragma once

#include "../../files/Image.h"
#include "../../math/Vec3.h"
#include "../../other/pch.h"

struct Triangle {
	Vec3 vertices[3]; // (0;0) is origin. Border is (-1;1) in x and y axies
	Vec3 rotation;    // In radians (x;y;z)
	Vec3 rotationMidPoint;
	Color colors[3];

	Triangle();
	Triangle(const Vec3 _vertices[3], const Color _colors[3]);
	Triangle(const Vec3 _vertices[3], const Color _colors[3], const Vec3 _rotationMidPoint = Vec3(), const Vec3 _rotation = Vec3());

	void applyFunctionToEachVertex(const std::function<void(Vec3&)>& _function);

	void translate(const Vec3& _deltaPosition);

	static Vec3 getSurfaceNormal(const Vec3 _vertices[3]);
};