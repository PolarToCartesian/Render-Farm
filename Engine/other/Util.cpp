#include "Util.h"
// Engine Namespace

namespace EN {
	namespace UTIL {
		// Recreation of Split function in java
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

		double degToRad(const double& _deg) {
			return (_deg * 180.f / PI);
		}

		double constrain(const double _x, const double& _min, const double& _max) {
			return (_x < _min) ? _min : ((_x > _max) ? _max : _x);
		}

		// True if successful
		bool openFile(FILE *& _file, const char* _filename, const char* _permissions) {
			if (fopen_s(&_file, _filename, _permissions) == 0) return true;

			return false;
		}

		void syncPrint(const std::string& _message) {
			std::lock_guard<std::mutex> guard(sharedPrintingMutex);

			std::cout << _message;
		}
	};
};