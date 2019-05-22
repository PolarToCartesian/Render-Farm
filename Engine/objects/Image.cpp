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
	FILE* file;

	if (EN::UTIL::openFile(file, _fileName, "w")) {
		// PPM Header
		fprintf(file, "P3 \n%u %u\n255\n", this->imageWidth, this->imageHeight);

		// PPM Contents
		for (unsigned int i = 0; i < this->nPixels; i++) {
			fprintf(file, "%u %u %u\n", static_cast<unsigned int>(this->colorBuffer[i].r), static_cast<unsigned int>(this->colorBuffer[i].g), static_cast<unsigned int>(this->colorBuffer[i].b));
		}
	} else {
		std::cout << "[ERROR] While Writing To \"" << _fileName << "\"" << std::endl;
	}
	
	fclose(file);
}

Image::~Image() {
	delete[] colorBuffer;
}