#pragma once

#include "../cmd/Logging.h"
#include "../other/Defines.h"

#include <functional>

class File {
	private:
		FILE* filePtr = nullptr;

		bool isFileOpen = false;

		bool canWrite, canRead;

		std::string filename;
		std::string permissions;

	public:
		File(const std::string& _filename, const std::string& _permissions);
		~File();

		void open(const std::string& _filename, const std::string& _permissions);

		std::string read() const;
		char* readBytesNoVerif(const uint32_t _nBytes) const;
		void readLineByLine(const std::function<void(const std::string&, const unsigned int)>& _lambda) const;

		void writeNoVerif(const std::string& _content);
		void write(const std::string& _content);

		void writeBufferNoVerif(const void* _buff, const size_t _elementSize, size_t _elementCount);
		void writeBuffer(const void* _buff, const size_t _elementSize, size_t _elementCount);

		bool isOpen()   const;
		bool isClosed() const;

		bool isReadable() const;
		bool isWritable() const;

		void close();
};