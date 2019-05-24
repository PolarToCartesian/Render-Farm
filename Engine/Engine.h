// Author : PolarToCartesian
// Github : NaN

/*
Resources :
	- https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_olcEngine3D_Part4.cpp
	- https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
	- https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
	- https://codeplea.com/triangular-interpolation
	- StackOverflow (do I even have to mention it?)
*/

#pragma once

#include "objects/Camera.h"
#include "objects/Image.h"
#include "objects/Model.h"
#include "objects/Light.h"

#include "other/Util.h"

void engineRenderImage(const unsigned int& _frame, const unsigned int& _frames, const Image& _renderSurface, const std::string& _fileName);

class Engine {
	private:
		unsigned int width, height;

		Matrix4x4 perspectiveMatrix;

		unsigned int fov;
		double zNear, zFar;

		unsigned int currentFrameBeingRendered = 0;

		double* depthBuffer = nullptr;

		std::vector<Image> renderImages;

	public:
		std::vector<Model> models;
		std::vector<Light> lights;

		Camera camera;

	private:
		void dealWithDepthBuffer();

		unsigned int getIndexInColorBuffer(const unsigned int& _x, const unsigned int& _y);

		void apply3D(const Triangle& _tr, bool& _doRenderTriangle, Vector3D& _triangleSurfaceNormal, Vector3D* _rotatedVertices, Vector3D* _transformedVertices);

		void calculateValues();

	public:
		Engine(const unsigned int& _width, const unsigned int& _height, const unsigned int& _fov = 90, const double& _zNear = 0.1, const double& _zFar = 1000);

		~Engine();

		virtual void update() = 0;
		virtual void render() = 0;

		void setWidth(unsigned int& _width);
		void setHeight(unsigned int& _height);

		unsigned int getWidth()  const;
		unsigned int getHeight() const;

		unsigned int addLight(const Light & _light);
		Light copyLight(const unsigned int& _lightId);
		void setLight(const unsigned int& _lightId, const Light _light);

		unsigned int addModel(const Model & _model);
		Model copyModel(const unsigned int& _modelId) const;
		void setModel(const unsigned int& _modelId, const Model _model);

		void drawPointNoVerif(const unsigned int& _x, const unsigned int& _y, const Color & _color);

		void drawPoint(const unsigned int& _x, const unsigned int& _y, const Color & _color);

		void drawRectangleNoVerif(const unsigned int& _x, const unsigned int& _y, const unsigned int& _w, const unsigned int& _h, const Color & _color);

		void drawRectangle(const unsigned int& _x, const unsigned int& _y, const unsigned int& _w, const unsigned int& _h, const Color & _color);

		void drawTriangle3D(const Triangle& _tr);

		void renderAndWriteFrames(const unsigned int& _ticks);

		void writeVideo(const unsigned int& _fps);
};