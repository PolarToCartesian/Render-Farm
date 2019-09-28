#pragma once

#include "../File.h"

#include <experimental/filesystem>
#include <cassert>
#include <string>

class Video {
	private:
		std::vector<std::string> frames;

	public:
		Video();

		void addFrame(const std::string& _filename);
		void save(const std::string& _filename, const uint16_t _fps);
};