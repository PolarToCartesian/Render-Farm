#pragma once

#include "../other/pch.h"

#include "../other/Defines.h"

namespace MATH {
	unsigned int randomInt(const unsigned int _min, const unsigned int _max);

	float degToRad(const float _deg);

	float constrain(const float _x, const float _min, const float _max);
};