#include "Renderer.h"

// Private Methods

void Renderer::resetDepthBuffer() {
	std::fill_n(this->depthBuffer, this->width * this->height, -1.f);
}

void Renderer::calculatePerspectiveMatrix() {
	this->perspectiveMatrix = Mat4x4::getPerspectiveMatrix(this->width, this->height, this->fov, this->zNear, this->zFar);
}

std::unordered_map<std::string, VertexNormalCalcInfo> Renderer::calculateVertexNormals(const Model& _model, const std::vector<std::array<Vec3, 3>>& _rotatedVerticesOfTriangles) const {
	// Calculate Vertex Normals
	// Thansk to : https://stackoverflow.com/questions/6656358/calculating-normals-in-a-triangle-mesh/6661242#6661242

	//  We use a hash table to divide render times by 20 (according to my rough estimates)!
	//  1) hashed rotated vertex    2) Info About rotated Vertex To Calculate Normal
	std::unordered_map<std::string, VertexNormalCalcInfo> vertexNormalCalc;
	vertexNormalCalc.reserve(3 * _model.triangles.size());

	bool doVertexNormalsNeedToBeCalculated = false;

	for (uint64_t i = 0; i < _model.triangles.size(); i++) {
		if (_model.triangles[i].isSmoothed) {
			doVertexNormalsNeedToBeCalculated = true;
			break;
		}
	}

	if (doVertexNormalsNeedToBeCalculated) {
		for (const std::array<Vec3, 3>& rotatedVertices : _rotatedVerticesOfTriangles) {
			const Vec3 normal = Triangle::getSurfaceNormal(rotatedVertices);

			const float weight = 1; //*

			for (const Vec3& rotatedVertex : rotatedVertices) {
				const std::string hashedRotatedVertex = Vec3::hashVec3(rotatedVertex);

				if (vertexNormalCalc.find(hashedRotatedVertex) == vertexNormalCalc.end())
					vertexNormalCalc.insert({ hashedRotatedVertex, { Vec3(), 0 } });

				vertexNormalCalc[hashedRotatedVertex].normal += normal * weight;
				vertexNormalCalc[hashedRotatedVertex].sumWeights += weight;
			}
		}

		for (auto& it : vertexNormalCalc)
			it.second.normal /= it.second.sumWeights;
	}

	return vertexNormalCalc;
}

// For the basic Renderer (thx) https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
void Renderer::drawTriangle2D(const Vec3& _a, const Vec3& _b, const Vec3& _c, const std::function<std::optional<Color<>>(const uint16_t _x, const uint16_t _y)>& _func) {		
	Vec3 t0 = Vec3::intify(_a), t1 = Vec3::intify(_b), t2 = Vec3::intify(_c);

	if (t0.y == t1.y && t0.y == t2.y)
		return;

	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	const uint16_t total_height = static_cast<uint16_t>(t2.y - t0.y);

	for (uint16_t i = 0; i < total_height; i++) {
		const uint16_t y = static_cast<uint16_t>(t0.y + i);

		if (y < 0 || y > this->height - 1)
			continue;

		const bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		const int segment_height = static_cast<int>(second_half ? t2.y - t1.y : t1.y - t0.y);

		const float alpha = i / static_cast<float>(total_height);
		const float beta = static_cast<float>(i - (second_half ? t1.y - t0.y : 0)) / segment_height;

		Vec3 A = t0 + (t2 - t0) * alpha;
		Vec3 B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;

		if (A.x > B.x)
			std::swap(A, B);

		// Limit X Between (0 and width - 1) in the loop to avoid trying to set a pixel out of the screen buffer and to maximize performance
		for (int j = static_cast<int>((A.x < 0) ? 0 : A.x); j <= static_cast<int>((B.x > this->width - 2) ? this->width - 2 : B.x); j++) {
			const uint16_t x = j;

			const uint32_t pixelIndex = this->renderSurface.getIndex(x, y);

			std::optional<Color<>> pixelColor = _func(x, y);

			if (pixelColor.has_value())
				this->renderSurface.colorBuffer[pixelIndex] = pixelColor.value();
		}
	}
}

void Renderer::drawModels(const std::vector<Light*>& _lightsInScene) {
	for (const std::string& modelName : this->renderModels) {
		Model& model = this->models.at(modelName);

		std::vector<std::array<Vec3, 3>> rotatedVerticesOfTriangles;
		rotatedVerticesOfTriangles.reserve(model.triangles.size());

		model.applyFunctionToEachTriangle([this, &rotatedVerticesOfTriangles](Triangle& _tr) {
			rotatedVerticesOfTriangles.push_back(_tr.getRotatedVertices(this->camera));
		});

		std::unordered_map<std::string, VertexNormalCalcInfo> allVertexNormals = this->calculateVertexNormals(model, rotatedVerticesOfTriangles);

		// Render

		for (uint64_t trIndex = 0; trIndex < model.triangles.size(); trIndex++) {
			const Triangle&                triangle            = model.triangles[trIndex];
			const std::array<Vec3, 3>      rotatedVertices     = rotatedVerticesOfTriangles[trIndex];
			const Vec3                     surfaceNormal       = Triangle::getSurfaceNormal(rotatedVertices);
			const Material&                material            = this->materials.at(triangle.material);
			const std::array<Vec3, 3>      transformedVertices = Triangle::getTransformedVertices(rotatedVertices, this->camera, this->perspectiveMatrix, this->width, this->height);
				  BarycentricInterpolation bi(transformedVertices.data());

			// Check if triangle is facing camera
			if (!this->camera.isTriangleFacingCamera(rotatedVertices, surfaceNormal))
				continue;

			std::array<Vec3, 3> vertexNormals;
			std::array<Color<>, 3> baseVertexColors;

			if (triangle.isSmoothed) {
				vertexNormals = {
					allVertexNormals[Vec3::hashVec3(rotatedVertices[0])].normal,
					allVertexNormals[Vec3::hashVec3(rotatedVertices[1])].normal,
					allVertexNormals[Vec3::hashVec3(rotatedVertices[2])].normal
				};
				
				baseVertexColors = material.vertexColors;
			} else {
				baseVertexColors = {
					Light::getDiffusedLighting(rotatedVertices[0], material.vertexColors[0], surfaceNormal, _lightsInScene, this->camera.position),
					Light::getDiffusedLighting(rotatedVertices[1], material.vertexColors[1], surfaceNormal, _lightsInScene, this->camera.position),
					Light::getDiffusedLighting(rotatedVertices[2], material.vertexColors[2], surfaceNormal, _lightsInScene, this->camera.position)
				};
			}

			// Start Rendering

			this->drawTriangle2D(transformedVertices[0], transformedVertices[1], transformedVertices[2], [&](const uint16_t _x, const uint16_t _y) -> std::optional<Color<>> {
				bi.precalculateValuesForPosition(_x, _y);

				// Pixel Depth (w)
				const float w = bi.interpolateWPrecalc(std::array<float, 3> { transformedVertices[0].w, transformedVertices[1].w, transformedVertices[2].w });

				// Render Pixel if it is in front of the pixel already there
				const uint32_t pixelIndex = renderSurface.getIndex(_x, _y);

				if (w < this->depthBuffer[pixelIndex] || this->depthBuffer[pixelIndex] == -1) {
					this->depthBuffer[pixelIndex] = w;

					Color<float> pixelColorFloat = bi.interpolateWPrecalc(std::array<Color<float>, 3>{ Color<float>(baseVertexColors[0]), Color<float>(baseVertexColors[1]), Color<float>(baseVertexColors[2]) });
					pixelColorFloat.constrain(0, 255);

					const Vec3 position = bi.interpolateWPrecalc(rotatedVertices);

					Color<> pixelColor;

					if (triangle.isSmoothed) {
						const Vec3 normal = bi.interpolateWPrecalc(vertexNormals);

						pixelColor = Light::getColorWithLighting(position, Color<>(pixelColorFloat), normal, _lightsInScene, this->camera.position, material);
					} else {
						pixelColor = pixelColorFloat + Light::getSpecularLighting(position, surfaceNormal, _lightsInScene, this->camera.position, material);
					}

					return pixelColor;
				} else {
					return {};
				}
			});
		}
	}
}

// Public Methods

Renderer::Renderer(const uint16_t _width, const uint16_t _height, const Color<>& _backgroundColor, const uint8_t _fov, const float _zNear, const float _zFar)
	: width(_width), height(_height), fov(_fov), zNear(_zNear), zFar(_zFar), renderSurface(_width, _height, _backgroundColor)
{
	std::experimental::filesystem::create_directory("./out");
	std::experimental::filesystem::create_directory("./out/frames");

	this->calculatePerspectiveMatrix();
	this->backgroundColor = _backgroundColor;
	this->depthBuffer = new float[static_cast<uint32_t>(this->width * this->height)];
}

Renderer::~Renderer() {
	delete[] this->depthBuffer;
}

 uint32_t Renderer::getWidth()  const { return this->width;  }
 uint32_t Renderer::getHeight() const { return this->height; }

void Renderer::renderAndWriteFrames(const uint32_t _nFrames) {
	// Render And Write Frames
	for (uint32_t nFrame = 0; nFrame < _nFrames; nFrame++) {
		{ // Rendering
			const Timer renderingTimer((std::string("[RENDERING] Frame n") + std::to_string(nFrame + 1) + " was rendered in ").c_str());

			this->resetDepthBuffer();
			this->renderModels.clear();
			this->renderLights.clear();

			// Do Not Update If It Is The First Frame
			if (!(nFrame == 0)) this->update();

			this->render3D();

			this->camera.calculateRotationMatrices();

			// Get All Lights In Scene
			std::vector<Light*> lightsInScene;
			lightsInScene.reserve(this->renderLights.size());

			for (const std::string& _lightName : this->renderLights)
				lightsInScene.push_back(&this->lights.at(_lightName));

			this->drawModels(lightsInScene);
		}

		{ // Writing
			const Timer renderingTimer((std::string("[WRITING]   Frame n") + std::to_string(nFrame + 1) + " was written in ").c_str());

			renderSurface.writeToDisk("./out/frames/" + std::to_string(nFrame + 1) + ".ppm");
		}
		
		std::fill_n(this->renderSurface.colorBuffer.get(), this->width * this->height, 0);
	}
}

void Renderer::writeVideo(const uint32_t _nFrames, const uint16_t _fps) {
	std::experimental::filesystem::create_directory("./out/video");

	Video video;

	for (uint32_t i = 1; i < _nFrames+1; i++)
		video.addFrame("./out/frames/" + std::to_string(i) + ".ppm");

	video.save("./out/video/video.avi", _fps);
}