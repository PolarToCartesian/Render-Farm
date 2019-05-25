#pragma once

#include <experimental/filesystem>
#include <functional>
#include <algorithm>
#include <strstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <thread>
#include <limits>
#include <memory>
#include <cmath>
#include <mutex>

#ifdef __unix
	#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
#endif