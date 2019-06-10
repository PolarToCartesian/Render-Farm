#pragma once

#include "../other/Defines.h"
#include "../other/pch.h"

namespace MATH {
	unsigned int randomInt(const unsigned int& _min, const unsigned int& _max);

	double degToRad(const double& _deg);

	double constrain(const double _x, const double& _min, const double& _max);
};