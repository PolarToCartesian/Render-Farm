#include "Math.h"

unsigned int EN::MATH::randomInt(const unsigned int& _min, const unsigned int& _max) {
	return _min + (rand() % static_cast<int>(_max - _min + 1));
}

TYPE EN::MATH::degToRad(const TYPE& _deg) { return (_deg * 180.f / PI); }

TYPE EN::MATH::constrain(const TYPE _x, const TYPE& _min, const TYPE& _max) {
	return (_x < _min) ? _min : ((_x > _max) ? _max : _x);
}