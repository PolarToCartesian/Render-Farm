#pragma once

#include "../other/Defines.h"

#include <iostream>
#include <mutex>

namespace CMD {
	namespace {
		std::mutex printMutex;
	}

	/*
	* Prints a message or an object to the console with a given color
	*
	* The "_bypassMutex" parameter if set to true doesn't use the "printMutex"
	* which insures that only one message is being printed at the same time
	* so that the message doesn't get "tangled" with other messages.
	* This can only occur in a multi-threaded program.
	*/

	template<typename T>
	void print(const T& _message, const LOG_TYPE& _type = LOG_TYPE::normal, const bool _bypassMutex = false) {
		if (!_bypassMutex) {
			std::lock_guard<std::mutex> lock(printMutex);
		}

		#ifdef _WIN32
			WORD color_atrib = AINSI_COLORS.at("WHITE");

			switch (_type)
			{
				case LOG_TYPE::success:
					color_atrib = AINSI_COLORS.at("GREEN");
					break;
				case LOG_TYPE::warning:
					color_atrib = AINSI_COLORS.at("ORANGE");
					break;
				case LOG_TYPE::error:
					color_atrib = AINSI_COLORS.at("RED");
					break;
			}

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_atrib);

			std::cout << _message;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), AINSI_COLORS.at("WHITE"));
		#else
			const char* prepended_coloring = "";

			switch (_type)
			{
				case LOG_TYPE::success:
					prepended_coloring = AINSI_COLORS.at("GREEN");
					break;
				case LOG_TYPE::warning:
					prepended_coloring = AINSI_COLORS.at("ORANGE");
					break;
				case LOG_TYPE::error:
					prepended_coloring = AINSI_COLORS.at("RED");
					break;
			}

			std::cout << prepended_coloring << _message << AINSI_COLORS.at("RESET");
		#endif	
	}

	// Prints a message or an object to the console with a given color and appends a newline character
	
	template<typename T>
	void println(const T& _message, const LOG_TYPE& _type = LOG_TYPE::normal) {
		/*
		 * In order to print a message or an object appended with a newline character,
		 * we have the call two times the print function :
		 *   1) Print the actual message or object
		 *   2) Print the newline character
		 * to print it, we have to lock the "printMutex" before calling two times the print function
		 * because something could be printed in between the two print calls. In order to do this,
		 * we bypass the "printMutex" when calling the print functions since we have already locked it.
		*/

		std::lock_guard<std::mutex> lock(printMutex);

		print(_message, _type, true);
		print("\n", LOG_TYPE::normal, true);
	}
};