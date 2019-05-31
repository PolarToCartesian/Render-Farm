#pragma once

#include "../other/pch.h"

#include "../other/Defines.h"
#include "../util/Logging.h"

class File {
	private:
		FILE* filePtr = nullptr;

		bool isFileOpen = false;
		bool doLogOpeningAndClosing = true;

		std::string filename;
		std::string permissions;

	public:
		File(const std::string& _filename, const std::string& _permissions = FILE_READ_WRITE, const bool& _doLogOpeningAndClosing = true);
		~File();

		void open(const std::string& _filename, const std::string& _permissions = FILE_READ_WRITE);

		void readLineByLine(const std::function<void(const std::string&)>& _lambda) const;

		void writeNoVerif(const std::string& _content);
		void write(const std::string& _content);

		bool isOpen()   const;
		bool isClosed() const;

		void close();
};