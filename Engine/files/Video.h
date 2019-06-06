#pragma once

#include "File.h"

class Video {
	private:
		std::vector<std::string> frames;

		bool doLogSaving;

	public:
		Video(const bool& _doLogSaving = true);

		void addFrame(const std::string& _filename);
		void save(const std::string& _filename, const unsigned int& _fps);
};