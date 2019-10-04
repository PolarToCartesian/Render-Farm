#pragma once

#include "../cameras/Camera.h"
#include "../../math/Vertex.h"
#include "../../math/Mat4x4.h"
#include "../../files/media/Image.h"

#include <optional>

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

	void setTextureCoordinates(const Vec3 _textureCoord[3]);

	std::array<Vec3, 3> getRotatedVertices(const Camera* _cam) const;

	static Vec3 getSurfaceNormal(const std::array<Vec3, 3>& _points);
	static void drawTriangle2D(const Image& _renderSurface, const Vec3& _a, const Vec3& _b, const Vec3& _c, const std::function<std::optional<Color<>>(const uint16_t _x, const uint16_t _y)>& _func);
	static std::array<Vec3, 3> getTransformedVertices(const std::array<Vec3, 3>& _rotatedVertices, const Camera* _cam, const Mat4x4& _perspectiveMatrix, const uint16_t _width, const uint16_t _height);
};