#pragma once

#include "../other/Defines.h"
#include "../other/pch.h"

namespace MATH {
	unsigned int randomInt(const unsigned int _min, const unsigned int _max);

	float degToRad(const float _deg);

	float constrain(const float _x, const float _min, const float _max);
};