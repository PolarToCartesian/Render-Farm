#include "Image.h"

// Image Class

Image::Image(const Image& _img) {
	this->imageWidth = _img.imageWidth;
	this->imageHeight = _img.imageHeight;
	this->nPixels = _img.nPixels;

	this->colorBuffer = new Color<>[this->nPixels];
	std::memcpy(this->colorBuffer, _img.colorBuffer, this->nPixels * sizeof(Color<>));
}

Image::Image(const std::string& _filename, const bool _doLog) {
	File file(_filename, FILE_READ, _doLog);

	if (file.isOpen()) {
		file.readLineByLine([this, _filename, _doLog](const std::string& _line, const unsigned int _lineNumber) {
			std::istringstream lineStream(_line);

			switch (_lineNumber) {
				case 1:
					{
						std::string ppmType;

						lineStream >> ppmType;

						if (ppmType != "P3") {
							if (_doLog) LOG::println("[IMAGE] ERROR WHILE READING \"" + _filename + "\". We Only Support P3", LOG_TYPE::error);

							this->~Image();

							return;
						}
					}
					break;
				case 2:
					{
						lineStream >> this->imageWidth >> this->imageHeight;

						this->nPixels = this->imageWidth * this->imageHeight;
						this->colorBuffer = new Color<>[this->nPixels];
					}
					break;
				case 3:
					break;

				default:
					std::string r, g, b;

					lineStream >> r >> g >> b;

					Color<> color(std::stoi(r), std::stoi(g), std::stoi(b));

					this->colorBuffer[_lineNumber - 2] = color;
			}
		});
	} else {
		this->~Image();
	}
}

Image::Image(const unsigned int _imageWidth, const unsigned int _imageHeight, const Color<>& _backgroundColor) : imageWidth(_imageWidth), imageHeight(_imageHeight) {
	this->nPixels = this->imageWidth * this->imageHeight;

	this->colorBuffer = new Color<>[this->nPixels];
	std::fill_n(this->colorBuffer, this->nPixels, _backgroundColor);
}

uint32_t Image::getIndex(const uint16_t _x, const uint16_t _y) const {
	return _y * this->imageWidth + _x;
}

unsigned int Image::getWidth()  const { return this->imageWidth; }
unsigned int Image::getHeight() const { return this->imageHeight; }

void Image::setColor(const uint16_t _x, const uint16_t _y, const Color<>& _c) {
	this->colorBuffer[this->getIndex(_x, _y)] = _c;
}

Color<> Image::sample(const uint16_t _x, const uint16_t _y) const {
	return this->colorBuffer[this->getIndex(_x, _y)];
}

void Image::resize(const uint16_t _width, const uint16_t _height) {
	Image newImage = Image(_width, _height);
	
	for (uint16_t x = 0; x < _width; x++) {
		for (uint16_t y = 0; y < _height; y++) {
			const uint16_t sampleX = static_cast<uint16_t>((x / static_cast<double>(_width))  * this->imageWidth);
			const uint16_t sampleY = static_cast<uint16_t>((y / static_cast<double>(_height)) * this->imageHeight);
			const Color<> pixelColor = this->sample(sampleX, sampleY);

			newImage.setColor(x, y, pixelColor);
		}
	}

	delete[] this->colorBuffer;

	this->imageWidth  = _width;
	this->imageHeight = _height;
	this->nPixels = this->imageWidth * this->imageHeight;
	this->colorBuffer = new Color<>[this->nPixels];
	
	std::memcpy(this->colorBuffer, newImage.colorBuffer, this->nPixels * sizeof(Color<>));
}

void Image::writeToDisk(const std::string& _fileName) const {
	File file(_fileName, FILE_WRITE, false);

	if (file.isOpen()) {
		std::string fileContents = "P3 \n" + std::to_string(this->imageWidth) + " " + std::to_string(this->imageHeight) + "\n255\n";

		// PPM Contents
		for (unsigned int i = 0; i < this->nPixels; i++) {
			fileContents += std::to_string(static_cast<uint8_t>(this->colorBuffer[i].r)) + " " + 
				            std::to_string(static_cast<uint8_t>(this->colorBuffer[i].g)) + " " + 
				            std::to_string(static_cast<uint8_t>(this->colorBuffer[i].b)) + "\n";
		}

		file.writeNoVerif(fileContents);
	} else {
		LOG::println("[ERROR] While Writing To \"" + _fileName + "\"", LOG_TYPE::error);
	}
}

Image::~Image() {
	delete[] colorBuffer;
}