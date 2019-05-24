#pragma once

#include "Model.h"

// Model Struct

// Inspired By https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_olcEngine3D_Part4.cpp
Model::Model(const char* _filePath, const Vector3D& _delataPosition, const bool& _randomColors, const Color& _flatColor, const Vector3D& _centerOfRotation, const Vector3D& _rotation) {	
	std::ifstream file(_filePath);

	if (file.is_open()) {
		std::string line, dataType, junk;

		std::vector<Vector3D> vertices;

		while (std::getline(file, line)) {
			if (!line.empty()) {
				std::istringstream lineStream(line);

				lineStream >> dataType;

				if (dataType == "v") {
					double x = 0.f, y = 0.f, z = 0.f;

					lineStream >> x >> y >> z;

					vertices.emplace_back(x, y, z);
				} else if (dataType == "f") {
					unsigned int vertexIndex1 = 0, vertexIndex2 = 0, vertexIndex3 = 0;

					if (line.find("//") == std::string::npos) {
						lineStream >> vertexIndex1 >> vertexIndex2 >> vertexIndex3;
					} else {
						lineStream >> vertexIndex1 >> junk >> vertexIndex2 >> junk >> vertexIndex3;
					}

					Vector3D triangleVertices[3] = {vertices[vertexIndex1 - 1], vertices[vertexIndex2 - 1], vertices[vertexIndex3 - 1]};

					Vector3D triangleColors[3]   = { _flatColor, _flatColor, _flatColor };

					if (_randomColors) {
						using EN::UTIL::randomInt;
						for (unsigned char i = 0; i < 3; i++) {
							triangleColors[i] = Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255));
						}
					}

					this->triangles.emplace_back(triangleVertices, triangleColors, _centerOfRotation, _rotation);
				}
			}
		}

		EN::UTIL::syncPrint("[READING] Read File \"" + std::string(_filePath) + "\" (" + std::to_string(this->triangles.size()) + " trianlges)\n");
	} else {
		EN::UTIL::syncPrint("[ERROR] The File \"" + std::string(_filePath) + "\" Was Unbale To Be Opened!\n");
	}

	file.close();
}

Model::Model(const Triangle* _triangles, const unsigned int& _numTriangles) {
	triangles.reserve(_numTriangles);

	for (unsigned int i = 0; i < _numTriangles; i++) { triangles.push_back(_triangles[i]); }
}

void Model::applyFunctionToEachTriangle(const std::function<void(Triangle&)>& _function) {
	std::for_each(this->triangles.begin(), this->triangles.end(), _function);
}

void Model::setRotation(const Vector3D& _rotation) {
	this->applyFunctionToEachTriangle([&_rotation](Triangle & _triangle) { _triangle.rotation = _rotation; });
}

void Model::translate(const Vector3D& _deltaPosition) {
	this->applyFunctionToEachTriangle([& _deltaPosition](Triangle& _triangle) { _triangle.translate(_deltaPosition); });
}

void Model::rotate(const Vector3D& _deltaRotation) {
	this->applyFunctionToEachTriangle([& _deltaRotation](Triangle& _triangle) { _triangle.rotation += _deltaRotation; });
}