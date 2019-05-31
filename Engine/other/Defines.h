#pragma once

#include "pch.h"

// Can Be Changed By User

// This value is used for math operations
// Double takes more memory but is faster than float on many systems because float has to be simulated
// Try float and see if it is any faster (note: It may show warnings)
typedef double TYPE;

// The higher, the more memory and threads needed at once
// Putting it higher doesn't always make the rendering/writing faster
// Putting it lower  can also make it very slow
// You can and should try different values to see what suites you the best
constexpr unsigned int RENDERS_AND_WRITES_PER_CYCLE = 5;

// Do not modify past this comment
constexpr TYPE PI  = 3.14159265359;
constexpr TYPE TAU = 2 * PI;

// File

constexpr unsigned int FILE_LINE_BUFFER_SIZE = 255;

constexpr const char* FILE_READ  = "r";
constexpr const char* FILE_WRITE = "w";
constexpr const char* FILE_READ_WRITE = "r+";

// Logging

const enum class LOG_TYPE : unsigned char {
	success, normal, warning, error
};

const std::unordered_map<const char*, const char*> AINSI_COLORS({
	{"GREEN",  "\x1B[32m"}, {"ORANGE", "\x1B[31;1m"},
	{"RED",    "\x1B[31m"}, {"RESET",  "\x1B[0m"   }
});

// Other

constexpr const char* PYTHON_VIDEO_WRITER_SOURCE_CODE_LINES[] = {
	"# python FramesToVideo.py fps",
	"# exemple: python FramesToVideo.py 30",
	"",
	"import os, cv2, sys",
	"",
	"fileName = './../frames/1.ppm'",
	"if os.path.isfile(fileName):",
	"	height, width, channels = cv2.imread(fileName).shape",
	"",
	"	outputFile = cv2.VideoWriter('./video.avi',cv2.VideoWriter_fourcc(*'DIVX'), int(sys.argv[1]), (width, height))",
	"",
	"	i = 1",
	"	while True:",
	"		fileName = './../frames/' + str(i) + '.ppm'",
	"		if os.path.isfile(fileName):",
	"			outputFile.write(cv2.imread(fileName))",
	"		else :",
	"			outputFile.release()",
	"			exit(0)",
	"",
	"		i = i + 1",
	"else:",
	"	exit(1)"
};