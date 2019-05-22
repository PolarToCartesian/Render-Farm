#pragma once

#include "../pch.h"

#include "Triangle.h"
#include "Vector3D.h"
#include "../other/Util.h"

struct Model {
	std::vector<Triangle> triangles;

	bool doRender = true;

	Model(const std::string& _filePath, const Vector3D& _delataPosition = Vector3D(0), const bool& _randomColors = false, 
		  const Color& _flatColor = Color(255), const Vector3D& _centerOfRotation = Vector3D(0), const Vector3D& _rotation = Vector3D(0));

	Model(const Triangle* _triangles, const unsigned int& _numTriangles);

	void applyFunctionToEachTriangle(const std::function<void(Triangle&)>& _function);

	void setRotation(const Vector3D& _rotation);
	void translate(const Vector3D& _deltaPosition);
	void rotate(const Vector3D& _deltaRotation);
};