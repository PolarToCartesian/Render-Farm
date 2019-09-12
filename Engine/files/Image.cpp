#include "Image.h"

// Image Class

Image::Image(const Image& _img)
	: imageWidth(_img.imageWidth), imageHeight(_img.imageHeight), nPixels(_img.nPixels)
{
	this->colorBuffer = std::make_unique<Color<>[]>(this->nPixels);

	std::memcpy(this->colorBuffer.get(), _img.colorBuffer.get(), this->nPixels * sizeof(Color<>));
}

Image::Image(const std::string& _filename) {
	try {
		File file(_filename, "r");

		uint16_t maxColor = 0;

		file.readLineByLine([this, &_filename, &maxColor](const std::string& _line, const unsigned int _lineNumber) {
			std::istringstream lineStream(_line);

			if (_lineNumber == 1) {
				const std::string ppmType = UTIL::extractValue<std::string>(lineStream);

				if (ppmType != "P3") {
					CMD::println("[IMAGE] ERROR WHILE READING \"" + _filename + "\". We Only Support P3", LOG_TYPE::error);

					this->~Image();

					return;
				}
			} else if (_lineNumber == 2) {
				this->imageWidth  = UTIL::extractValue<uint16_t>(lineStream);
				this->imageHeight = UTIL::extractValue<uint16_t>(lineStream);

				this->nPixels     = this->imageWidth * this->imageHeight;
				this->colorBuffer = std::make_unique<Color<>[]>(this->nPixels);
			} else if (_lineNumber == 3) {
				maxColor = UTIL::extractValue<uint16_t>(lineStream);
			} else {
				const uint8_t red   = static_cast<uint8_t>(UTIL::extractValue<uint16_t>(lineStream) / static_cast<float>(maxColor) * 255);
				const uint8_t green = static_cast<uint8_t>(UTIL::extractValue<uint16_t>(lineStream) / static_cast<float>(maxColor) * 255);
				const uint8_t blue  = static_cast<uint8_t>(UTIL::extractValue<uint16_t>(lineStream) / static_cast<float>(maxColor) * 255);

				this->colorBuffer[_lineNumber - 2] = Color<>(red, green, blue);
			}
		});
	} catch (const char* _error) {
		CMD::println("[ERROR] While Opening/Writing To \"" + _filename + "\": " + std::string(_error), LOG_TYPE::error);
	}
}

Image::Image(const unsigned int _imageWidth, const unsigned int _imageHeight, const Color<>& _backgroundColor) : imageWidth(_imageWidth), imageHeight(_imageHeight) {
	this->nPixels = this->imageWidth * this->imageHeight;

	this->colorBuffer = std::make_unique<Color<>[]>(this->nPixels);
	std::fill_n(this->colorBuffer.get(), this->nPixels, _backgroundColor);
}

 uint32_t Image::getIndex(const uint16_t _x, const uint16_t _y) const { return _y * this->imageWidth + _x; }

 uint16_t Image::getWidth()  const { return this->imageWidth; }
 uint16_t Image::getHeight() const { return this->imageHeight; }

 void Image::setColor(const uint16_t _x, const uint16_t _y, const Color<>& _c) {
	this->colorBuffer[_y * this->imageWidth + _x] = _c;
}

 Color<> Image::sample(const uint16_t _x, const uint16_t _y) const {
	return this->colorBuffer[_y * this->imageWidth + _x];
}

void Image::resize(const uint16_t _width, const uint16_t _height) {
	Image newImage = Image(_width, _height);
	
	for (uint16_t x = 0; x < _width; x++) {
		for (uint16_t y = 0; y < _height; y++) {
			const uint16_t sampleX = static_cast<uint16_t>((x / static_cast<float>(_width))  * this->imageWidth);
			const uint16_t sampleY = static_cast<uint16_t>((y / static_cast<float>(_height)) * this->imageHeight);
			const Color<> pixelColor = this->sample(sampleX, sampleY);

			newImage.setColor(x, y, pixelColor);
		}
	}

	this->colorBuffer.reset();

	this->imageWidth  = _width;
	this->imageHeight = _height;
	this->nPixels     = this->imageWidth * this->imageHeight;
	this->colorBuffer = std::make_unique<Color<>[]>(this->nPixels);
	
	std::memcpy(this->colorBuffer.get(), newImage.colorBuffer.get(), this->nPixels * sizeof(Color<>));
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