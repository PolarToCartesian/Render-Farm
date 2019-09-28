#include "Model.h"

// Inspired By https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_olcEngine3D_Part4.cpp

Model::Model() {}

Model::Model(const std::string& _filePath, const std::string& _material, const Vec3& _delataPosition, const Vec3& _centerOfRotation, const Vec3& _rotation) {
	try {
		File file(_filePath, "r");

		if (file.isOpen()) {
			std::string dataType, junk;

			std::vector<Vertex> vertices;

			file.readLineByLine([&](const std::string& _line, const unsigned int _lineNumber) {
				std::istringstream lineStream(_line);

				lineStream >> dataType;

				if (dataType == "v") {
					float x = 0.f, y = 0.f, z = 0.f;

					lineStream >> x >> y >> z;

					vertices.emplace_back(Vec3(x + _delataPosition.x, y + _delataPosition.y, z + _delataPosition.z));
				} else if (dataType == "f") {
					uint32_t vertexIndex1 = 0, vertexIndex2 = 0, vertexIndex3 = 0, vertexIndex4 = 0;

					if (_line.find("/") == std::string::npos) {
						lineStream >> vertexIndex1 >> vertexIndex2 >> vertexIndex3;

						// Check For 4 Component Face
						if (!lineStream.str().empty())
							lineStream >> vertexIndex4;
					} else {
						lineStream >> vertexIndex1 >> junk >> vertexIndex2 >> junk >> vertexIndex3;

						// Check For 4 Component Face
						if (!lineStream.str().empty())
							lineStream >> junk >> vertexIndex4;
					}

					// Triangle 1
					const Vertex triangle1Vertices[] = { vertices[vertexIndex1 - 1], vertices[vertexIndex2 - 1], vertices[vertexIndex3 - 1] };
					triangles.emplace_back(triangle1Vertices, _material, _centerOfRotation, _rotation, true);

					// Triangle 2
					if (vertexIndex4 > 0) {
						const Vertex triangle2Vertices[] = { vertices[vertexIndex1 - 1], vertices[vertexIndex3 - 1], vertices[vertexIndex4 - 1] };
						triangles.emplace_back(triangle2Vertices, _material, _centerOfRotation, _rotation, true);
					}
				}
				});

			CMD::println("[READING] Read File \"" + std::string(_filePath) + "\" (" + std::to_string(this->triangles.size()) + " trianlges)", LOG_TYPE::success);
		}
		else {
			CMD::println("[ERROR] The File \"" + std::string(_filePath) + "\" Was Unbale To Be Opened!", LOG_TYPE::error);
		}

		file.close();
	} catch (const char* _error) {
		CMD::println("[ERROR] While Opening/Writing To \"" + _filePath + "\": " + std::string(_error), LOG_TYPE::error);
	}
}

Model::Model(const Triangle* _triangles, const uint64_t _numTriangles) {
	triangles.reserve(_numTriangles);

	for (unsigned int i = 0; i < _numTriangles; i++) { triangles.push_back(_triangles[i]); }
}

void Model::applyFunctionToEachTriangle(const std::function<void(Triangle&)>& _function) {
	std::for_each(this->triangles.begin(), this->triangles.end(), _function);
}

void Model::rotate(const Vec3& _deltaRotation) {
	this->applyFunctionToEachTriangle([&_deltaRotation](Triangle& _triangle) { _triangle.rotation += _deltaRotation; });
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