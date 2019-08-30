#pragma once

#include "../math/BarycentricInterpolation.h"
#include "materials/Material.h"
#include "../files/Video.h"
#include "objects/Camera.h"
#include "../files/Image.h"
#include "objects/Light.h"
#include "shapes/Model.h"

struct VertexNormalCalcInfo {
	Vec3 normal;
	float sumWeights;
};

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
		std::vector<Material> materials;
		std::deque<uint64_t> renderQueue;

	public:
		Camera camera;

	private:
		void resetDepthBuffer();

		uint32_t getIndexInColorBuffer(const uint16_t _x, const uint16_t _y);

		void calculatePerspectiveMatrix();

		std::unordered_map<std::string, VertexNormalCalcInfo> calculateVertexNormals(const Model& _model, const std::vector<std::array<Vec3, 3>> & _rotatedVertices) const;

		void drawTriangle2D(const Vec3& _a, const Vec3& _b, const Vec3& _c, const std::function<std::optional<Color<>>(const uint16_t _x, const uint16_t _y)>& _func);

		void drawModels();

	public:
		Renderer(const uint16_t _width, const uint16_t _height, const Color<>& _backgroundColor = Color<>(51), const uint8_t _fov = 90, const float _zNear = 0.1, const float _zFar = 1000);

		~Renderer();

		virtual void update() = 0;
		virtual void render() = 0;

		uint32_t getWidth()  const;
		uint32_t getHeight() const;

		uint64_t addLight(const Light& _light);
		Light    copyLight(const uint64_t _lightId) const;
		Light&   getLightRef(const uint64_t _lightId);
		void     setLight(const uint64_t _lightId, const Light& _light);

		uint64_t addModel(const Model& _model);
		Model    copyModel(const uint64_t _modelId) const;
		Model&   getModelRef(const uint64_t _modelId);
		void     setModel(const uint64_t _modelId, const Model& _model);

		uint64_t  addMaterial(const Material& _material);
		Material  copyMaterial(const uint64_t _materialId) const;
		Material& getMaterialRef(const uint64_t _materialId);
		void      setMaterial(const uint64_t _materialId, const Material& _material);

		void addModelToRenderQueue(const uint64_t _modelId);

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