// Author : PolarToCartesian
// Github : NaN

/*
Resources :
	- https://codeplea.com/triangular-interpolation
	- https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
	- https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
	- https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_olcEngine3D_Part4.cpp
	- StackOverflow (do I even have to mention it?)
*/

#pragma once

#include "../files/Video.h"
#include "objects/Camera.h"
#include "../files/Image.h"
#include "objects/Light.h"
#include "shapes/Model.h"

class Renderer {
	private:
		uint16_t width, height;

		Mat4x4 perspectiveMatrix;

		uint8_t fov;
		double zNear, zFar;

		unsigned int indexImageBeingRendered = 0;

		double* depthBuffer = nullptr;

		Image* renderImages[RENDERS_AND_WRITES_PER_CYCLE];

		Color<> backgroundColor;

		std::vector<Model> models;
		std::vector<Light> lights;

	public:
		Camera camera;

	private:
		void resetDepthBuffer();

		uint32_t getIndexInColorBuffer(const uint16_t _x, const uint16_t _y);

		void calculatePerspectiveMatrix();

	public:
		Renderer(const uint16_t _width, const uint16_t _height, const Color<>& _backgroundColor = Color<>(51), const uint8_t _fov = 90, const double _zNear = 0.1, const double _zFar = 1000);

		~Renderer();

		virtual void update() = 0;
		virtual void render() = 0;

		uint32_t getWidth()  const;
		uint32_t getHeight() const;

		uint64_t addLight(const Light& _light);
		Light    copyLight(const uint16_t _lightId) const;
		Light&   getLightRef(const uint16_t _lightId);
		void     setLight(const uint16_t _lightId, const Light& _light);

		uint64_t addModel(const Model& _model);
		Model    copyModel(const uint16_t _modelId) const;
		Model&   getModelRef(const uint16_t _modelId);
		void     setModel(const uint16_t _modelId, const Model _model);

		void drawModel(const uint16_t _modelId);

		void drawPointNoVerif(const uint16_t _x, const uint16_t _y, const Color<>& _color);

		void drawPoint(const uint16_t _x, const uint16_t _y, const Color<>& _color);

		void drawRectangleNoVerif(const uint16_t _x, const uint16_t _y, const uint16_t _w, const uint16_t _h, const Color<>& _color);

		void drawRectangle(const uint16_t _x, const uint16_t _y, const uint16_t _w, const uint16_t _h, const Color<>& _color);

		void drawImageNoVerif(const uint16_t _x, const uint16_t _y, const Image& _image);

		void drawImage(const uint16_t _x, const uint16_t _y, const Image& _image);

		void drawTriangle3D(const Triangle& _tr);

		void drawDisk(const Vec3& _position, const uint16_t _radius, const Color<>& _color);

		void renderAndWriteFrames(const uint32_t _nFrames);

		void writeVideo(const uint32_t _nFrames, const uint16_t _fps = 30);
};