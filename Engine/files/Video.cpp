#include "Video.h"

Video::Video(const bool _doLogSaving) : doLogSaving(_doLogSaving) {}

void Video::addFrame(const std::string& _filename)
{
	this->frames.push_back(_filename);
}

void Video::save(const std::string& _filename, const uint16_t _fps)
{
	assert(this->frames.size() > 0);
	assert(_fps > 0);

	std::experimental::filesystem::create_directory("./temp/");

	File file("./temp/videoEncoder.py", FILE_WRITE, false);

	if (file.isOpen())
	{
		unsigned int nLines = sizeof(VIDEO_ENCODING_PYTHON_SOURCE_CODE) / sizeof(const char*);

		for (unsigned int i = 0; i < nLines; i++)
			file.writeNoVerif(VIDEO_ENCODING_PYTHON_SOURCE_CODE[i] + std::string("\n"));

		if (this->doLogSaving)
			CMD::println("[VIDEO] Added Python Script  (1/3)", LOG_TYPE::success);
	} else {
		if (this->doLogSaving)
			CMD::println("[ERROR] While Writing Python (1/3)", LOG_TYPE::error);
	}

	// Close file here so that the system can execute it
	file.close();

	if (this->doLogSaving)
		CMD::println("[VIDEO] Writing Video        (2/3)", LOG_TYPE::success);

	std::string arguments = std::to_string(_fps) + " " + _filename;

	std::for_each(this->frames.begin(), this->frames.end(), [&arguments](const std::string& _frameName) {
		arguments += " \"" + _frameName + "\"";
	});

	system(("python ./temp/videoEncoder.py " + arguments).c_str());

	if (this->doLogSaving)
		CMD::println("[VIDEO] Wrote Video          (3/3)", LOG_TYPE::success);
}