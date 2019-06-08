#include "Image.h"

// Image Class

Image::Image(const std::string& _filename, const bool& _doLog) {
	File file(_filename, FILE_READ, _doLog);

	if (file.isOpen()) {
		file.readLineByLine([this, _filename, _doLog](const std::string& _line, const unsigned int& _lineNumber) {
			std::istringstream lineStream(_line);

			switch (_lineNumber) {
				case 1:
					{
						std::string ppmType;

						lineStream >> ppmType;

						if (ppmType != "P3") {
							if (_doLog) EN::LOG::println("[IMAGE] ERROR WHILE READING \"" + _filename + "\". We Only Support P3", LOG_TYPE::error);

							this->~Image();

							return;
						}
					}
					break;
				case 2:
					{
						lineStream >> this->imageWidth >> this->imageHeight;

						this->nPixels = this->imageWidth * this->imageHeight;
						this->colorBuffer = new Color[this->nPixels];
					}
					break;
				case 3:
					break;

				default:
					std::string r, g, b;

					lineStream >> r >> g >> b;

					Color color(std::stoi(r), std::stoi(g), std::stoi(b));

					this->colorBuffer[_lineNumber - 2] = color;
			}
		});
	} else {
		this->~Image();
	}
}

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

Color Image::sample(const unsigned int& _x, const unsigned int& _y) const {
	return this->colorBuffer[this->getIndex(_x, _y)];
}

void Image::writeToDisk(const std::string& _fileName) const {
	File file(_fileName, FILE_WRITE, false);

	if (file.isOpen()) {
		std::string fileContents = "P3 \n" + std::to_string(this->imageWidth) + " " + std::to_string(this->imageHeight) + "\n255\n";

		// PPM Contents
		for (unsigned int i = 0; i < this->nPixels; i++) {
			fileContents += std::to_string(static_cast<unsigned char>(this->colorBuffer[i].r)) + " " + 
				            std::to_string(static_cast<unsigned char>(this->colorBuffer[i].g)) + " " + 
				            std::to_string(static_cast<unsigned char>(this->colorBuffer[i].b)) + "\n";
		}

		file.writeNoVerif(fileContents);
	} else {
		EN::LOG::println("[ERROR] While Writing To \"" + _fileName + "\"", LOG_TYPE::error);
	}
}

Image::~Image() {
	delete[] colorBuffer;
}