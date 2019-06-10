#pragma once

#include "../other/Color.h"
#include "../math/Vec3.h"
#include "../other/pch.h"
#include "File.h"

// Only Works With PPM Images With 255 in header

class Image {
	private:
		unsigned int imageWidth = 0, imageHeight = 0;

		unsigned int nPixels = 0;
		
	public:
		Color * colorBuffer = nullptr;

	public:
		Image(const std::string& _filename, const bool& _doLog = false);
		Image(const unsigned int& _imageWidth, const unsigned int& _imageHeight, const Color& _backgroundColor = Color(0));

		unsigned int getIndex(const unsigned int& _x, const unsigned int& _y) const;

		unsigned int getWidth()  const;
		unsigned int getHeight() const;

		void setColor(const unsigned int& _x, const unsigned int& _y, const Color& _c);

		Color sample(const unsigned int& _x, const unsigned int& _y) const;

		void writeToDisk(const std::string& _fileName) const;

		~Image();
};