#include "Image.h"

// Image Class

Image::Image(const Image& _img)
	: imageWidth(_img.imageWidth), imageHeight(_img.imageHeight), nPixels(_img.nPixels)
{
	this->colorBuffer = (Color<>*) std::malloc(this->nPixels * sizeof(Color<>));

	std::memcpy( this->colorBuffer, _img.colorBuffer, this->nPixels * sizeof(Color<>) );
}

Image::Image(const unsigned int _imageWidth, const unsigned int _imageHeight, const Color<>& _backgroundColor) : imageWidth(_imageWidth), imageHeight(_imageHeight) {
	this->nPixels = this->imageWidth * this->imageHeight;

	this->colorBuffer = (Color<>*) std::malloc(this->nPixels * sizeof(Color<>));
	std::fill_n(this->colorBuffer, this->nPixels, _backgroundColor);
}

 uint32_t Image::getIndex(const uint16_t _x, const uint16_t _y) const { return _y * this->imageWidth + _x; }

 uint16_t Image::getWidth()  const { return this->imageWidth; }
 uint16_t Image::getHeight() const { return this->imageHeight; }

 void Image::fill(const Color<>& _bgColor) const {
	 std::fill_n(this->colorBuffer, this->imageWidth * this->imageHeight, _bgColor);
 }

 void Image::setColor(const uint16_t _x, const uint16_t _y, const Color<>& _c) {
	this->colorBuffer[_y * this->imageWidth + _x] = _c;
}

 Color<> Image::sample(const uint16_t _x, const uint16_t _y) const {
	return this->colorBuffer[_y * this->imageWidth + _x];
}

void Image::writeToDisk(const std::string& _fileName) const {
	try {
		File file(_fileName, "wb");

		const std::string header = "P6\n" + std::to_string(this->imageWidth) + " " + std::to_string(this->imageHeight) + " 255\n";

		file.writeNoVerif(header);

		const uint64_t fileBodySize = this->nPixels * 3;
		uint8_t* fileBody   = new uint8_t[fileBodySize];
		uint32_t pixelIndex = 0, filePixelIndex = 0;

		for (uint16_t y = 0; y < this->imageHeight; y++) {
			for (uint16_t x = 0; x < this->imageWidth; x++) {
				fileBody[filePixelIndex++] = this->colorBuffer[pixelIndex].r;
				fileBody[filePixelIndex++] = this->colorBuffer[pixelIndex].g;
				fileBody[filePixelIndex++] = this->colorBuffer[pixelIndex++].b;
			}
		}

		file.writeBufferNoVerif(fileBody, 1, fileBodySize);

		delete[] fileBody;
	} catch (const char* _error) {
		CMD::println("[ERROR] While Writing To \"" + _fileName + "\": " + std::string(_error), LOG_TYPE::error);
	}
}

Image::~Image() {
	std::free(this->colorBuffer);
}