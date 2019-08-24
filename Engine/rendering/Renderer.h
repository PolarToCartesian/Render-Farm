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
		float zNear, zFar;

		unsigned int indexImageBeingRendered = 0;

		float* depthBuffer = nullptr;

		Image* renderImages[RENDERS_AND_WRITES_PER_CYCLE];

		Color<> backgroundColor;

		std::vector<Model> models;
		std::vector<Light> lights;
		std::vector<uint64_t> renderQueue;

	public:
		Camera camera;

	private:
		void resetDepthBuffer();

		uint32_t getIndexInColorBuffer(const uint16_t _x, const uint16_t _y);

		void calculatePerspectiveMatrix();

		void drawModels();

	public:
		Renderer(const uint16_t _width, const uint16_t _height, const Color<>& _backgroundColor = Color<>(51), const uint8_t _fov = 90, const float _zNear = 0.1, const float _zFar = 1000);

		~Renderer();

		virtual void update() = 0;
		virtual void render() = 0;

		uint32_t getWidth()  const;
		uint32_t getHeight() const;

		uint32_t addLight(const Light& _light);
		Light    copyLight(const uint32_t _lightId) const;
		Light&   getLightRef(const uint32_t _lightId);
		void     setLight(const uint32_t _lightId, const Light& _light);

		uint32_t addModel(const Model& _model);
		Model    copyModel(const uint32_t _modelId) const;
		Model&   getModelRef(const uint32_t _modelId);
		void     setModel(const uint32_t _modelId, const Model& _model);

		void addModelToRenderQueue(const uint32_t _modelId);

		void drawPointNoVerif(const uint16_t _x, const uint16_t _y, const Color<>& _color);
		void drawPoint(const uint16_t _x, const uint16_t _y, const Color<>& _color);

		void drawRectangleNoVerif(const uint16_t _x, const uint16_t _y, const uint16_t _w, const uint16_t _h, const Color<>& _color);
		void drawRectangle(const uint16_t _x, const uint16_t _y, const uint16_t _w, const uint16_t _h, const Color<>& _color);

		void drawImageNoVerif(const uint16_t _x, const uint16_t _y, const Image& _image);
		void drawImage(const uint16_t _x, const uint16_t _y, const Image& _image);

		void drawImageNoVerif(const uint16_t _x, const uint16_t _y, const uint16_t _width, const uint16_t _height, const Image& _image);
		void drawImage(const uint16_t _x, const uint16_t _y, const uint16_t _width, const uint16_t _height, const Image& _image);

		void drawDisk(const Vec3& _position, const uint16_t _radius, const Color<>& _color);

		void renderAndWriteFrames(const uint32_t _nFrames);

		void writeVideo(const uint32_t _nFrames, const uint16_t _fps = 30);
};