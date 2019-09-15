#pragma once

#include "Triangle.h"
#include "../../math/Vec3.h"
#include "../../files/File.h"
#include "../../cmd/Logging.h"
#include "../../other/Defines.h"

struct Model {
	std::vector<Triangle> triangles;

	Model();

	Model(const std::string& _filePath, const std::string& _material, const Vec3& _delataPosition = Vec3(0), const Vec3& _centerOfRotation = Vec3(0), const Vec3& _rotation = Vec3(0));

	Model(const Triangle* _triangles, const uint64_t _numTriangles);

	void applyFunctionToEachTriangle(const std::function<void(Triangle&)>& _function);

	void rotate(const Vec3& _deltaRotation);
	void setRotation(const Vec3& _rotation);

	void setCenterOfRotation(const Vec3& _centerOfRotation);

	void translate(const Vec3& _deltaPosition);
};