#pragma once

#include <experimental/filesystem>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>
#include <fcntl.h>
#include <vector>
#include <thread>
#include <cmath>
#include <array>
#include <mutex>
#include <io.h>

#ifdef _WIN32
	#define NOMINMAX

	#include <Windows.h>
#endif