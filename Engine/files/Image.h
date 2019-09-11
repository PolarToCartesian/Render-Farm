#pragma once


#include "File.h"
#include "../math/Vec3.h"
#include "../other/Util.h"
#include "../other/Color.h"

#include <sstream>

// Only Works With PPM Images With 255 in header

class Image {
	private:
		uint16_t imageWidth = 0, imageHeight = 0;

		uint32_t nPixels = 0;
		
	public:
		std::unique_ptr<Color<>[]> colorBuffer;

	public:
		Image(const Image& _img);
		Image(const std::string& _filename);
		Image(const unsigned int _imageWidth, const unsigned int _imageHeight, const Color<>& _backgroundColor = Color<>());

		uint32_t getIndex(const uint16_t _x, const uint16_t _y) const;

		uint16_t getWidth()  const;
		uint16_t getHeight() const;

		void setColor(const uint16_t _x, const uint16_t _y, const Color<>& _c);

		Color<> sample(const uint16_t _x, const uint16_t _y) const;
		Color<> operator()(const uint16_t _i) const;
		Color<> operator()(const uint16_t _x, const uint16_t _y) const;

		void resize(const uint16_t _width, const uint16_t _height);

		void writeToDisk(const std::string& _fileName) const;
};

typedef Image Texture;