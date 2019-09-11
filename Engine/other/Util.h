#pragma once

namespace UTIL {
	template <typename T>
	T extractValue(std::istringstream& _istrstr) {
		T val;
		_istrstr >> val;

		return val;
	}
};