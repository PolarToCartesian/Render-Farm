#include "File.h"

File::File(const std::string _filename, const std::string _permissions, const bool _doLogOpeningAndClosing) {
	this->doLogOpeningAndClosing = _doLogOpeningAndClosing;

	this->open(_filename, _permissions);
}

File::~File() {
	if (this->isFileOpen)
		this->close();
}

void File::open(const std::string _filename, const std::string _permissions) {
	if (!this->isFileOpen) this->close();

	this->filename = _filename;
	this->permissions = _permissions;

	#ifdef _WIN32
		this->isFileOpen = (fopen_s(&this->filePtr, _filename.c_str(), _permissions.c_str()) == 0);
	#else
		this->filePtr    = fopen(_filename.c_str(), _permissions.c_str());
		this->isFileOpen = (this->filePtr != NULL);
	#endif

	if (this->doLogOpeningAndClosing) {
		if (this->isFileOpen) {
			CMD::println("[FILE] The File \"" + std::string(_filename) + "\" Was Successfully Opened", LOG_TYPE::success);
		} else {
			perror("[FILE] Error While Opening");
		}
	}

	this->canRead  = (this->isFileOpen && (this->permissions == FILE_READ  || this->permissions == FILE_READ_WRITE));
	this->canWrite = (this->isFileOpen && (this->permissions == FILE_WRITE || this->permissions == FILE_READ_WRITE));
}

void File::close() {
	// Close the file only if the file is open
	if (this->isFileOpen) {
		fclose(this->filePtr);

		if (this->doLogOpeningAndClosing)
			CMD::println("[FILE] The File \"" + std::string(this->filename) + "\" Was Successfully Closed", LOG_TYPE::success);

		this->filePtr     = nullptr;
		this->filename    = "";
		this->permissions = "";
		this->isFileOpen  = false;
		this->canRead     = false;
		this->canWrite    = false;
	} else if (this->doLogOpeningAndClosing) {
		CMD::println("[FILE] The File \"" + std::string(this->filename) + "\" Could Not Be Closed Since It Is Already Closed", LOG_TYPE::warning);
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
			CMD::println("[FILE] The File \"" + std::string(this->filename) + "\" Can Only Be Read From Not Written To", LOG_TYPE::error);
		}
	} else {
		CMD::println("[FILE] The File \"" + std::string(this->filename) + "\" Can Not Be Written To Because It Is Closed", LOG_TYPE::error);
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
				CMD::println("[FILE] The File \"" + std::string(this->filename) + "\" Encountered An Memory Error While Reading", LOG_TYPE::error);
			}
		} else {
			CMD::println("[FILE] The File \"" + std::string(this->filename) + "\" Can Only Be Written To Not Read From", LOG_TYPE::error);
		}
	} else {
		CMD::println("[FILE] The File \"" + std::string(this->filename) + "\" Can Not Be Read From Because It Is Closed", LOG_TYPE::error);
	}

	return "";
}

void File::readLineByLine(const std::function<void(const std::string&, const unsigned int)>& _lambda) const {
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
			CMD::println("[FILE] The File \"" + std::string(this->filename) + "\" Can Only Be Written To Not Read From", LOG_TYPE::error);
		}
	} else {
		CMD::println("[FILE] The File \"" + std::string(this->filename) + "\" Can Not Be Read From Because It Is Closed", LOG_TYPE::error);
	}
}

bool File::isOpen()   const { return  this->isFileOpen; }
bool File::isClosed() const { return !this->isFileOpen; }

bool File::isReadable() const { return this->canRead;  }
bool File::isWritable() const { return this->canWrite; }