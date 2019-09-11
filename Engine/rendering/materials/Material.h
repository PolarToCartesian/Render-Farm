#pragma once

#include "../../files/Image.h"

#include <array>

struct Material {
	float reflectivity;
	float shineDamper;

	std::array<Color<>, 3> vertexColors;

	Material(const float _reflectivity, const float _shineDamper, const std::array<Color<>, 3>& _colors);
};