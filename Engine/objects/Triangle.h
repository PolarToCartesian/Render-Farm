#pragma once

#include "../other/pch.h"

#include "Vector3D.h"

struct Triangle {
	Vector3D vertices[3]; // (0;0) is origin. Border is (-1;1) in x and y axies
	Vector3D rotation;    // In radians (x;y;z)
	Vector3D rotationMidPoint;
	Color colors[3];

	Triangle();
	Triangle(const Vector3D _vertices[3], const Color _colors[3]);
	Triangle(const Vector3D _vertices[3], const Color _colors[3], const Vector3D _rotationMidPoint = Vector3D(), const Vector3D _rotation = Vector3D());

	void applyFunctionToEachVertex(const std::function<void(Vector3D&)>& _function);

	void translate(const Vector3D& _deltaPosition);
};

// Engine Namespace

namespace EN {
	namespace TRIANGLE {
		Vector3D getSurfaceNormal(const Vector3D _vertices[3]);
	};
};