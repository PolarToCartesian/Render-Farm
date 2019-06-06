#pragma once

#include "pch.h"

/// Can Be Changed By User

// The higher, the more memory and threads needed at once
// Putting it higher doesn't always make the rendering/writing faster and can cause errors
// Putting it lower  can also make it very slow
// You can and should try different values to see what suites you the best depending on your hardware
constexpr unsigned int RENDERS_AND_WRITES_PER_CYCLE = 5;

/// Do not modify past this comment

constexpr double PI  = 3.14159265359;
constexpr double TAU = 2 * PI;

// File

constexpr unsigned int FILE_LINE_BUFFER_SIZE = 255;

constexpr const char* FILE_READ  = "r";
constexpr const char* FILE_WRITE = "w";
constexpr const char* FILE_READ_WRITE = "r+";

// Logging

enum class LOG_TYPE : unsigned char {
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

// Video

constexpr const char* VIDEO_ENCODING_PYTHON_SOURCE_CODE[] = {
	"# python FramesToVideo.py fps",
	"# exemple: python FramesToVideo.py 30 __outputFileName__ __kjkqjf.ppm __qskdfjq__.ppm",
	"",
	"import os, cv2, sys",
	"",
	"fps = int(sys.argv[1])",
	"output_file_name = sys.argv[2]",
	"file_names = sys.argv[3:len(sys.argv)]",
	"",
	"reference = file_names[0]",
	"if os.path.isfile(reference) :",
	"	height, width, channels = cv2.imread(reference).shape",
	"",
	"	outputFile = cv2.VideoWriter(output_file_name, cv2.VideoWriter_fourcc('M','J','P','G'), fps, (width, height))",
	"",
	"	for filename in file_names:",
	"		outputFile.write(cv2.imread(filename))",
	"",
	"	outputFile.release()",
	"	exit(0)",
	"else:",
	"	exit(1)"
};