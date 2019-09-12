#pragma once

#include "shapes/Model.h"
#include "objects/Light.h"
#include "../other/Timer.h"
#include "../files/Video.h"
#include "objects/Camera.h"
#include "../files/Image.h"
#include "materials/Material.h"
#include "../math/BarycentricInterpolation.h"

#include <experimental/filesystem>
#include <optional>
#include <deque>

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

		float* depthBuffer = nullptr;

		Color<> backgroundColor;

		Image renderSurface;

	public:
		Camera camera;
		
		std::unordered_map<std::string, Light> lights;
		std::unordered_map<std::string, Model> models;
		std::unordered_map<std::string, Material> materials;

		std::deque<std::string> renderModels;
		std::deque<std::string> renderLights;

	private:
		void resetDepthBuffer();

		void calculatePerspectiveMatrix();

		std::unordered_map<std::string, VertexNormalCalcInfo> calculateVertexNormals(const Model& _model, const std::vector<std::array<Vec3, 3>> & _rotatedVertices) const;

		void drawTriangle2D(const Vec3& _a, const Vec3& _b, const Vec3& _c, const std::function<std::optional<Color<>>(const uint16_t _x, const uint16_t _y)>& _func);

		void drawModels(const std::vector<Light>& _lightsInScene);

	public:
		Renderer(const uint16_t _width, const uint16_t _height, const Color<>& _backgroundColor = Color<>(51), const uint8_t _fov = 90, const float _zNear = 0.1, const float _zFar = 1000);

		~Renderer();

		virtual void update() = 0;
		virtual void render3D() = 0;

		uint32_t getWidth()  const;
		uint32_t getHeight() const;

		void renderAndWriteFrames(const uint32_t _nFrames);

		void writeVideo(const uint32_t _nFrames, const uint16_t _fps = 30);
};