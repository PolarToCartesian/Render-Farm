#include "Model.h"

// Model Struct

// Inspired By https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_olcEngine3D_Part4.cpp
Model::Model(const char* _filePath, const Vec3& _delataPosition, const bool _randomColors, const Color& _flatColor, const Vec3& _centerOfRotation, const Vec3& _rotation) {	
	File file(_filePath, FILE_READ, false);

	if (file.isOpen()) {
		std::string dataType, junk;

		std::vector<Vec3> vertices;

		file.readLineByLine([&](const std::string& _line, const unsigned int _lineNumber) {
			std::istringstream lineStream(_line);

			lineStream >> dataType;

			if (dataType == "v") {
				double x = 0.f, y = 0.f, z = 0.f;

				lineStream >> x >> y >> z;

				vertices.emplace_back(x + _delataPosition.x, y + _delataPosition.y, z + _delataPosition.z);
			} else if (dataType == "f") {
				unsigned int vertexIndex1 = 0, vertexIndex2 = 0, vertexIndex3 = 0;

				if (_line.find("/") == std::string::npos) {
					lineStream >> vertexIndex1 >> vertexIndex2 >> vertexIndex3;
				} else {
					lineStream >> vertexIndex1 >> junk >> vertexIndex2 >> junk >> vertexIndex3;
				}

				Vec3 triangleVertices[3] = { vertices[vertexIndex1 - 1], vertices[vertexIndex2 - 1], vertices[vertexIndex3 - 1] };

				Color triangleColors[3]  = { _flatColor, _flatColor, _flatColor };

				if (_randomColors) {
					using MATH::randomInt;
					for (unsigned char i = 0; i < 3; i++) {
						triangleColors[i] = Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255));
					}
				}

				triangles.emplace_back(triangleVertices, triangleColors, _centerOfRotation, _rotation);
			}
		});

		LOG::println("[READING] Read File \"" + std::string(_filePath) + "\" (" + std::to_string(this->triangles.size()) + " trianlges)", LOG_TYPE::success);
	} else {
		LOG::println("[ERROR] The File \"" + std::string(_filePath) + "\" Was Unbale To Be Opened!", LOG_TYPE::error);
	}

	file.close();
}

Model::Model(const Triangle* _triangles, const uint64_t _numTriangles) {
	triangles.reserve(_numTriangles);

	for (unsigned int i = 0; i < _numTriangles; i++) { triangles.push_back(_triangles[i]); }
}

void Model::applyFunctionToEachTriangle(const std::function<void(Triangle&)>& _function) {
	std::for_each(this->triangles.begin(), this->triangles.end(), _function);
}

void Model::setRotation(const Vec3& _rotation) {
	this->applyFunctionToEachTriangle([& _rotation](Triangle & _triangle) { _triangle.rotation = _rotation; });
}

void Model::setCenterOfRotation(const Vec3& _centerOfRotation) {
	this->applyFunctionToEachTriangle([& _centerOfRotation](Triangle& _triangle) { _triangle.rotationMidPoint = _centerOfRotation; });
}

void Model::translate(const Vec3& _deltaPosition) {
	this->applyFunctionToEachTriangle([& _deltaPosition](Triangle& _triangle) { _triangle.translate(_deltaPosition); });
}

void Model::rotate(const Vec3& _deltaRotation) {
	this->applyFunctionToEachTriangle([& _deltaRotation](Triangle& _triangle) { _triangle.rotation += _deltaRotation; });
}