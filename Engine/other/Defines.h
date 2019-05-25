#pragma once

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
constexpr TYPE PI = 3.14159265359;
constexpr TYPE TAU = 2 * PI;

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