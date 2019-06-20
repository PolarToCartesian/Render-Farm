#include "Renderer.h"

// Engine Class

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

// Public Methods

Renderer::Renderer(const uint16_t _width, const uint16_t _height, const Color& _backgroundColor, const uint8_t _fov, const double _zNear, const double _zFar) : width(_width), height(_height), fov(_fov), zNear(_zNear), zFar(_zFar) {
	std::experimental::filesystem::create_directory("./out");
	std::experimental::filesystem::create_directory("./out/frames");

	this->calculatePerspectiveMatrix();
	this->backgroundColor = _backgroundColor;
	this->depthBuffer = new double[static_cast<uint32_t>(this->width) * this->height];
}

Renderer::~Renderer() {
	delete[] this->depthBuffer;
}

inline uint32_t Renderer::getWidth()  const { return this->width; }
inline uint32_t Renderer::getHeight() const { return this->height; }

inline uint64_t Renderer::addLight(const Light& _light)                          { this->lights.push_back(_light); return static_cast<unsigned int>(this->lights.size() - 1); }
inline Light    Renderer::copyLight(const uint16_t _lightId) const               { return this->lights[_lightId]; }
inline Light&   Renderer::getLightRef(const uint16_t _lightId)                   { return this->lights[_lightId]; }
inline void     Renderer::setLight(const uint16_t _lightId, const Light& _light) { this->lights[_lightId] = _light; }

inline uint64_t Renderer::addModel(const Model& _model)                          { this->models.push_back(_model); return static_cast<unsigned int>(this->models.size() - 1); }
inline Model    Renderer::copyModel(const uint16_t _modelId) const               { return this->models[_modelId]; }
inline Model&   Renderer::getModelRef(const uint16_t _modelId)                   { return this->models[_modelId]; }
inline void     Renderer::setModel(const uint16_t _modelId, const Model _model)  { this->models[_modelId] = _model; }

inline void Renderer::drawModel(const uint16_t _modelId) {
	this->models[_modelId].applyFunctionToEachTriangle([this](Triangle& _tr) {
		this->drawTriangle3D(_tr);
	});
}

inline void Renderer::drawPointNoVerif(const uint16_t _x, const uint16_t _y, const Color& _color) {
	renderImages[this->indexImageBeingRendered]->colorBuffer[getIndexInColorBuffer(_x, _y)] = _color;
}

inline void Renderer::drawPoint(const uint16_t _x, const uint16_t _y, const Color& _color) {
	if (_x > 0 && _x < this->width && _y > 0 && _y < this->height) {
		drawPointNoVerif(_x, _y, _color);
	}
}

void Renderer::drawRectangleNoVerif(const uint16_t _x, const uint16_t _y, const uint16_t _w, const uint16_t _h, const Color& _color) {
	for (uint16_t y = _y; y < _y + _h; y++) {
		const uint32_t baseIndex = y * this->width;

		for (uint16_t x = _x; x < _x + _w; x++) {
			renderImages[this->indexImageBeingRendered]->colorBuffer[baseIndex + x] = _color;
		}
	}
}

void Renderer::drawRectangle(const uint16_t _x, const uint16_t _y, const uint16_t _w, const uint16_t _h, const Color& _color) {
	for (uint16_t y = _y; y < ((_y + _h > this->height - 1) ? this->height : (_y + _h)); y++) {
		const uint32_t baseIndex = y * this->width;

		for (uint16_t x = _x; x < ((_x + _w > this->width - 1) ? this->width : (_x + _w)); x++) {
			renderImages[this->indexImageBeingRendered]->colorBuffer[baseIndex + x] = _color;
		}
	}
}

void Renderer::drawImageNoVerif(const uint16_t _x, const uint16_t _y, const Image& _image) {
	uint16_t sampleX = 0, sampleY = 0;
	
	for (uint16_t y = _y; y < _y + _image.getHeight(); y++) {
		const uint32_t baseIndex = y * this->width;

		for (uint16_t x = _x; x < _x + _image.getWidth(); x++) {
			renderImages[this->indexImageBeingRendered]->colorBuffer[baseIndex + x] = _image.sample(sampleX, sampleY);
			sampleX++;
		}

		sampleX = 0;
		sampleY++;
	}
}

void Renderer::drawImage(const uint16_t _x, const uint16_t _y, const Image& _image) {
	uint16_t xEnd = (_x + _image.getWidth()  > this->getWidth()  - 1) ? (this->width)  : (_x + _image.getWidth() );
	uint16_t yEnd = (_y + _image.getHeight() > this->getHeight() - 1) ? (this->height) : (_y + _image.getHeight());

	uint16_t sampleX = 0, sampleY = 0;

	for (uint16_t y = _y; y < yEnd; y++) {
		uint32_t baseIndex = y * this->width;

		for (uint16_t x = _x; x < xEnd; x++) {
			renderImages[this->indexImageBeingRendered]->colorBuffer[baseIndex + x] = _image.sample(sampleX, sampleY);
			sampleX++;
		}

		sampleX = 0;
		sampleY++;
	}
}

// Renders a triangle in 3D
void Renderer::drawTriangle3D(const Triangle& _tr) {
	Vec3 rotatedPoints[3] = { _tr.vertices[0].position, _tr.vertices[1].position, _tr.vertices[2].position };

	const Mat4x4 cameraRotationXMatrix = Mat4x4::getRotationXMatrix(-this->camera.rotation.x);
	const Mat4x4 cameraRotationYMatrix = Mat4x4::getRotationYMatrix(-this->camera.rotation.y);
	const Mat4x4 cameraRotationZMatrix = Mat4x4::getRotationZMatrix(-this->camera.rotation.z);

	const Mat4x4 triangleRotationXMatrix = Mat4x4::getRotationXMatrix(_tr.rotation.x);
	const Mat4x4 triangleRotationYMatrix = Mat4x4::getRotationYMatrix(_tr.rotation.y);
	const Mat4x4 triangleRotationZMatrix = Mat4x4::getRotationZMatrix(_tr.rotation.z);

	// For Every Vertex
	for (uint8_t v = 0; v < 3; v++) {
		// Rotate Triangle Around Point
		rotatedPoints[v] -= _tr.rotationMidPoint;

		// Rotate Triangle Around Point
		rotatedPoints[v] *= triangleRotationXMatrix;
		rotatedPoints[v] *= triangleRotationYMatrix;
		rotatedPoints[v] *= triangleRotationZMatrix;

		// Rotate Triangle Around Point
		rotatedPoints[v] += _tr.rotationMidPoint;

		// Apply Camera Rotation
		rotatedPoints[v] -= this->camera.position;
		rotatedPoints[v] += this->camera.position + rotatedPoints[v] * (-3) + rotatedPoints[v] * cameraRotationXMatrix
																			+ rotatedPoints[v] * cameraRotationYMatrix
																			+ rotatedPoints[v] * cameraRotationZMatrix;
	}

	Vec3 triangleSurfaceNormal = Triangle::getSurfaceNormal(rotatedPoints);

	// Check if triangle is facing camera
	if (!this->camera.isTriangleFacingCamera(rotatedPoints, triangleSurfaceNormal)) return;

	Vec3 transformedPoints[3] = { rotatedPoints[0], rotatedPoints[1], rotatedPoints[2] };

	for (uint8_t v = 0; v < 3; v++) {
		// Camera Translation
		transformedPoints[v] -= this->camera.position;

		// Apply Perspective
		transformedPoints[v] = transformedPoints[v] * this->perspectiveMatrix;

		if (transformedPoints[v].w < 0) return;
		if (transformedPoints[v].w > 0) transformedPoints[v] /= transformedPoints[v].w;

		// -1 to 1    to    0-width
		transformedPoints[v].x = ((transformedPoints[v].x - 1.f) / -2.f) * this->width;
		// 1  to -1   to    height-0
		transformedPoints[v].y = ((transformedPoints[v].y + 1.f) / +2.f) * this->height;
	}

	const Color triangleColors[3] = { _tr.vertices[0].color, _tr.vertices[1].color, _tr.vertices[2].color };
	const std::array<Color, 3> lightenedVertexColors = Light::applyLightingToVertices(rotatedPoints, triangleColors, triangleSurfaceNormal, this->lights);

	// Start Rendering
	// PreCalculate Values(For Barycentric Interpolation)
	// Thanks to : https://codeplea.com/triangular-interpolation

	const double denominator = (transformedPoints[1].y - transformedPoints[2].y) * (transformedPoints[0].x - transformedPoints[2].x) + (transformedPoints[2].x - transformedPoints[1].x) * (transformedPoints[0].y - transformedPoints[2].y);
	double precalculated[6]  = { (transformedPoints[1].y - transformedPoints[2].y),  (transformedPoints[2].x - transformedPoints[1].x),  (transformedPoints[2].y - transformedPoints[0].y),  (transformedPoints[0].x - transformedPoints[2].x), 0, 0 };

	// For the basic Renderer (thx) https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling

	Vec3 t0 = Vec3::intify(transformedPoints[0]);
	Vec3 t1 = Vec3::intify(transformedPoints[1]);
	Vec3 t2 = Vec3::intify(transformedPoints[2]);

	if (t0.y == t1.y && t0.y == t2.y) return;

	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	uint16_t total_height = static_cast<uint16_t>(t2.y - t0.y);

	// Render Triangle With The 3 Triangulated :D colors
	for (uint16_t i = 0; i < total_height; i++) {
		uint16_t y = static_cast<uint16_t>(t0.y + i);

		if (y < 0 || y > this->height - 1) continue;

		const bool second_half   = i > t1.y - t0.y || t1.y == t0.y;
		const int segment_height = static_cast<int>(second_half ? t2.y - t1.y : t1.y - t0.y);

		const double alpha = i / static_cast<double>(total_height);
		const double beta  = static_cast<double>(i - (second_half ? t1.y - t0.y : 0)) / segment_height;

		Vec3 A = t0 + (t2 - t0) * alpha;
		Vec3 B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;

		if (A.x > B.x) std::swap(A, B);

		precalculated[5] = (y - transformedPoints[2].y);

		// Limit X Between (0 and width - 1) in the loop to avoid trying to set a pixel out of the screen buffer and to maximize performance
		for (int j = static_cast<int>((A.x < 0) ? 0 : A.x); j <= static_cast<int>((B.x > this->width - 2) ? this->width - 2 : B.x); j++) {
			const uint16_t x = j;
			
			precalculated[4] = (x - transformedPoints[2].x);

			double VertexPositionWeights[3] = { (precalculated[0] * precalculated[4] + precalculated[1] * precalculated[5]) / denominator, (precalculated[2] * precalculated[4] + precalculated[3] * precalculated[5]) / denominator, 0 };
			VertexPositionWeights[2]        = 1 - VertexPositionWeights[0] - VertexPositionWeights[1];
			const double VertexPositionWeightSum  = VertexPositionWeights[0] + VertexPositionWeights[1] + VertexPositionWeights[2];

			// Pixel Depth (w)
			double w = 0;

			// For every vertex (Barycentric Interpolation)
			for (uint8_t c = 0; c < 3; c++) {
				w += transformedPoints[c].w * VertexPositionWeights[c];
			}

			// Calculate W (depth of pixel)
			w /= VertexPositionWeightSum;

			// Render Pixel if it is in front of the pixel already there
			const uint32_t pixelIndex = this->getIndexInColorBuffer(x, y);

			if (w < this->depthBuffer[pixelIndex] || this->depthBuffer[pixelIndex] == -1) {
				this->depthBuffer[pixelIndex] = w;

				// Triangulate :D the pixel color

				double r = 0, g = 0, b = 0;

				// For every vertex
				for (uint8_t c = 0; c < 3; c++) {
					r += lightenedVertexColors[c].r * VertexPositionWeights[c];
					g += lightenedVertexColors[c].g * VertexPositionWeights[c];
					b += lightenedVertexColors[c].b * VertexPositionWeights[c];
				}

				r /= VertexPositionWeightSum;
				g /= VertexPositionWeightSum;
				b /= VertexPositionWeightSum;

				const Color pixelColor(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b));

				renderImages[this->indexImageBeingRendered]->colorBuffer[pixelIndex] = pixelColor;
			}
		}
	}
}

void Renderer::drawDisk(const Vec3& _position, const uint16_t _radius, const Color& _color) {
	const uint32_t radiusSquared = _radius * _radius;
	const uint16_t centerX = static_cast<uint16_t>(_position.x);
	const uint16_t centerY = static_cast<uint16_t>(_position.y);

	for (int32_t deltaX = -_radius; deltaX <= _radius; deltaX++) {
		const uint64_t deltaXSquared = static_cast<uint64_t>(deltaX) * deltaX;

		const uint32_t x = centerX + deltaX;

		if (x < 0 || x >= this->width) continue;

		for (int32_t deltaY = -_radius; deltaY <= _radius; deltaY++) {
			const uint32_t y = centerY + deltaY;
			
			if (y < 0 || y >= this->height) continue;

			if (deltaXSquared + static_cast<uint64_t>(deltaY) * deltaY <= radiusSquared) {
				Color pixelColor = _color;

				this->drawPointNoVerif(x, centerY + deltaY, _color);
			}
		}
	}
}

void Renderer::drawSphere(const Vec3& _position, const uint16_t _radius, const Color& _color) {
	// Ray Marching : http://jamie-wong.com/2016/07/15/ray-marching-signed-distance-functions/#signed-distance-functions

	
}

void Renderer::renderAndWriteFrames(const uint32_t _nFrames) {
	std::thread writeThreads[RENDERS_AND_WRITES_PER_CYCLE];

	// Allocate images
	for (unsigned int i = 0; i < RENDERS_AND_WRITES_PER_CYCLE; i++) {
		this->renderImages[i] = new Image(this->width, this->height, this->backgroundColor);
	}

	// Render And Write Frames
	for (unsigned int nCycle = 0; nCycle < std::ceil(_nFrames / (double) RENDERS_AND_WRITES_PER_CYCLE); nCycle++) {
		unsigned int nStartFrame   = nCycle * RENDERS_AND_WRITES_PER_CYCLE;
		unsigned int nCurrentFrame = nStartFrame;
		unsigned int nEndFrame     = nStartFrame + RENDERS_AND_WRITES_PER_CYCLE;
		if (nEndFrame > _nFrames) nEndFrame = _nFrames;

		LOG::println("\n[RENDERING / WRITING] Starting Cycle " + std::to_string(nCycle + 1) + " (" + std::to_string(nEndFrame - nStartFrame) + " frames)\n", LOG_TYPE::normal);
	
		for (this->indexImageBeingRendered = 0; this->indexImageBeingRendered < nEndFrame - nStartFrame; this->indexImageBeingRendered++) {
			auto startTime = std::chrono::system_clock::now();
		
			this->resetDepthBuffer();

			// Call User Defined Functions
			this->update();
			this->render();

			auto endTime = std::chrono::system_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

			LOG::println("[RENDERING] Rendered " + std::to_string(nCurrentFrame + 1) + " / " + std::to_string(_nFrames) + " (" + std::to_string(elapsed.count()) + "ms)", LOG_TYPE::success);

			std::string fileName = "./out/frames/" + std::to_string(nCurrentFrame + 1) + ".ppm";

			const Image* imageBeingRenderedPtr = renderImages[this->indexImageBeingRendered];

			writeThreads[this->indexImageBeingRendered] = std::thread([nCurrentFrame, &_nFrames, &fileName, &imageBeingRenderedPtr]() {
				auto startTime = std::chrono::system_clock::now();

				imageBeingRenderedPtr->writeToDisk(fileName);

				auto endTime   = std::chrono::system_clock::now();
				auto elapsed   = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

				LOG::println("[WRITING]   Wrote    " + std::to_string(nCurrentFrame + 1) + " / " + std::to_string(_nFrames) + " (" + std::to_string(elapsed.count()) + "ms)", LOG_TYPE::success);
			});

			nCurrentFrame++;
		}

		for (unsigned int i = 0; i < nEndFrame - nStartFrame; i++) {
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

	for (unsigned int i = 1; i < _nFrames+1; i++) {
		video.addFrame("./out/frames/" + std::to_string(i) + ".ppm");
	}

	video.save("./out/video/video.avi", _fps);
}