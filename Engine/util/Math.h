#pragma once

#include "../other/pch.h"

#include "../other/Defines.h"

namespace EN {
	namespace MATH {
		unsigned int randomInt(const unsigned int& _min, const unsigned int& _max);

		TYPE degToRad(const TYPE& _deg);

		TYPE constrain(const TYPE _x, const TYPE& _min, const TYPE& _max);
	};
};