#pragma once

// Math
constexpr double PI = 3.14159265359;
constexpr double TAU = 2 * PI;

// Other
constexpr unsigned int MAX_THREADS = 5;

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