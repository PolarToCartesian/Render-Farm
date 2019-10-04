#pragma once

#include "models/Model.h"
#include "../other/Util.h"
#include "objects/Light.h"
#include "cameras/Camera.h"
#include "../other/Timer.h"
#include "materials/Material.h"
#include "../files/media/Video.h"
#include "../files/media/Image.h"
#include "../math/BarycentricInterpolation.h"

#include <experimental/filesystem>
#include <optional>

struct VertexNormalCalcInfo {
	Vec3 normal;
	float sumWeights;
};

class Renderer {
	private:
		uint16_t width, height;

		Mat4x4 perspectiveMatrix;

		float* depthBuffer = nullptr;

		Color<> backgroundColor;

		Image renderSurface;

	public:
		Camera* camera = nullptr;
		
		std::unordered_map<std::string, Light> lights;
		std::unordered_map<std::string, Model> models;
		std::unordered_map<std::string, Texture> textures;
		std::unordered_map<std::string, Material> materials;

		std::vector<std::string> renderModels;
		std::vector<std::string> renderLights;

	private:
		void resetDepthBuffer();

		void calculatePerspectiveMatrix();

		std::unordered_map<std::string, VertexNormalCalcInfo> calculateVertexNormals(const Model& _model, const std::vector<std::array<Vec3, 3>> & _rotatedVertices) const;

		void drawModels(const std::vector<Light*>& _lightsInScene);

		void renderAndWriteFrames(const uint32_t _fps, const uint32_t _duration);

		void writeVideo(const uint32_t _fps, const uint32_t _duration);

	public:
		Renderer(const uint16_t _width, const uint16_t _height, Camera* _camera, const Color<>& _backgroundColor = Color<>(51));

		virtual void update() = 0;
		virtual void render2D() = 0;
		virtual void render3D() = 0;

		uint32_t getWidth()  const;
		uint32_t getHeight() const;

		void run(const uint32_t _fps, const uint32_t _duration);

		void fillDisk(const uint16_t _x, const uint16_t _y, const uint16_t _r, const Color<>& _color);
		void fillRect(const uint16_t _x, const uint16_t _y, const uint16_t _w, const uint16_t _h, const Color<>& _color);
		void textureRect(const uint16_t _x, const uint16_t _y, const uint16_t _w, const uint16_t _h, const std::string& _textureName);

		~Renderer();
};