#include "File.h"

File::File(const std::string& _filename, const std::string& _permissions) {
	this->open(_filename, _permissions);
}

File::~File() {
	if (this->isFileOpen)
		this->close();
}

void File::open(const std::string& _filename, const std::string& _permissions) {
	if (!this->isFileOpen) this->close();

	this->filename = _filename;
	this->permissions = _permissions;

	#ifdef _WIN32
		this->isFileOpen = (fopen_s(&this->filePtr, _filename.c_str(), _permissions.c_str()) == 0);
	#else
		this->filePtr    = fopen(_filename.c_str(), _permissions.c_str());
		this->isFileOpen = (this->filePtr != NULL);
	#endif

	if (!this->isFileOpen) {
		std::perror("Failure while opening file: ");
		throw 0;
	}

	this->canRead  = (this->permissions.find("r") != std::string::npos);
	this->canWrite = (this->permissions.find("w") != std::string::npos);
}

void File::close() {
	// Close the file only if the file is open
	if (this->isFileOpen) {
		fclose(this->filePtr);

		this->filePtr     = nullptr;
		this->filename    = "";
		this->permissions = "";
		this->isFileOpen  = false;
		this->canRead     = false;
		this->canWrite    = false;
	}
}

void File::writeNoVerif(const std::string& _content) {
	std::fprintf(this->filePtr, _content.c_str());
}

void File::write(const std::string& _content) {
	// Write to file if the file is open
	if (this->isFileOpen) {
		// Write if the file can be written to
		if (this->canWrite) {
			this->writeNoVerif(_content);
		} else {
			throw "NOT_WRITABLE";
		}
	} else {
		throw "CLOSED";
	}
}

void File::writeBufferNoVerif(const void* _buff, const size_t _elementSize, size_t _elementCount) {
	std::fwrite(_buff, _elementSize, _elementCount, this->filePtr);
}

void File::writeBuffer(const void* _buff, const size_t _elementSize, size_t _elementCount) {
	// Write to file if the file is open
	if (this->isFileOpen) {
		// Write if the file can be written to
		if (this->canWrite) {
			this->writeBufferNoVerif(_buff, _elementSize, _elementCount);
		}
		else {
			throw "NOT_WRITABLE";
		}
	}
	else {
		throw "CLOSED";
	}
}

std::string File::read() const {
	// Read If the file is open
	if (this->isFileOpen) {
		// Read File only if the file can be read from
		if (this->canRead) {
			// Thanks To : http://www.fundza.com/c4serious/fileIO_reading_all/

			fseek(this->filePtr, 0L, SEEK_END);
			long bytesInFile = ftell(this->filePtr);

			fseek(this->filePtr, 0L, SEEK_SET);

			char* charBuffer = (char*) std::calloc(bytesInFile, sizeof(char));

			if (charBuffer != NULL) {
				fread(charBuffer, sizeof(char), bytesInFile, this->filePtr);

				return std::string(charBuffer);
			} else {
				throw "MEMORY_ERROR";
			}
		} else {
			throw "NOT_READABLE";
		}
	} else {
		throw "CLOSED";
	}

	return "";
}

char* File::readBytesNoVerif(const uint32_t _nBytes) const {
	char* buff = (char*)std::malloc(_nBytes);

	if (buff != nullptr) {
		fread(buff, 1, _nBytes, this->filePtr);

		return buff;
	}

	return nullptr;
}

void File::readLineByLine(const std::function<void(const std::string&, const uint32_t)>& _lambda) const {
	// Read line by line if the file is open
	if (this->isFileOpen) {
		// Read File only if the file can be read from
		if (this->canRead) {
			unsigned int lineNumber = 1;
			char line[FILE_LINE_BUFFER_SIZE];

			// For Every Line
			while (std::fgets(line, sizeof(line), this->filePtr) != NULL) {
				// Remove \n at the end of the line if there is one
				if (line[strlen(line) - 1] == '\n')
					line[strlen(line) - 1] = ' ';

				// Call the lambda function with the line as argument
				_lambda(line, lineNumber);
				lineNumber++;
			}
		} else {
			throw "NOT_WRITABLE";
		}
	} else {
		throw "CLOSED";
	}
}

bool File::isOpen()   const { return  this->isFileOpen; }
bool File::isClosed() const { return !this->isFileOpen; }

bool File::isReadable() const { return this->canRead;  }
bool File::isWritable() const { return this->canWrite; }