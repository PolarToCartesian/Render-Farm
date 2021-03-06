#pragma once

#include <unordered_map>

#ifdef _WIN32
	#include <Windows.h>
#endif

/// Do not modify past this comment

constexpr float PI  = static_cast<float>(3.14159265359);
constexpr float TAU = static_cast<float>(2 * PI);

// File

constexpr uint16_t FILE_LINE_BUFFER_SIZE = 255;

// Logging

enum class LOG_TYPE : uint8_t {
	success, normal, warning, error
};

#ifdef _WIN32
	const std::unordered_map<const char*, const WORD> AINSI_COLORS({
		{"GREEN",  10}, {"ORANGE", 14},
		{"RED",    12}, {"WHITE",  15}
	});
#else
	const std::unordered_map<const char*, const char*> AINSI_COLORS({
		{"GREEN",  "\x1B[32m"}, {"ORANGE", "\x1B[31;1m"},
		{"RED",    "\x1B[31m"}, {"RESET",  "\x1B[0m"   }
	});
#endif // _WIN32

// Other

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
	"",
	"f = open(reference, \"r\")",
	"",
	"a = f.readline()",
	"dimLine = f.readline()",
	"height = int(dimLine.split(\" \")[1])",
	"width  = int(dimLine.split(\" \")[0])",
	"",
	"if os.path.isfile(reference) :",
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