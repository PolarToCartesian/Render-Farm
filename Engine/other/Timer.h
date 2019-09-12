#pragma once

#include "../cmd/Logging.h"

#include <chrono>

struct Timer {
	const char* str;
	std::chrono::system_clock::time_point start;

	Timer(const char* _str);
	~Timer();
};