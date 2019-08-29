#pragma once

#include <experimental/filesystem>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <iostream>
#include <optional>
#include <sstream>
#include <cstring>
#include <cassert>
#include <vector>
#include <thread>
#include <cmath>
#include <array>
#include <mutex>
#include <deque>

#ifdef _WIN32
	#define NOMINMAX

	#include <Windows.h>
#endif