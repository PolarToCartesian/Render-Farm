#include "Triangle.h"

// Triangle Class

Triangle::Triangle() {}

Triangle::Triangle(const Vertex _vertices[3], const std::string& _material, const bool _isSmoothed) : isSmoothed(_isSmoothed), material(_material) {
	std::memcpy(this->vertices, _vertices, 3 * sizeof(Vertex));

	for (uint8_t i = 0; i < 3; i++)
		this->rotationMidPoint += this->vertices[i].position;

	this->rotationMidPoint /= 3;
}

Triangle::Triangle(const Vertex _vertices[3], const std::string& _material, const Vec3& _rotationMidPoint, const Vec3& _rotation, const bool _isSmoothed) : isSmoothed(_isSmoothed), material(_material), rotationMidPoint(_rotationMidPoint), rotation(_rotation) {
	std::memcpy(this->vertices, _vertices, 3 * sizeof(Vertex));
}

void Triangle::applyFunctionToEachVertex(const std::function<void(Vertex&)>& _function) {
	for (uint8_t i = 0; i < 3; i++) { _function(this->vertices[i]); }
}

void Triangle::translate(const Vec3& _deltaPosition) {
	this->applyFunctionToEachVertex([&_deltaPosition](Vertex& _vertex) { _vertex.position += _deltaPosition; });
	this->rotationMidPoint += _deltaPosition;
}

void Triangle::setTextureCoordinates(const Vec3 _textureCoord[3]) {
	for (uint8_t i = 0; i < 3; i++)
		this->vertices[i].textureCoord = _textureCoord[i];
}

std::array<Vec3, 3> Triangle::getRotatedVertices(const Camera* _cam) const {
	std::array<Vec3, 3> rotatedVertices{
		this->vertices[0].position + Vec3(0, 0, 0, 1),
		this->vertices[1].position + Vec3(0, 0, 0, 1),
		this->vertices[2].position + Vec3(0, 0, 0, 1)
	};

	const Mat4x4 triangleRotationMatrix = Mat4x4::getRotationMatrix(this->rotation.x, this->rotation.y, this->rotation.z);

	// For Every Vertex
	for (uint8_t v = 0; v < 3; v++) {
		// Rotate Triangle Around Point
		rotatedVertices[v] -= this->rotationMidPoint;
		rotatedVertices[v] *= triangleRotationMatrix;
		rotatedVertices[v] += this->rotationMidPoint;

		// Apply Camera Rotation
		_cam->applyRotation(rotatedVertices[v]);
	}

	return rotatedVertices;
}

Vec3 Triangle::getSurfaceNormal(const std::array<Vec3, 3>& _points) {
	//https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal

	const Vec3 U = _points[1] - _points[0];
	const Vec3 V = _points[2] - _points[0];

	return Vec3::normalize(Vec3(U.y * V.z - U.z * V.y, 
								U.z * V.x - U.x * V.z,
								U.x * V.y - U.y * V.x));
}


// For the basic Renderer (thx) https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
void Triangle::drawTriangle2D(const Image& _renderSurface, const Vec3& _a, const Vec3& _b, const Vec3& _c, const std::function<std::optional<Color<>>(const uint16_t _x, const uint16_t _y)>& _func) {
	Vec3 t0 = Vec3::intify(_a), t1 = Vec3::intify(_b), t2 = Vec3::intify(_c);

	if (t0.y == t1.y && t0.y == t2.y)
		return;

	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	const uint16_t total_height = static_cast<uint16_t>(t2.y - t0.y);

	for (uint16_t i = 0; i < total_height; i++) {
		const uint16_t y = static_cast<uint16_t>(t0.y + i);

		if (y < 0 || y > _renderSurface.getHeight() - 1)
			continue;

		const bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		const int segment_height = static_cast<int>(second_half ? t2.y - t1.y : t1.y - t0.y);

		const float alpha = i / static_cast<float>(total_height);
		const float beta = static_cast<float>(i - (second_half ? t1.y - t0.y : 0)) / segment_height;

		Vec3 A = t0 + (t2 - t0) * alpha;
		Vec3 B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;

		if (A.x > B.x)
			std::swap(A, B);

		// Limit X Between (0 and width - 1) in the loop to avoid trying to set a pixel out of the screen buffer and to maximize performance
		for (int j = static_cast<int>((A.x < 0) ? 0 : A.x); j <= static_cast<int>((B.x > _renderSurface.getWidth() - 2) ? _renderSurface.getWidth() - 2 : B.x); j++) {
			const uint16_t x = j;

			const uint32_t pixelIndex = _renderSurface.getIndex(x, y);

			std::optional<Color<>> pixelColor = _func(x, y);

			if (pixelColor.has_value())
				_renderSurface.colorBuffer[pixelIndex] = pixelColor.value();
		}
	}
}


std::array<Vec3, 3> Triangle::getTransformedVertices(const std::array<Vec3, 3>& _rotatedVertices, const Camera* _cam, const Mat4x4& _perspectiveMatrix, const uint16_t _width, const uint16_t _height) {
	std::array<Vec3, 3> transformedVertices = _rotatedVertices;

	// For Every Vertex
	for (uint8_t v = 0; v < 3; v++) {
		// Camera Translation
		transformedVertices[v] -= _cam->position;

		// Apply Perspective
		transformedVertices[v] = transformedVertices[v] * _perspectiveMatrix;

		if (transformedVertices[v].w < 0)
			continue;

		if (transformedVertices[v].w > 0)
			transformedVertices[v] /= transformedVertices[v].w;

		// -1 to 1    to    0-width
		transformedVertices[v].x = ((transformedVertices[v].x - 1.f) / -2.f) * _width;
		// 1  to -1   to    height-0
		transformedVertices[v].y = ((transformedVertices[v].y + 1.f) / +2.f) * _height;
	}

	return transformedVertices;
}