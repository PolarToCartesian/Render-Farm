#pragma once

#include "../../files/Image.h"
#include "../../math/Vertex.h"
#include "../../math/Mat4x4.h"
#include "../objects/Camera.h"

struct Triangle {
	Vertex vertices[3];
	Vec3 rotation;
	Vec3 rotationMidPoint;

	std::string material;

	bool isSmoothed = false; // use phong

	Triangle();
	Triangle(const Vertex _vertices[3], const std::string& _material, const bool _isSmoothed = false);
	Triangle(const Vertex _vertices[3], const std::string& _material, const Vec3& _rotationMidPoint = Vec3(), const Vec3& _rotation = Vec3(), const bool _isSmoothed = false);

	void applyFunctionToEachVertex(const std::function<void(Vertex&)>& _function);

	void translate(const Vec3& _deltaPosition);

	std::array<Vec3, 3> getRotatedVertices(const Camera& _cam) const;

	static Vec3 getSurfaceNormal(const std::array<Vec3, 3>& _points);
	static std::array<Vec3, 3> getTransformedVertices(const std::array<Vec3, 3>& _rotatedVertices, const Camera& _cam, const Mat4x4& _perspectiveMatrix, const uint16_t _width, const uint16_t _height);
};