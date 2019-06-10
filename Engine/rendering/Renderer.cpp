#include "Renderer.h"

// Engine Class

// Private Methods

inline void Renderer::resetDepthBuffer() {
	std::fill_n(this->depthBuffer, this->width * this->height, -1.f);
}

unsigned int Renderer::getIndexInColorBuffer(const unsigned int& _x, const unsigned int& _y) {
	return renderImages[this->indexImageBeingRendered]->getIndex(_x, _y);
}

void Renderer::calculatePerspectiveMatrix() {
	this->perspectiveMatrix = Mat4x4::getPerspectiveMatrix(this->width, this->height, this->fov, this->zNear, this->zFar);
}

// Public Methods

Renderer::Renderer(const unsigned int& _width, const unsigned int& _height, const Color& _backgroundColor, const unsigned int& _fov, const double& _zNear, const double& _zFar) : width(_width), height(_height), fov(_fov), zNear(_zNear), zFar(_zFar) {
	std::experimental::filesystem::create_directory("./out");
	std::experimental::filesystem::create_directory("./out/frames");

	this->calculatePerspectiveMatrix();
	this->backgroundColor = _backgroundColor;
	this->depthBuffer = new double[this->width * this->height];
}

Renderer::~Renderer() {
	delete[] this->depthBuffer;
}

unsigned int Renderer::getWidth()  const { return this->width; }
unsigned int Renderer::getHeight() const { return this->height; }

unsigned int Renderer::addLight(const Light& _light)                               { this->lights.push_back(_light); return static_cast<unsigned int>(this->lights.size() - 1); }
Light        Renderer::copyLight(const unsigned int& _lightId) const               { return this->lights[_lightId]; }
Light&       Renderer::getLightRef(const unsigned int& _lightId)                   { return this->lights[_lightId]; }
void         Renderer::setLight(const unsigned int& _lightId, const Light& _light) { this->lights[_lightId] = _light; }

unsigned int Renderer::addModel(const Model & _model)                              { this->models.push_back(_model); return static_cast<unsigned int>(this->models.size() - 1); }
Model        Renderer::copyModel(const unsigned int& _modelId) const               { return this->models[_modelId]; }
Model&       Renderer::getModelRef(const unsigned int& _modelId)                   { return this->models[_modelId]; }
void         Renderer::setModel(const unsigned int& _modelId, const Model _model)  { this->models[_modelId] = _model; }

void Renderer::drawPointNoVerif(const unsigned int& _x, const unsigned int& _y, const Color & _color) {
	renderImages[this->indexImageBeingRendered]->colorBuffer[getIndexInColorBuffer(_x, _y)] = _color;
}

void Renderer::drawPoint(const unsigned int& _x, const unsigned int& _y, const Color & _color) {
	if (_x > 0 && _x < this->width && _y > 0 && _y < this->height) {
		drawPointNoVerif(_x, _y, _color);
	}
}

void Renderer::drawRectangleNoVerif(const unsigned int& _x, const unsigned int& _y, const unsigned int& _w, const unsigned int& _h, const Color & _color) {
	for (unsigned int y = _y; y < _y + _h; y++) {
		unsigned int baseIndex = y * this->width;

		for (unsigned int x = _x; x < _x + _w; x++) {
			renderImages[this->indexImageBeingRendered]->colorBuffer[baseIndex + x] = _color;
		}
	}
}

void Renderer::drawRectangle(const unsigned int& _x, const unsigned int& _y, const unsigned int& _w, const unsigned int& _h, const Color & _color) {
	for (unsigned int y = _y; y < ((_y + _h > this->height - 1) ? this->height : (_y + _h)); y++) {
		unsigned int baseIndex = y * this->width;

		for (unsigned int x = _x; x < ((_x + _w > this->width - 1) ? this->width : (_x + _w)); x++) {
			renderImages[this->indexImageBeingRendered]->colorBuffer[baseIndex + x] = _color;
		}
	}
}

void Renderer::drawImageNoVerif(const unsigned int& _x, const unsigned int& _y, const Image& _image) {
	unsigned int sampleX = 0, sampleY = 0;
	
	for (unsigned int y = _y; y < _y + _image.getHeight(); y++) {
		unsigned int baseIndex = y * this->width;

		for (unsigned int x = _x; x < _x + _image.getWidth(); x++) {
			renderImages[this->indexImageBeingRendered]->colorBuffer[baseIndex + x] = _image.sample(sampleX, sampleY);
			sampleX++;
		}

		sampleX = 0;
		sampleY++;
	}
}

void Renderer::drawImage(const unsigned int& _x, const unsigned int& _y, const Image& _image) {
	unsigned int xEnd = (_x + _image.getWidth()  > this->getWidth()  - 1) ? (this->width)  : (_x + _image.getWidth() );
	unsigned int yEnd = (_y + _image.getHeight() > this->getHeight() - 1) ? (this->height) : (_y + _image.getHeight());

	unsigned int sampleX = 0, sampleY = 0;

	for (unsigned int y = _y; y < yEnd; y++) {
		unsigned int baseIndex = y * this->width;

		for (unsigned int x = _x; x < xEnd; x++) {
			renderImages[this->indexImageBeingRendered]->colorBuffer[baseIndex + x] = _image.sample(sampleX, sampleY);
			sampleX++;
		}

		sampleX = 0;
		sampleY++;
	}
}

// Renders a triangle in 3D
void Renderer::drawTriangle3D(const Triangle& _tr) {
	Vec3 rotatedVertices[3] = { 0 };
	std::memcpy(rotatedVertices, _tr.vertices, 3 * sizeof(Vec3));

	// For Every Vertex
	for (unsigned char v = 0; v < 3; v++) {
		// Rotate Triangle Around Point
		rotatedVertices[v] -= _tr.rotationMidPoint;

		// Rotate
		rotatedVertices[v] *= Mat4x4::getRotationXMatrix(_tr.rotation.x);
		rotatedVertices[v] *= Mat4x4::getRotationYMatrix(_tr.rotation.y);
		rotatedVertices[v] *= Mat4x4::getRotationZMatrix(_tr.rotation.z);

		// Rotate Triangle Around Point
		rotatedVertices[v] += _tr.rotationMidPoint;
	}

	Vec3 triangleSurfaceNormal = Triangle::getSurfaceNormal(rotatedVertices);

	// Check if triangle is facing camera
	if (!this->camera.isTriangleFacingCamera(rotatedVertices, triangleSurfaceNormal)) return;

	Vec3 transformedVertices[3] = { 0 };
	std::memcpy(transformedVertices, rotatedVertices, 3 * sizeof(Vec3));

	for (unsigned char v = 0; v < 3; v++) {
		// Camera Translation
		transformedVertices[v] -= this->camera.position;

		// Apply Perspective
		transformedVertices[v] = transformedVertices[v] * this->perspectiveMatrix;

		if (transformedVertices[v].w < 0) return;
		if (transformedVertices[v].w > 0) transformedVertices[v] /= transformedVertices[v].w;

		// -1 to 1    to    0-width
		transformedVertices[v].x = ((transformedVertices[v].x - 1.f) / -2.f) * this->width;
		// 1  to -1   to    height-0
		transformedVertices[v].y = ((transformedVertices[v].y + 1.f) / +2.f) * this->height;
	}

	std::array<Color, 3> lightenedVertexColors = EN::LIGHT::applyLightingToVertices(rotatedVertices, _tr.colors, triangleSurfaceNormal, this->lights);

	// Start Rendering
	// PreCalculate Values(For Barycentric Interpolation)
	// Thanks to : https://codeplea.com/triangular-interpolation

	double denominator      = (transformedVertices[1].y - transformedVertices[2].y) * (transformedVertices[0].x - transformedVertices[2].x) + (transformedVertices[2].x - transformedVertices[1].x) * (transformedVertices[0].y - transformedVertices[2].y);
	double precalculated[6] = { (transformedVertices[1].y - transformedVertices[2].y),  (transformedVertices[2].x - transformedVertices[1].x),  (transformedVertices[2].y - transformedVertices[0].y),  (transformedVertices[0].x - transformedVertices[2].x), 0, 0 };

	// For the basic Renderer (thx) https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling

	Vec3 t0 = Vec3::intify(transformedVertices[0]);
	Vec3 t1 = Vec3::intify(transformedVertices[1]);
	Vec3 t2 = Vec3::intify(transformedVertices[2]);

	if (t0.y == t1.y && t0.y == t2.y) return;

	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	unsigned int total_height = static_cast<unsigned int>(t2.y - t0.y);

	// Render Triangle With The 3 Triangulated :D colors
	for (unsigned int i = 0; i < static_cast<unsigned int>(total_height); i++) {
		int y = static_cast<unsigned int>(t0.y + i);

		if (y < 0 || y > this->height - 1) continue;

		bool second_half   = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = static_cast<int>(second_half ? t2.y - t1.y : t1.y - t0.y);

		double alpha = (double) i / total_height;
		double beta  = (double) (i - (second_half ? t1.y - t0.y : 0)) / segment_height;

		Vec3 A = t0 + (t2 - t0) * alpha;
		Vec3 B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;

		if (A.x > B.x) std::swap(A, B);

		// Limit X Between (0 and width - 1) in the loop to avoid trying to set a pixel out of the screen buffer and to maximize performance
		for (int j = static_cast<int>((A.x < 0) ? 0 : A.x); j <= static_cast<int>((B.x > this->width - 2) ? this->width - 2 : B.x); j++) {
			unsigned int x = j;

			// Continue Barycentric Interpolation
			// To calculate the w value (depth of the pixel to dicide if we should render it or not)

			precalculated[4] = (x - transformedVertices[2].x);
			precalculated[5] = (y - transformedVertices[2].y);

			double VertexPositionWeights[3] = { (precalculated[0] * precalculated[4] + precalculated[1] * precalculated[5]) / denominator, (precalculated[2] * precalculated[4] + precalculated[3] * precalculated[5]) / denominator, 0 };
			VertexPositionWeights[2]        = 1 - VertexPositionWeights[0] - VertexPositionWeights[1];
			double VertexPositionWeightSum  = VertexPositionWeights[0] + VertexPositionWeights[1] + VertexPositionWeights[2];

			// Pixel Depth (w)
			double w = 0;

			// For every vertex (Barycentric Interpolation)
			for (unsigned char c = 0; c < 3; c++) {
				w += transformedVertices[c].w * VertexPositionWeights[c];
			}

			// Calculate W (depth of pixel)
			w /= VertexPositionWeightSum;

			// Render Pixel if it is in front of the pixel already there
			unsigned int pixelIndex = this->getIndexInColorBuffer(x, y);

			if (w < this->depthBuffer[pixelIndex] || this->depthBuffer[pixelIndex] == -1) {
				this->depthBuffer[pixelIndex] = w;

				// Triangulate :D the pixel color

				double r = 0, g = 0, b = 0;

				// For every vertex
				for (unsigned char c = 0; c < 3; c++) {
					r += lightenedVertexColors[c].r * VertexPositionWeights[c];
					g += lightenedVertexColors[c].g * VertexPositionWeights[c];
					b += lightenedVertexColors[c].b * VertexPositionWeights[c];
				}

				r /= VertexPositionWeightSum;
				g /= VertexPositionWeightSum;
				b /= VertexPositionWeightSum;

				Color pixelColor(static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b));

				renderImages[this->indexImageBeingRendered]->colorBuffer[pixelIndex] = pixelColor;
			}
		}
	}
}

void Renderer::renderAndWriteFrames(const unsigned int& _nFrames) {
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
			this->render(false);

			// Render Every Model
			for (unsigned int nModel = 0; nModel < this->models.size(); nModel++) {
				if (this->models[nModel].doRender) {
					this->models[nModel].applyFunctionToEachTriangle([this](Triangle& _tr) {
						this->drawTriangle3D(_tr);
					});
				}
			}

			// Call User Defined Functions
			this->render(true);

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
	}
}

void Renderer::writeVideo(const unsigned int& _nFrames, const unsigned int& _fps) {
	std::experimental::filesystem::create_directory("./out/video");

	Video video(true);

	for (unsigned int i = 1; i < _nFrames+1; i++) {
		video.addFrame("./out/frames/" + std::to_string(i) + ".ppm");
	}

	video.save("./out/video/video.avi", _fps);
}