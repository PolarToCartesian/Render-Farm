#pragma once

#include "../../files/media/Image.h"

#include <array>

struct Material {
	float reflectivity;
	float shineDamper;

	std::array<Color<>, 3> vertexColors;
	std::string textureName = "";

	bool isTextured = false;

	Material(const float _reflectivity, const float _shineDamper, const std::array<Color<>, 3>& _colors);
	Material(const float _reflectivity, const float _shineDamper, const std::string& _textureName);
};