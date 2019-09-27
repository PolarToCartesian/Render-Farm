#pragma once

#include "../cmd/Logging.h"

#include <chrono>

struct Timer {
	std::string str;
	std::chrono::system_clock::time_point start;

	Timer(const std::string& _str);
	~Timer();
};