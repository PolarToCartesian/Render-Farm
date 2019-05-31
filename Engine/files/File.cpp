#include "File.h"

File::File(const std::string& _filename, const std::string& _permissions, const bool& _doLogOpeningAndClosing) {
	this->doLogOpeningAndClosing = _doLogOpeningAndClosing;

	this->open(_filename, _permissions);
}

File::~File() {
	if (this->isFileOpen) this->close();
}

void File::open(const std::string& _filename, const std::string& _permissions) {
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
			EN::LOG::println("[FILE] The File \"" + std::string(_filename) + std::string("\" Was Successfully Opened"), LOG_TYPE::success);
		}
		else {
			EN::LOG::println("[FILE] The File \"" + std::string(_filename) + std::string("\" Was Unable To Be Opened"), LOG_TYPE::error);
		}
	}
}

void File::close() {
	// Close the file only if the file is open
	if (this->isFileOpen) {
		fclose(this->filePtr);

		if (this->doLogOpeningAndClosing) {
			EN::LOG::println("[FILE] The File \"" + std::string(this->filename) + std::string("\" Was Successfully Closed"), LOG_TYPE::success);
		}

		this->filePtr = nullptr;
		this->filename = "";
		this->permissions = "";
		this->isFileOpen = false;
	} else if (this->doLogOpeningAndClosing) {
		EN::LOG::println("[FILE] The File \"" + std::string(this->filename) + std::string("\" Could Not Be Closed Since It Is Already Closed"), LOG_TYPE::warning);
	}
}

void File::writeNoVerif(const std::string& _content) {
	std::fprintf(this->filePtr, _content.c_str());
}

void File::write(const std::string& _content) {
	// Write to file if the file is open
	if (this->isFileOpen) {
		// Write if the file can be written to
		if (this->permissions != FILE_READ) {
			this->writeNoVerif(_content);
		}
		else {
			EN::LOG::println("[FILE] The File \"" + std::string(this->filename) + std::string("\" Can Only Be Read From Not Written To"), LOG_TYPE::error);
		}
	} else {
		EN::LOG::println("[FILE] The File \"" + std::string(this->filename) + std::string("\" Can Not Be Written To Because It Is Closed"), LOG_TYPE::error);
	}
}

void File::readLineByLine(const std::function<void(const std::string&)>& _lambda) const {
	// Read line by line if the file is open
	if (this->isFileOpen) {
		// Read File only if the file can be read from
		if (this->permissions != FILE_WRITE) {
			char line[FILE_LINE_BUFFER_SIZE];

			// For Every Line
			while (std::fgets(line, sizeof(line), this->filePtr) != NULL) {
				// Remove \n at the end of the line if there is one
				if (line[strlen(line) - 1] == '\n') {
					line[strlen(line) - 1] = ' ';
				}

				// Call the lambda function with the line as argument
				_lambda(line);
			}
		}
		else {
			EN::LOG::println("[FILE] The File \"" + std::string(this->filename) + std::string("\" Can Only Be Written To Not Read From"), LOG_TYPE::error);
		}
	}
	else {
		EN::LOG::println("[FILE] The File \"" + std::string(this->filename) + std::string("\" Can Not Be Read From Because It Is Closed"), LOG_TYPE::error);
	}
}

bool File::isOpen()   const { return this->isFileOpen; }
bool File::isClosed() const { return !this->isFileOpen; }