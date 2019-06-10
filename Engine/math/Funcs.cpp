#include "Funcs.h"

unsigned int MATH::randomInt(const unsigned int& _min, const unsigned int& _max) {
	return _min + (rand() % static_cast<int>(_max - _min + 1));
}

double MATH::degToRad(const double& _deg) { return (_deg * 180.f / PI); }

double MATH::constrain(const double _x, const double& _min, const double& _max) {
	return (_x < _min) ? _min : ((_x > _max) ? _max : _x);
}