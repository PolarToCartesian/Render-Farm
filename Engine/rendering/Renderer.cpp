#include "Renderer.h"

// Private Methods

inline void Renderer::resetDepthBuffer() {
	std::fill_n(this->depthBuffer, this->width * this->height, -1.f);
}

inline uint32_t Renderer::getIndexInColorBuffer(const uint16_t _x, const uint16_t _y) {
	return renderImages[this->indexImageBeingRendered]->getIndex(_x, _y);
}

void Renderer::calculatePerspectiveMatrix() {
	this->perspectiveMatrix = Mat4x4::getPerspectiveMatrix(this->width, this->height, this->fov, this->zNear, this->zFar);
}

std::unordered_map<std::string, VertexNormalCalcInfo> Renderer::calculateVertexNormals(const Model& _model, const std::vector<std::array<Vec3, 3>>& _rotatedVerticesOfTriangles) const {
	// Calculate Vertex Normals

	//  We use a hash table to divide render times by 20 (according to my rough estimates)!
	//  1) hashed rotated vertex    2) Info About rotated Vertex To Calculate Normal
	std::unordered_map<std::string, VertexNormalCalcInfo> vertexNormalCalc;
	vertexNormalCalc.reserve(3 * _model.triangles.size());

	auto hashRotatedVertex = [](const Vec3& _rotatedVertex) -> std::string {
		return "x" + std::to_string(_rotatedVertex.x) + "y" + std::to_string(_rotatedVertex.y) + "z" + std::to_string(_rotatedVertex.z);
	};

	bool doVertexNormalsNeedToBeCalculated = false;

	for (uint64_t i = 0; i < _model.triangles.size(); i++) {
		if (_model.triangles[i].isSmoothed) {
			doVertexNormalsNeedToBeCalculated = true;
			break;
		}
	}

	if (doVertexNormalsNeedToBeCalculated) {
		for (const std::array<Vec3, 3> & rotatedVertices : _rotatedVerticesOfTriangles) {
			const Vec3 normal = Triangle::getSurfaceNormal(rotatedVertices);
			const float weight = 1; //*

			for (const Vec3& rotatedVertex : rotatedVertices) {
				const std::string hashedRotatedVertex = hashRotatedVertex(rotatedVertex);

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

			const uint32_t pixelIndex = this->getIndexInColorBuffer(x, y);

			std::optional<Color<>> pixelColor = _func(x, y);

			if (pixelColor.has_value())
				renderImages[this->indexImageBeingRendered]->colorBuffer[pixelIndex] = pixelColor.value();
		}
	}
}

void Renderer::drawModels() {
	for (const uint64_t modelId : this->renderQueue) {
		Model& model = this->getModelRef(modelId);

		std::vector<std::array<Vec3, 3>> rotatedVerticesOfTriangles;
		rotatedVerticesOfTriangles.reserve(model.triangles.size());

		model.applyFunctionToEachTriangle([this, &rotatedVerticesOfTriangles](Triangle& _tr) {
			rotatedVerticesOfTriangles.push_back(_tr.getRotatedVertices(this->camera));
		});

		std::unordered_map<std::string, VertexNormalCalcInfo> vertexNormals = this->calculateVertexNormals(model, rotatedVerticesOfTriangles);

		// Lambda

		auto hashRotatedVertex = [](const Vec3& _rotatedVertex) -> std::string {
			return "x" + std::to_string(_rotatedVertex.x) + "y" + std::to_string(_rotatedVertex.y) + "z" + std::to_string(_rotatedVertex.z);
		};

		auto getVertexNormal = [&vertexNormals, hashRotatedVertex](const Vec3& _rotatedVertex) -> Vec3 {
			return vertexNormals[hashRotatedVertex(_rotatedVertex)].normal;
		};

		// Render

		for (uint64_t trIndex = 0; trIndex < model.triangles.size(); trIndex++) {
			const Triangle& _tr = model.triangles[trIndex];
			const std::array<Vec3, 3> rotatedVertices = rotatedVerticesOfTriangles[trIndex];
			const Vec3 triangleSurfaceNormal = Triangle::getSurfaceNormal(rotatedVertices);

			// Check if triangle is facing camera
			if (!this->camera.isTriangleFacingCamera(rotatedVertices, triangleSurfaceNormal))
				continue;

			std::array<Vec3, 3> vertexNormals;

			if (_tr.isSmoothed)
				vertexNormals = { getVertexNormal(rotatedVertices[0]), getVertexNormal(rotatedVertices[1]), getVertexNormal(rotatedVertices[2]) };

			std::array<Vec3, 3> transformedVertices = Triangle::getTransformedVertices(rotatedVertices, this->camera, this->perspectiveMatrix, this->width, this->height);

			// Get the color of each vertex
			std::array<Color<>, 3> triangleBaseColors;
			
			if (_tr.isSmoothed) {
				triangleBaseColors = { _tr.vertices[0].color, _tr.vertices[1].color, _tr.vertices[2].color };
			} else {
				triangleBaseColors = {
					Light::getDiffusedLighting(rotatedVertices[0], _tr.vertices[0].color, triangleSurfaceNormal, this->lights, this->camera.position),
					Light::getDiffusedLighting(rotatedVertices[1], _tr.vertices[1].color, triangleSurfaceNormal, this->lights, this->camera.position),
					Light::getDiffusedLighting(rotatedVertices[2], _tr.vertices[2].color, triangleSurfaceNormal, this->lights, this->camera.position)
				};
			}

			// Start Rendering

			const BarycentricInterpolation bi(transformedVertices.data());

			this->drawTriangle2D(transformedVertices[0], transformedVertices[1], transformedVertices[2], [&](const uint16_t _x, const uint16_t _y) -> std::optional<Color<>> {
				// Pixel Depth (w)
				const float w = bi.interpolate(_x, _y, new float[3]{ transformedVertices[0].w, transformedVertices[1].w, transformedVertices[2].w });

				// Render Pixel if it is in front of the pixel already there
				const uint32_t pixelIndex = this->getIndexInColorBuffer(_x, _y);

				if (w < this->depthBuffer[pixelIndex] || this->depthBuffer[pixelIndex] == -1) {
					this->depthBuffer[pixelIndex] = w;

					Color<float> basePixelColor = bi.interpolate(_x, _y, new Color<float>[3] { Color<float>(triangleBaseColors[0]), Color<float>(triangleBaseColors[1]), Color<float>(triangleBaseColors[2]) });
					basePixelColor.constrain(0, 255);

					const Vec3 position = bi.interpolate(_x, _y, new Vec3[3] { rotatedVertices[0], rotatedVertices[1], rotatedVertices[2] });

					Color<> pixelColor;

					if (_tr.isSmoothed) {
						const Vec3 normal = bi.interpolate(_x, _y, new Vec3[3] { vertexNormals[0], vertexNormals[1], vertexNormals[2] });

						pixelColor = Light::getColorWithLighting(position, Color<>(basePixelColor), normal, this->lights, this->camera.position, _tr.reflectivity);
					} else {
						Color<float> pixelColorFloat = Color<float>(basePixelColor) / 255.f + Color<float>(Light::getSpecularLighting(position, triangleSurfaceNormal, this->lights, this->camera.position, _tr.reflectivity)) / 255.f;

						pixelColorFloat.constrain(0, 1);

						pixelColor = Color<>(pixelColorFloat * 255.f);
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

Renderer::Renderer(const uint16_t _width, const uint16_t _height, const Color<>& _backgroundColor, const uint8_t _fov, const float _zNear, const float _zFar) : width(_width), height(_height), fov(_fov), zNear(_zNear), zFar(_zFar) {
	std::experimental::filesystem::create_directory("./out");
	std::experimental::filesystem::create_directory("./out/frames");

	this->calculatePerspectiveMatrix();
	this->backgroundColor = _backgroundColor;
	this->depthBuffer = new float[static_cast<uint32_t>(this->width * this->height)];
}

Renderer::~Renderer() {
	delete[] this->depthBuffer;
}

inline uint32_t Renderer::getWidth()  const { return this->width; }
inline uint32_t Renderer::getHeight() const { return this->height; }

uint64_t Renderer::addLight(const Light& _light)                          { this->lights.push_back(_light); return static_cast<unsigned int>(this->lights.size() - 1); }
Light    Renderer::copyLight(const uint64_t _lightId) const               { return this->lights[_lightId]; }
Light&   Renderer::getLightRef(const uint64_t _lightId)                   { return this->lights[_lightId]; }
void     Renderer::setLight(const uint64_t _lightId, const Light& _light) { this->lights[_lightId] = _light; }

uint64_t Renderer::addModel(const Model& _model)                           { this->models.push_back(_model); return static_cast<unsigned int>(this->models.size() - 1); }
Model    Renderer::copyModel(const uint64_t _modelId) const                { return this->models[_modelId]; }
Model&   Renderer::getModelRef(const uint64_t _modelId)                    { return this->models[_modelId]; }
void     Renderer::setModel(const uint64_t _modelId, const Model& _model)  { this->models[_modelId] = _model; }

void Renderer::addModelToRenderQueue(const uint64_t _modelId) {
	this->renderQueue.push_back(_modelId);
}

inline void Renderer::drawPointNoVerif(const uint16_t _x, const uint16_t _y, const Color<>& _color) {
	this->renderImages[this->indexImageBeingRendered]->colorBuffer[getIndexInColorBuffer(_x, _y)] = _color;
}

inline void Renderer::drawPoint(const uint16_t _x, const uint16_t _y, const Color<>& _color) {
	if (_x > 0 && _x < this->width && _y > 0 && _y < this->height) {
		this->drawPointNoVerif(_x, _y, _color);
	}
}

void Renderer::drawRectangleNoVerif(const uint16_t _x, const uint16_t _y, const uint16_t _w, const uint16_t _h, const Color<>& _color) {
	for (uint16_t y = _y; y < _y + _h; y++) {
		const uint32_t baseIndex = y * this->width;

		for (uint16_t x = _x; x < _x + _w; x++)
			this->renderImages[this->indexImageBeingRendered]->colorBuffer[baseIndex + x] = _color;
	}
}

void Renderer::drawRectangle(const uint16_t _x, const uint16_t _y, const uint16_t _w, const uint16_t _h, const Color<>& _color) {
	for (uint16_t y = _y; y < ((_y + _h > this->height - 1) ? this->height : (_y + _h)); y++) {
		const uint32_t baseIndex = y * this->width;

		for (uint16_t x = _x; x < ((_x + _w > this->width - 1) ? this->width : (_x + _w)); x++)
			this->renderImages[this->indexImageBeingRendered]->colorBuffer[baseIndex + x] = _color;
	}
}

void Renderer::drawImageNoVerif(const uint16_t _x, const uint16_t _y, const Image& _image) {
	uint16_t screenX = _x, screenY = _y;
	
	for (uint16_t sampleY = 0; sampleY < _image.getHeight(); sampleY++) {
		const uint32_t baseScreenIndex = screenY * this->width;
		const uint32_t baseSampleIndex = sampleY * _image.getWidth();

		for (uint16_t sampleX = 0; sampleX < _image.getWidth(); sampleX++) {
			this->renderImages[this->indexImageBeingRendered]->colorBuffer[baseScreenIndex + screenX] = _image.sample(sampleX, sampleY);

			screenX++;
		}

		screenY++;
		screenX = _x;
	}
}

void Renderer::drawImage(const uint16_t _x, const uint16_t _y, const Image& _image) {
	uint16_t screenX = _x, screenY = _y;

	const uint16_t endY = (_y + _image.getHeight() >= this->height) ? this->height - _y - 1 : _image.getHeight();
	const uint16_t endX = (_x + _image.getWidth()  >= this->width)  ? this->width  - _x - 1 : _image.getWidth();
	
	for (uint16_t sampleY = 0; sampleY < endY; sampleY++) {
		const uint32_t baseScreenIndex = screenY * this->width;
		const uint32_t baseSampleIndex = sampleY * _image.getWidth();

		for (uint16_t sampleX = 0; sampleX < endX; sampleX++) {
			this->renderImages[this->indexImageBeingRendered]->colorBuffer[baseScreenIndex + screenX] = _image.sample(sampleX, sampleY);

			screenX++;
		}

		screenY++;
		screenX = _x;
	}
}

void Renderer::drawImageNoVerif(const uint16_t _x, const uint16_t _y, const uint16_t _width, const uint16_t _height, const Image& _image) {
	Image img(_image);

	img.resize(_width, _height);

	this->drawImageNoVerif(_x, _y, img);
}

void Renderer::drawImage(const uint16_t _x, const uint16_t _y, const uint16_t _width, const uint16_t _height, const Image& _image)
{
	Image img(_image);
	
	img.resize(_width, _height);

	this->drawImage(_x, _y, img);
}

void Renderer::drawDisk(const Vec3& _position, const uint16_t _radius, const Color<>& _color) {
	const uint32_t radiusSquared = static_cast<uint32_t>(std::pow(_radius, 2));
	const uint16_t centerX = static_cast<uint16_t>(_position.x);
	const uint16_t centerY = static_cast<uint16_t>(_position.y);

	for (int32_t deltaX = -_radius; deltaX <= _radius; deltaX++) {
		const uint64_t deltaXSquared = static_cast<uint32_t>(std::pow(deltaX, 2));

		const uint32_t x = centerX + deltaX;

		if (x < 0 || x >= this->width)
			continue;

		for (int32_t deltaY = -_radius; deltaY <= _radius; deltaY++) {
			const uint32_t y = centerY + deltaY;
			
			if (y < 0 || y >= this->height)
				continue;

			if (deltaXSquared + static_cast<uint64_t>(deltaY) * deltaY <= radiusSquared)
				this->drawPointNoVerif(x, centerY + deltaY, _color);
		}
	}
}

void Renderer::renderAndWriteFrames(const uint32_t _nFrames) {
	std::thread writeThreads[RENDERS_AND_WRITES_PER_CYCLE];

	// Allocate images
	for (uint8_t i = 0; i < RENDERS_AND_WRITES_PER_CYCLE; i++)
		this->renderImages[i] = new Image(this->width, this->height, this->backgroundColor);

	// Render And Write Frames
	for (unsigned int nCycle = 0; nCycle < std::ceil(_nFrames / (float) RENDERS_AND_WRITES_PER_CYCLE); nCycle++) {
		const uint32_t nStartFrame = nCycle * RENDERS_AND_WRITES_PER_CYCLE;
		uint32_t nCurrentFrame = nStartFrame;
		uint32_t nEndFrame     = nStartFrame + RENDERS_AND_WRITES_PER_CYCLE;

		if (nEndFrame > _nFrames)
			nEndFrame = _nFrames;

		CMD::println("\n[RENDERING / WRITING] Starting Cycle " + std::to_string(nCycle + 1) + " (" + std::to_string(nEndFrame - nStartFrame) + " frames)\n", LOG_TYPE::normal);
	
		for (this->indexImageBeingRendered = 0; this->indexImageBeingRendered < nEndFrame - nStartFrame; this->indexImageBeingRendered++) {
			const auto startTime = std::chrono::system_clock::now();
		
			this->resetDepthBuffer();
			this->renderQueue.clear();

			// Call User Defined Functions
			// Do Not Update If It Is The First Frame
			if (!(nCycle == 0 && this->indexImageBeingRendered == 0)) this->update();
			this->render();

			this->drawModels();

			const auto endTime = std::chrono::system_clock::now();
			const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

			CMD::println("[RENDERING] Rendered " + std::to_string(nCurrentFrame + 1) + " / " + std::to_string(_nFrames) + " (" + std::to_string(elapsed.count()) + "ms)", LOG_TYPE::success);

			const std::string fileName = "./out/frames/" + std::to_string(nCurrentFrame + 1) + ".ppm";

			const Image* imageBeingRenderedPtr = renderImages[this->indexImageBeingRendered];

			writeThreads[this->indexImageBeingRendered] = std::thread([nCurrentFrame, _nFrames, fileName, imageBeingRenderedPtr]() {
				const auto startTime = std::chrono::system_clock::now();

				imageBeingRenderedPtr->writeToDisk(fileName);

				const auto endTime = std::chrono::system_clock::now();
				const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

				CMD::println("[WRITING]   Wrote    " + std::to_string(nCurrentFrame + 1) + " / " + std::to_string(_nFrames) + " (" + std::to_string(elapsed.count()) + "ms)", LOG_TYPE::success);
			});

			nCurrentFrame++;
		}

		for (uint8_t i = 0; i < nEndFrame - nStartFrame; i++) {
			// Join Threads
			writeThreads[i].join();

			// Reset Images To Black
			std::fill_n(this->renderImages[i]->colorBuffer, this->width * this->height, this->backgroundColor);
		}

		std::this_thread::yield();
	}
}

void Renderer::writeVideo(const uint32_t _nFrames, const uint16_t _fps) {
	std::experimental::filesystem::create_directory("./out/video");

	Video video(true);

	for (unsigned int i = 1; i < _nFrames+1; i++)
		video.addFrame("./out/frames/" + std::to_string(i) + ".ppm");

	video.save("./out/video/video.avi", _fps);
}