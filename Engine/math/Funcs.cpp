#include "Funcs.h"

unsigned int MATH::randomInt(const unsigned int _min, const unsigned int _max)
{
	return _min + (rand() % static_cast<int>(_max - _min + 1));
}

float MATH::degToRad(const float _deg) { return (_deg * 180.f / PI); }

float MATH::constrain(const float _x, const float _min, const float _max)
{
	return (_x < _min) ? _min : ((_x > _max) ? _max : _x);
}