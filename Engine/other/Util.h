#pragma once

#include "../pch.h"

#include "Defines.h"

// Engine Namespace

namespace EN {
	namespace UTIL {
		namespace {
			std::mutex sharedPrintingMutex;
		}

		std::vector<std::string> splitString(const std::string& _str, const char& _seperator);

		unsigned int randomInt(const unsigned int& _min, const unsigned int& _max);

		double degToRad(const double& _deg);

		double constrain(const double _x, const double& _min, const double& _max);
	
		// True if successful
		bool openFile(FILE*& _file, const char* _filename, const char* _permissions);

		void syncPrint(const std::string& _message);
	};
};