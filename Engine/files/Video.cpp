#include "Video.h"

Video::Video() {}

void Video::addFrame(const std::string& _filename) {
	this->frames.push_back(_filename);
}

void Video::save(const std::string& _filename, const uint16_t _fps) {
	assert(this->frames.size() > 0);
	assert(_fps > 0);

	std::experimental::filesystem::create_directory("./temp/");
	try {
		File file("./temp/videoEncoder.py", FILE_WRITE);

		unsigned int nLines = sizeof(VIDEO_ENCODING_PYTHON_SOURCE_CODE) / sizeof(const char*);

		for (unsigned int i = 0; i < nLines; i++)
			file.writeNoVerif(VIDEO_ENCODING_PYTHON_SOURCE_CODE[i] + std::string("\n"));

		CMD::println("[VIDEO] Added Python Script  (1/3)", LOG_TYPE::success);

		file.close();
	} catch (const char* _error) {
		CMD::println("[ERROR] While Opening/Writing To \"./temp/videoEncoder.py\": " + std::string(_error), LOG_TYPE::error);
	}

	CMD::println("[VIDEO] Writing Video        (2/3)", LOG_TYPE::success);

	std::string arguments = std::to_string(_fps) + " " + _filename;

	std::for_each(this->frames.begin(), this->frames.end(), [&arguments](const std::string& _frameName) {
		arguments += " \"" + _frameName + "\"";
	});

	system(("python ./temp/videoEncoder.py " + arguments).c_str());

	CMD::println("[VIDEO] Wrote Video          (3/3)", LOG_TYPE::success);
}