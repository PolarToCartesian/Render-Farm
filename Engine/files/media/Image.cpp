#include "Image.h"

// Image Class

Image::Image() {}

Image::Image(const Image& _img)
	: imageWidth(_img.imageWidth), imageHeight(_img.imageHeight), nPixels(_img.nPixels)
{
	this->colorBuffer = new Color<>[this->nPixels];

	std::memcpy( this->colorBuffer, _img.colorBuffer, this->nPixels * sizeof(Color<>) );
}
Image::Image(const std::string& _filename) {
	File file(_filename.c_str(), "rb");

	if (file.isReadable()) {
		// Read Header
		float maxColorVal = 0; // Unused
		std::string magicNumber;

		std::istringstream stream(file.read());
		stream >> magicNumber >> this->imageWidth >> this->imageHeight >> maxColorVal;

		file.close();
		this->nPixels     = this->imageWidth * this->imageHeight;
		this->colorBuffer = new Color<>[this->nPixels];

		const uint64_t nSubPixels = this->nPixels * 3;

		// Parse Image Data
		if (magicNumber == "P6") {
			FILE* fp;
			bool isFileOpen;

			#ifdef _WIN32
				isFileOpen = (fopen_s(&fp, _filename.c_str(), "rb") == 0);
			#else
				fp = fopen(_filename.c_str(), "rb");
				isFileOpen = (fp != NULL);
			#endif

			if (isFileOpen) {
				uint8_t* header[15];
				fread(header, 1, 15, fp);
				fread(this->colorBuffer, 1, nSubPixels, fp);

				fclose(fp);
			} else {
				perror("Error while reading image file");
			}
		} else {
			CMD::println("File Type Is Not Supported (Only P6)!", LOG_TYPE::error);
		}
	} else {
		perror("Error while reading image file");
	}
}

Image::Image(const unsigned int _imageWidth, const unsigned int _imageHeight, const Color<>& _backgroundColor) : imageWidth(_imageWidth), imageHeight(_imageHeight) {
	this->nPixels = this->imageWidth * this->imageHeight;

	this->colorBuffer = new Color<>[this->nPixels];
	std::fill_n(this->colorBuffer, this->nPixels, _backgroundColor);
}

 uint32_t Image::getIndex(const uint16_t _x, const uint16_t _y) const { return _y * this->imageWidth + _x; }

 uint16_t Image::getWidth()  const { return this->imageWidth; }
 uint16_t Image::getHeight() const { return this->imageHeight; }

 void Image::fill(const Color<>& _bgColor) const {
	 std::fill_n(this->colorBuffer, this->imageWidth * this->imageHeight, _bgColor);
 }

 void Image::set(const uint16_t _x, const uint16_t _y, const Color<>& _c) {
	this->colorBuffer[_y * this->imageWidth + _x] = _c;
}

 Color<> Image::sample(const uint16_t _x, const uint16_t _y) const {
	return this->colorBuffer[_y * this->imageWidth + _x];
}

void Image::writeToDisk(const std::string& _fileName) const {
	try {
		File file(_fileName, "wb");

		// Image Header
		const std::string header = "P6\n" + std::to_string(this->imageWidth) + " " + std::to_string(this->imageHeight) + " 255\n";

		file.writeNoVerif(header);

		// Image Data
		file.writeBufferNoVerif(this->colorBuffer, 1, this->nPixels * 3);

	} catch (const char* _error) {
		CMD::println("[ERROR] While Writing To \"" + _fileName + "\": " + std::string(_error), LOG_TYPE::error);
	}
}

void Image::resize(const uint16_t _width, const uint16_t _height) {
	uint32_t newNPixels = _width * _height;
	Color<>* newBuff = new Color<>[newNPixels];

	for (uint16_t x = 0; x < _width; x++)
		for (uint16_t y = 0; y < _height; y++)
			newBuff[y * _width + x] = this->sample(static_cast<uint16_t>(x / (float) _width  * this->imageWidth), 
												   static_cast<uint16_t>(y / (float) _height * this->imageHeight));

	delete[] this->colorBuffer;
	this->colorBuffer = newBuff;

	this->nPixels = newNPixels;
	this->imageWidth = _width;
	this->imageHeight = _height;
}

Image::~Image() {
	delete[] this->colorBuffer;
}