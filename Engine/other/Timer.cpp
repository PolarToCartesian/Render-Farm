#include "Timer.h"

Timer::Timer(const std::string& _str) : str(_str) {
	this->start = std::chrono::system_clock::now();
}

Timer::~Timer() {
	const auto end     = std::chrono::system_clock::now();
	const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::lock_guard<std::mutex> lock(CMD::printMutex);

	std::cout << this->str << elapsed.count() << "ms\n";
}