#pragma once

#include "../math/Vec3.h"
#include "../other/pch.h"
#include "File.h"

class Image {
	private:
		unsigned int imageWidth, imageHeight;

		unsigned int nPixels;
		
	public:
		Color * colorBuffer;

	public:
		Image(const unsigned int& _imageWidth, const unsigned int& _imageHeight);

		unsigned int getIndex(const unsigned int& _x, const unsigned int& _y) const;

		unsigned int getWidth()  const;
		unsigned int getHeight() const;

		void setColor(const unsigned int& _x, const unsigned int& _y, const Color& _c);

		void writeToDisk(const std::string& _fileName) const;

		~Image();
};