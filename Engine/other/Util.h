#pragma once

namespace UTIL {
	template <typename T>
	T constrain(const T& _x, const T& _min, const T& _max) {
		return (_x < _min) ? _min : ((_x > _max) ? _max : _x);
	}

	template <typename T>
	T extractValue(std::istringstream& _istrstr) {
		T val;
		_istrstr >> val;

		return val;
	}
};