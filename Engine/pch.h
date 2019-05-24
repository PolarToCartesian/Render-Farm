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
#include <cmath>
#include <mutex>

//#include <memory>

#ifdef __unix
	#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
#endif