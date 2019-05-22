#pragma once

#include "Model.h"

// Model Struct

// Inspired By https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_olcEngine3D_Part4.cpp
Model::Model(const std::string& _filePath, const Vector3D& _delataPosition, const bool& _randomColors, const Color& _flatColor, const Vector3D& _centerOfRotation, const Vector3D& _rotation) {
	std::ifstream file(_filePath);

	if (file.is_open()) {
		std::vector<Vector3D> vertices;

		char junk;

		while (!file.eof()) {
			char currentLine[MAX_CHARACTERS_PER_LINE];

			file.getline(currentLine, MAX_CHARACTERS_PER_LINE);

			std::strstream currentLineStream;
			currentLineStream << currentLine;

			if (currentLine[1] == ' ') {
				switch (currentLine[0]) {
					case 'v':
						{
							double x = 0.f, y = 0.f, z = 0.f;

							currentLineStream >> junk >> x >> y >> z;

							vertices.emplace_back(x, y, z);
						}

						break;
					case 'f':
						{
							currentLineStream >> junk;

							Vector3D triangleVertices[3];
							Color triangleColors[3];

							for (unsigned char i = 0; i < 3; i++) {
								std::string lineArgument;

								currentLineStream >> lineArgument;

								unsigned int vertexIndex = std::stoi(EN::UTIL::splitString(lineArgument, '/')[0]) - 1;

								triangleVertices[i] = vertices[vertexIndex] + _delataPosition;

								if (_randomColors) {
									triangleColors[i] = Color(EN::UTIL::randomInt(0, 255), EN::UTIL::randomInt(0, 255), EN::UTIL::randomInt(0, 255));
								} else {
									triangleColors[i] = _flatColor;
								}
							}

							this->triangles.emplace_back(triangleVertices, triangleColors, _centerOfRotation, _rotation);
						}

						break;
				}
			}
		}
	} else {
		std::cout << "[ERROR] The File \"" << _filePath << "\" Was Unbale To Be Opened!" << std::endl;
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