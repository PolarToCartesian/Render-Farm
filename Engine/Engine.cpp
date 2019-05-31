#include "Engine.h"

// Engine Class

// Private Methods

void Engine::resetDepthBuffer() {
	std::fill_n(this->depthBuffer, this->width * this->height, -1.f);
}

unsigned int Engine::getIndexInColorBuffer(const unsigned int& _x, const unsigned int& _y) {
	return renderImages[this->indexImageBeingRendered]->getIndex(_x, _y);
}

void Engine::calculatePerspectiveMatrix() {
	this->perspectiveMatrix = EN::MATRIX4X4::getPerspectiveMatrix(this->width, this->height, this->fov, this->zNear, this->zFar);
}

// Public Methods

Engine::Engine(const unsigned int& _width, const unsigned int& _height, const unsigned int& _fov, const TYPE& _zNear, const TYPE& _zFar) : width(_width), height(_height), fov(_fov), zNear(_zNear), zFar(_zFar) {
	std::experimental::filesystem::create_directory("./out");
	std::experimental::filesystem::create_directory("./out/frames");
	std::experimental::filesystem::create_directory("./out/video");

	this->calculatePerspectiveMatrix();

	this->depthBuffer = new TYPE[this->width * this->height];
}

Engine::~Engine() {
	delete[] this->depthBuffer;
}

unsigned int Engine::getWidth()  const { return this->width; }
unsigned int Engine::getHeight() const { return this->height; }

unsigned int Engine::addLight(const Light& _light) { this->lights.push_back(_light); return static_cast<unsigned int>(this->lights.size() - 1); }
Light Engine::copyLight(const unsigned int& _lightId) { return this->lights[_lightId]; }
void Engine::setLight(const unsigned int& _lightId, const Light _light) { this->lights[_lightId] = _light; }

unsigned int Engine::addModel(const Model & _model) { this->models.push_back(_model); return static_cast<unsigned int>(this->models.size() - 1); }
Model Engine::copyModel(const unsigned int& _modelId) const { return this->models[_modelId]; }
void Engine::setModel(const unsigned int& _modelId, const Model _model) { this->models[_modelId] = _model; }

void Engine::drawPointNoVerif(const unsigned int& _x, const unsigned int& _y, const Color & _color) {
	renderImages[this->indexImageBeingRendered]->colorBuffer[getIndexInColorBuffer(_x, _y)] = _color;
}

void Engine::drawPoint(const unsigned int& _x, const unsigned int& _y, const Color & _color) {
	if (_x > 0 && _x < this->width && _y > 0 && _y < this->height) {
		drawPointNoVerif(_x, _y, _color);
	}
}

void Engine::drawRectangleNoVerif(const unsigned int& _x, const unsigned int& _y, const unsigned int& _w, const unsigned int& _h, const Color & _color) {
	for (unsigned int y = _y; y < _y + _h; y++) {
		unsigned int baseIndex = y * this->width;

		for (unsigned int x = _x; x < _x + _w; x++) {
			renderImages[this->indexImageBeingRendered]->colorBuffer[baseIndex + x] = _color;
		}
	}
}

void Engine::drawRectangle(const unsigned int& _x, const unsigned int& _y, const unsigned int& _w, const unsigned int& _h, const Color & _color) {
	for (unsigned int y = (_y >= 0) ? _y : 0; y < ((_y + _h < this->height - 1) ? (_y + _h) : (this->height - 1)); y++) {
		unsigned int baseIndex = y * this->width;

		for (unsigned int x = (_x >= 0) ? _x : 0; x < ((_x + _w < this->width) ? (_x + _w) : (this->width - 1)); x++) {
			renderImages[this->indexImageBeingRendered]->colorBuffer[baseIndex + x] = _color;
		}
	}
}

// Renders a triangle in 3D
void Engine::drawTriangle3D(const Triangle& _tr) {	
	Vector3D rotatedVertices[3] = { 0 };
	std::memcpy(rotatedVertices, _tr.vertices, 3 * sizeof(Vector3D));

	// For Every Vertex
	for (unsigned char v = 0; v < 3; v++) {
		// Rotate Triangle Around Point
		rotatedVertices[v] -= _tr.rotationMidPoint;

		// Rotate
		rotatedVertices[v] *= EN::MATRIX4X4::getRotationXMatrix(_tr.rotation.x);
		rotatedVertices[v] *= EN::MATRIX4X4::getRotationYMatrix(_tr.rotation.y);
		rotatedVertices[v] *= EN::MATRIX4X4::getRotationZMatrix(_tr.rotation.z);

		// Rotate Triangle Around Point
		rotatedVertices[v] += _tr.rotationMidPoint;
	}

	Vector3D triangleSurfaceNormal = EN::TRIANGLE::getSurfaceNormal(rotatedVertices);

	// Check if triangle is facing camera
	if (!this->camera.isTriangleFacingCamera(rotatedVertices, triangleSurfaceNormal)) return;

	Vector3D transformedVertices[3] = { 0 };
	std::memcpy(transformedVertices, rotatedVertices, 3 * sizeof(Vector3D));

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

	Color lightenedVertexColors[3];
	EN::LIGHT::applyLightingToVertices(rotatedVertices, _tr.colors, triangleSurfaceNormal, this->lights, lightenedVertexColors);

	// Start Rendering
	// PreCalculate Values(For Barycentric Interpolation)
	// Thanks to : https://codeplea.com/triangular-interpolation

	TYPE denominator      = (transformedVertices[1].y - transformedVertices[2].y) * (transformedVertices[0].x - transformedVertices[2].x) + (transformedVertices[2].x - transformedVertices[1].x) * (transformedVertices[0].y - transformedVertices[2].y);
	TYPE precalculated[6] = { (transformedVertices[1].y - transformedVertices[2].y),  (transformedVertices[2].x - transformedVertices[1].x),  (transformedVertices[2].y - transformedVertices[0].y),  (transformedVertices[0].x - transformedVertices[2].x), 0, 0 };

	// For the basic Renderer (thx) https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling

	Vector3D t0 = EN::VECTOR3D::intify(transformedVertices[0]);
	Vector3D t1 = EN::VECTOR3D::intify(transformedVertices[1]);
	Vector3D t2 = EN::VECTOR3D::intify(transformedVertices[2]);

	if (t0.y == t1.y && t0.y == t2.y) return;

	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	unsigned int total_height = static_cast<unsigned int>(t2.y - t0.y);

	// Render Triangle With The 3 Triangulated :D colors
	for (unsigned int i = 0; i < static_cast<unsigned int>(total_height); i++) {
		unsigned int  y = static_cast<unsigned int>(t0.y + i);

		if (y < 0 || y > this->height - 1) continue;

		bool second_half   = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = static_cast<int>(second_half ? t2.y - t1.y : t1.y - t0.y);

		TYPE alpha = (TYPE) i / total_height;
		TYPE beta  = (TYPE) (i - (second_half ? t1.y - t0.y : 0)) / segment_height;

		Vector3D A = t0 + (t2 - t0) * alpha;
		Vector3D B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;

		if (A.x > B.x) std::swap(A, B);

		// Limit X Between (0 and width - 1) in the loop to avoid trying to set a pixel out of the screen buffer and to maximize performance
		for (int j = static_cast<int>((A.x < 0) ? 0 : A.x); j <= static_cast<int>((B.x > this->width - 2) ? this->width - 2 : B.x); j++) {
			unsigned int x = j;

			// Continue Barycentric Interpolation
			// To calculate the w value (depth of the pixel to dicide if we should render it or not)

			precalculated[4] = (x - transformedVertices[2].x);
			precalculated[5] = (y - transformedVertices[2].y);

			TYPE VertexPositionWeights[3] = { (precalculated[0] * precalculated[4] + precalculated[1] * precalculated[5]) / denominator, (precalculated[2] * precalculated[4] + precalculated[3] * precalculated[5]) / denominator, 0 };
			VertexPositionWeights[2]      = 1 - VertexPositionWeights[0] - VertexPositionWeights[1];
			TYPE VertexPositionWeightSum  = VertexPositionWeights[0] + VertexPositionWeights[1] + VertexPositionWeights[2];

			// Pixel Depth (w)
			TYPE w = 0;

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

				Color pixelColor(0);

				// For every vertex
				for (unsigned char c = 0; c < 3; c++) {
					pixelColor += lightenedVertexColors[c] * VertexPositionWeights[c];
				}

				pixelColor /= VertexPositionWeightSum;
				pixelColor.constrain(0, 255);
				pixelColor.intify();

				renderImages[this->indexImageBeingRendered]->colorBuffer[pixelIndex] = pixelColor;
			}
		}
	}
}

// Renders And Writes to a folder with the name of the title given in the constructor the selected number of frames.
void Engine::renderAndWriteFrames(const unsigned int& _nFrames) {
	std::thread writeThreads[RENDERS_AND_WRITES_PER_CYCLE];

	// Allocate images
	for (unsigned int i = 0; i < RENDERS_AND_WRITES_PER_CYCLE; i++) {
		this->renderImages[i] = new Image(this->width, this->height);
	}

	// Render And Write Frames
	for (unsigned int nCycle = 0; nCycle < std::ceil(_nFrames / (double) RENDERS_AND_WRITES_PER_CYCLE); nCycle++) {
		unsigned int nStartFrame   = nCycle * RENDERS_AND_WRITES_PER_CYCLE;
		unsigned int nCurrentFrame = nStartFrame;
		unsigned int nEndFrame     = nStartFrame + RENDERS_AND_WRITES_PER_CYCLE;
		if (nEndFrame > _nFrames) nEndFrame = _nFrames;

		EN::LOG::println("\n[RENDERING / WRITING] Starting Cycle " + std::to_string(nCycle + 1) + " (" + std::to_string(nEndFrame - nStartFrame) + " frames)\n", LOG_TYPE::normal);
	
		for (this->indexImageBeingRendered = 0; this->indexImageBeingRendered < nEndFrame - nStartFrame; this->indexImageBeingRendered++) {
			auto startTime = std::chrono::system_clock::now();
		
			// Call User Defined Functions
			this->render();
			this->update();

			// Render Every Model
			for (unsigned int nModel = 0; nModel < this->models.size(); nModel++) {
				if (this->models[nModel].doRender) {
					this->models[nModel].applyFunctionToEachTriangle([this](Triangle& _tr) {
						this->drawTriangle3D(_tr);
					});
				}
			}

			auto endTime = std::chrono::system_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

			EN::LOG::println("[RENDERING] Rendered " + std::to_string(nCurrentFrame + 1) + " / " + std::to_string(_nFrames) + " (" + std::to_string(elapsed.count()) + "ms)", LOG_TYPE::success);

			std::string fileName = "./out/frames/" + std::to_string(nCurrentFrame + 1) + ".ppm";

			const Image* imageBeingRenderedPtr = renderImages[this->indexImageBeingRendered];

			writeThreads[this->indexImageBeingRendered] = std::thread([nCurrentFrame, &_nFrames, &fileName, &imageBeingRenderedPtr]() {
				auto startTime = std::chrono::system_clock::now();

				imageBeingRenderedPtr->writeToDisk(fileName);

				auto endTime   = std::chrono::system_clock::now();
				auto elapsed   = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

				EN::LOG::println("[WRITING]   Wrote    " + std::to_string(nCurrentFrame + 1) + " / " + std::to_string(_nFrames) + " (" + std::to_string(elapsed.count()) + "ms)", LOG_TYPE::success);
			});

			this->resetDepthBuffer();

			nCurrentFrame++;
		}

		for (unsigned int i = 0; i < nEndFrame - nStartFrame; i++) {
			// Join Threads
			writeThreads[i].join();

			// Reset Images To Black
			std::fill_n(this->renderImages[i]->colorBuffer, this->width * this->height, 0);
		}
	}
}

void Engine::writeVideo(const unsigned int& _fps) {	
	File file("./out/video/videoEncoder.py", FILE_WRITE, false);

	if (file.isOpen()) {
		unsigned int nLines = sizeof(PYTHON_VIDEO_WRITER_SOURCE_CODE_LINES) / sizeof(const char *);

		for (unsigned int i = 0; i < nLines; i++) {
			file.writeNoVerif(PYTHON_VIDEO_WRITER_SOURCE_CODE_LINES[i] + std::string("\n"));
		}

		EN::LOG::println("[VIDEO] Added Python Script (1/3)", LOG_TYPE::success);
	} else {
		EN::LOG::println("[ERROR] While Writing Python File To Encode Video (1/3 failed)", LOG_TYPE::error);
	}

	// Close file here so that the system can execute it
	file.close();

	EN::LOG::println("[VIDEO] Writing Video (2/3)", LOG_TYPE::success);

	system(("cd ./out/video/ && python videoEncoder.py " + std::to_string(_fps)).c_str());

	EN::LOG::println("[VIDEO] Wrote Video (3/3)", LOG_TYPE::success);
}