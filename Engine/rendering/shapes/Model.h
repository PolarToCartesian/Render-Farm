#pragma once

#include "Triangle.h"
#include "../../other/Defines.h"
#include "../../logging/Logging.h"
#include "../../files/File.h"
#include "../../math/Vec3.h"
#include "../../other/pch.h"

struct Model {
	std::vector<Triangle> triangles;

	bool doRender = true;

	Model(const char* _filePath, const Vec3& _delataPosition = Vec3(0), const bool& _randomColors = false, 
		  const Color& _flatColor = Color(255), const Vec3& _centerOfRotation = Vec3(0), const Vec3& _rotation = Vec3(0));

	Model(const Triangle* _triangles, const unsigned int& _numTriangles);

	void applyFunctionToEachTriangle(const std::function<void(Triangle&)>& _function);

	void setRotation(const Vec3& _rotation);
	void setCenterOfRotation(const Vec3& _centerOfRotation);
	void translate(const Vec3& _deltaPosition);
	void rotate(const Vec3& _deltaRotation);
};