#pragma once

#include "../other/Defines.h"
#include "../other/pch.h"

namespace CMD {
	namespace {
		std::mutex printMutex;
	}

	template<typename T>
	void print(const T& _message, const LOG_TYPE& _type = LOG_TYPE::normal, const bool _bypassMutex = false) {
			
		if (!_bypassMutex) {
			// We use a mutex so that if multiple threads call print, the message doesn't get messed up!
			std::lock_guard<std::mutex> lock(printMutex);
		}

		#ifdef _WIN32
			WORD attrib = AINSI_COLORS.at("WHITE");

			switch (_type) {
				case LOG_TYPE::success:
					attrib = AINSI_COLORS.at("GREEN");
					break;
				case LOG_TYPE::warning:
					attrib = AINSI_COLORS.at("ORANGE");
					break;
				case LOG_TYPE::error:
					attrib = AINSI_COLORS.at("RED");
					break;
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attrib);

			std::cout << _message;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), AINSI_COLORS.at("WHITE"));
		#else
			// Add Coloring
			const char* prepend = "";

			switch (_type) {
				case LOG_TYPE::success:
					prepend = AINSI_COLORS.at("GREEN");
					break;
				case LOG_TYPE::warning:
					prepend = AINSI_COLORS.at("ORANGE");
					break;
				case LOG_TYPE::error:
					prepend = AINSI_COLORS.at("RED");
					break;
			}

			std::cout << prepend << _message << AINSI_COLORS.at("RESET");
		#endif	
	}

	template<typename T>
	void println(const T& _message, const LOG_TYPE& _type = LOG_TYPE::normal) {
		// In order to add \n at the end of the message, we have to call two times the print function
		// Once for the message and another time for the newline character
		// This happens because you can't always write : print(_message + std::string("\n"), _type);
		// So we lock here the printing mutex and call the print function with true for the argument : "_bypassMutex" since we already blocked it
		std::lock_guard<std::mutex> lock(printMutex);

		print(_message, _type, true);
		print("\n", LOG_TYPE::normal, true);
	}
};