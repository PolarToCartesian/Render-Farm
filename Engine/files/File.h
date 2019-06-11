#pragma once

#include "../other/Defines.h"
#include "../logging/Logging.h"
#include "../other/pch.h"

class File {
	private:
		FILE* filePtr = nullptr;

		bool isFileOpen = false;
		bool doLogOpeningAndClosing = true;

		bool canWrite, canRead;

		std::string filename;
		std::string permissions;

	public:
		File(const std::string& _filename, const std::string& _permissions = FILE_READ_WRITE, const bool _doLogOpeningAndClosing = true);
		~File();

		void open(const std::string& _filename, const std::string& _permissions = FILE_READ_WRITE);

		std::string read() const;
		void readLineByLine(const std::function<void(const std::string&, const unsigned int)>& _lambda) const;

		void writeNoVerif(const std::string& _content);
		void write(const std::string& _content);

		bool isOpen()   const;
		bool isClosed() const;

		bool isReadable() const;
		bool isWritable() const;

		void close();
};