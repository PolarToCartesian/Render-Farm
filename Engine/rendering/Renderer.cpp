#include "Renderer.h"

// Private Methods

void Renderer::resetDepthBuffer() {
	std::fill_n(this->depthBuffer, this->width * this->height, -1.f);
}

void Renderer::calculatePerspectiveMatrix() {
	this->perspectiveMatrix = Mat4x4::getPerspectiveMatrix(this->width, this->height, this->camera->fov, this->camera->zNear, this->camera->zFar);
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
			const std::array<Vec3, 3>      textureCoordinates  = { triangle.vertices[0].textureCoord, triangle.vertices[1].textureCoord, triangle.vertices[2].textureCoord };
			const Texture&                 texture             = (material.isTextured) ? this->textures[material.textureName] : Texture();
				  BarycentricInterpolation bi(transformedVertices);

			// Check if triangle is facing camera
			if (!this->camera->isTriangleFacingCamera(rotatedVertices, surfaceNormal))
				continue;

			std::array<Vec3, 3> vertexNormals;

			if (triangle.isSmoothed) {
				vertexNormals = {
					allVertexNormals[Vec3::hashVec3(rotatedVertices[0])].normal,
					allVertexNormals[Vec3::hashVec3(rotatedVertices[1])].normal,
					allVertexNormals[Vec3::hashVec3(rotatedVertices[2])].normal
				};
			}

			std::array<Color<float>, 3> baseVertexColors;

			if (!material.isTextured) {
				if (triangle.isSmoothed) {
					baseVertexColors = { material.vertexColors[0], material.vertexColors[1],material.vertexColors[2] };
				} else {
					baseVertexColors = {
						Light::getDiffusedLighting(rotatedVertices[0], material.vertexColors[0], surfaceNormal, _lightsInScene, this->camera->position),
						Light::getDiffusedLighting(rotatedVertices[1], material.vertexColors[1], surfaceNormal, _lightsInScene, this->camera->position),
						Light::getDiffusedLighting(rotatedVertices[2], material.vertexColors[2], surfaceNormal, _lightsInScene, this->camera->position)
					};
				}
			}

			// Start Rendering

			Triangle::drawTriangle2D(this->renderSurface, transformedVertices[0], transformedVertices[1], transformedVertices[2], [&](const uint16_t _x, const uint16_t _y) -> std::optional<Color<>> {
				bi.precalculateValuesForPosition(_x, _y);

				// Pixel Depth (w)
				const float w = bi.interpolateWPrecalc(std::array<float, 3> { transformedVertices[0].w, transformedVertices[1].w, transformedVertices[2].w });

				// Render Pixel if it is in front of the pixel already there
				const uint32_t pixelIndex = renderSurface.getIndex(_x, _y);

				if (w < this->depthBuffer[pixelIndex] || this->depthBuffer[pixelIndex] == -1) {
					this->depthBuffer[pixelIndex] = w;

					const Vec3 position     = bi.interpolateWPrecalc(rotatedVertices);
					const Vec3 textureCoord = (material.isTextured) ? Vec3::constrain(bi.interpolateWPrecalc(textureCoordinates), 0.f, 1.f) : Vec3();
					const Vec3 normal       = (triangle.isSmoothed) ? bi.interpolateWPrecalc(vertexNormals)                                 : surfaceNormal;

					const Color<float> basePixelColor = Color<float>::constrain((material.isTextured) ? Color<float>(texture.sample(textureCoord.x * texture.getWidth(), textureCoord.y * texture.getHeight())) : Color<float>(bi.interpolateWPrecalc(baseVertexColors)), 0.f, 255.f);
					
					return Color<>::constrain(Light::getColorWithLighting(position, basePixelColor, normal, _lightsInScene, this->camera->position, material), (uint8_t)0, (uint8_t)255);
				} else {
					return {};
				}
			});
		}
	}
}

void Renderer::renderAndWriteFrames(const uint32_t _fps, const uint32_t _duration) {
	// Render And Write Frames
	for (uint32_t nFrame = 0; nFrame < _fps * _duration; nFrame++) {
		{ // Rendering
			const Timer renderingTimer((std::string("[RENDERING] Frame n") + std::to_string(nFrame + 1) + " was rendered in ").c_str());

			this->resetDepthBuffer();
			this->renderModels.clear();
			this->renderLights.clear();

			// Do Not Update If It Is The First Frame
			if (!(nFrame == 0)) this->update();

			this->render3D();

			this->camera->calculateRotationMatrices();

			// Get All Lights In Scene
			std::vector<Light*> lightsInScene;
			lightsInScene.reserve(this->renderLights.size());

			for (const std::string& _lightName : this->renderLights)
				lightsInScene.push_back(&this->lights.at(_lightName));

			this->drawModels(lightsInScene);

			this->render2D();
		}

		{ // Writing
			const Timer renderingTimer((std::string("[WRITING]   Frame n") + std::to_string(nFrame + 1) + " was written in ").c_str());

			this->renderSurface.writeToDisk("./out/frames/" + std::to_string(nFrame + 1) + ".ppm");
		}

		this->renderSurface.fill(this->backgroundColor);
	}
}

void Renderer::writeVideo(const uint32_t _fps, const uint32_t _duration) {
	std::experimental::filesystem::create_directory("./out/video");

	Video video;

	for (uint32_t i = 1; i < _fps * _duration + 1; i++)
		video.addFrame("./out/frames/" + std::to_string(i) + ".ppm");

	video.save("./out/video/video.avi", _fps);
}

// Public Methods

Renderer::Renderer(const uint16_t _width, const uint16_t _height, Camera* _cam, const Color<>& _backgroundColor)
	: width(_width), height(_height), camera(_cam), renderSurface(_width, _height, _backgroundColor)
{
	std::experimental::filesystem::create_directory("./out");
	std::experimental::filesystem::create_directory("./out/frames");

	this->calculatePerspectiveMatrix();
	this->backgroundColor = _backgroundColor;
	this->depthBuffer = new float[static_cast<uint32_t>(this->width * this->height)];
}

uint32_t Renderer::getWidth()  const { return this->width;  }
uint32_t Renderer::getHeight() const { return this->height; }

void Renderer::run(const uint32_t _fps, const uint32_t _duration) {
	this->renderAndWriteFrames(_fps, _duration);
	this->writeVideo(_fps, _duration);
}

void Renderer::fillDisk(const uint16_t _x, const uint16_t _y, const uint16_t _r, const Color<>& _color) {
	const float r2 = static_cast<float>(std::pow(_r, 2));

	for (int y = -_r; y <= _r; y++) {
		const float y2 = static_cast<float>(std::pow(y, 2));

		for (int x = -_r; x <= _r; x++) {
			if (x * x + y2 <= r2)
				this->renderSurface.set(_x + x, y + _y, _color);			
		}
	}
}

void Renderer::fillRect(const uint16_t _x, const uint16_t _y, const uint16_t _w, const uint16_t _h, const Color<>& _color) {
	const uint16_t xEnd = (_x + _w < this->width)  ? _x + _w : this->width  - 1;
	const uint16_t yEnd = (_y + _h < this->height) ? _y + _h : this->height - 1;
	
	for (uint16_t y = _y; y < yEnd; y++) {
		uint32_t renderSurfacePixelIndex = this->width * y + _x;

		for (uint16_t x = _x; x < xEnd; x++)
			this->renderSurface.colorBuffer[renderSurfacePixelIndex++] = _color;
	}
}

void Renderer::textureRect(const uint16_t _x, const uint16_t _y, const uint16_t _w, const uint16_t _h, const std::string& _textureName) {
	Texture* texture = &this->textures[_textureName];

	const bool doResize = !(texture->getWidth() == _w && texture->getHeight() == _h);

	if (doResize) {
		Texture* resizedTexture = new Texture(*texture);
		resizedTexture->resize(_w, _h);
		texture = resizedTexture;
	}

	uint32_t texturePixelIndex = 0;

	const uint16_t xEnd = (_x + _w < this->width)  ? _x + _w : this->width  - 1;
	const uint16_t yEnd = (_y + _h < this->height) ? _y + _h : this->height - 1;

	for (uint16_t y = _y; y < yEnd; y++) {
		uint32_t renderSurfacePixelIndex = this->width * y + _x;

		for (uint16_t x = _x; x < xEnd; x++)
			this->renderSurface.colorBuffer[renderSurfacePixelIndex++] = texture->colorBuffer[texturePixelIndex++];
	}

	if (doResize) delete texture;
}

Renderer::~Renderer() {
	delete[] this->depthBuffer;
}