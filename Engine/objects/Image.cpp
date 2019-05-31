#include "Image.h"

// Image Class

Image::Image(const unsigned int& _imageWidth, const unsigned int& _imageHeight) : imageWidth(_imageWidth), imageHeight(_imageHeight) {
	this->nPixels = this->imageWidth * this->imageHeight;

	this->colorBuffer = new Color[this->nPixels];
}

unsigned int Image::getIndex(const unsigned int& _x, const unsigned int& _y) const {
	return _y * this->imageWidth + _x;
}

unsigned int Image::getWidth()  const { return this->imageWidth; }
unsigned int Image::getHeight() const { return this->imageHeight; }

void Image::setColor(const unsigned int& _x, const unsigned int& _y, const Color& _c) {
	this->colorBuffer[this->getIndex(_x, _y)] = _c;
}

void Image::writeToDisk(const char* _fileName) const {
	File file(_fileName, FILE_WRITE, false);

	if (file.isOpen()) {
		// PPM Header
		file.writeNoVerif("P3 \n" + std::to_string(this->imageWidth) + " " + std::to_string(this->imageHeight) + "\n255\n");

		// PPM Contents
		for (unsigned int i = 0; i < this->nPixels; i++) {
			file.writeNoVerif(std::to_string((unsigned char)this->colorBuffer[i].r) + " " + std::to_string((unsigned char)this->colorBuffer[i].g) + " " + std::to_string((unsigned char)this->colorBuffer[i].b) + "\n");
		}
	} else {
		EN::LOG::println("[ERROR] While Writing To \"" + std::string(_fileName) + "\"");
	}	
}

Image::~Image() {
	delete[] colorBuffer;
}