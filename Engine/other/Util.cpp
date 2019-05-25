#include "Util.h"
// Engine Namespace

namespace EN {
	namespace UTIL {
		std::vector<std::string> splitString(const std::string& _str, const char& _seperator) {
			unsigned int last = 0;

			std::vector<std::string> r;

			for (unsigned int current = 0; current < _str.size(); current++) {
				if (_str.at(current) == _seperator || current == _str.size() - 1) {
					r.push_back(_str.substr(last, current));

					last = current + 1;
				}
			}

			return r;
		}

		unsigned int randomInt(const unsigned int& _min, const unsigned int& _max) {
			return _min + (rand() % static_cast<int>(_max - _min + 1));
		}

		TYPE degToRad(const TYPE& _deg) { return (_deg * 180.f / PI); }

		TYPE constrain(const TYPE _x, const TYPE& _min, const TYPE& _max) {
			return (_x < _min) ? _min : ((_x > _max) ? _max : _x);
		}

		// True if successful
		bool openFile(FILE*& _file, const char* _filename, const char* _permissions) {
			#ifdef _WIN32
				return (fopen_s(&_file, _filename, _permissions) == 0);
			#else
				_file = fopen(_filename, _permissions);

				return (_file != NULL);
			#endif

			return false;
		}

		void print(const std::string& _message, const unsigned int& _label) {
			print(_message.c_str(), _label);
		}

		void print(const char* _message, const unsigned int& _label) {
			std::lock_guard<std::mutex> guard(sharedPrintingMutex);

			if (_label != 0) {
				const char* apend = ainsi_colors.at("RESET");
				const char* prepend = "";

				switch (_label) {
					case 1:
						prepend = ainsi_colors.at("GREEN");
						break;
					case 2:
						prepend = ainsi_colors.at("ORANGE");
						break;
					case 3:
						prepend = ainsi_colors.at("RED");
						break;
				}

				std::cout << prepend << _message << apend;
			}
			else {
				std::cout << _message;
			}
		}
	};
};