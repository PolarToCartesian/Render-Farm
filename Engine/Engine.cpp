#include "Engine.h"

// Engine Functions

void engineRenderImage(const unsigned int& _frame, const Image& _renderSurface, const std::string& _fileName) {
	auto startTime = std::chrono::system_clock::now();

	_renderSurface.writeToDisk(_fileName.c_str());

	auto endTime = std::chrono::system_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

	std::cout << "Wrote Frame n" << (_frame + 1) << " (" << elapsed.count() << "ms)" << std::endl;
}

// Engine Class

// Private Methods

void Engine::dealWithDepthBuffer() {
	delete[] this->depthBuffer;

	this->depthBuffer = new double[this->width * this->height];

	std::fill_n(this->depthBuffer, this->width * this->height, -1.f);
}

unsigned int Engine::getIndexInColorBuffer(const unsigned int& _x, const unsigned int& _y) {
	return renderImages[this->currentFrameBeingRendered].getIndex(_x, _y);
}

void Engine::calculateValues() {
	this->perspectiveMatrix = EN::MATRIX4X4::getPerspectiveMatrix(this->width, this->height, this->fov, this->zNear, this->zFar);
	this->dealWithDepthBuffer();
}

// in->triangle; "out"->doRenderTriangle, triangleSurfaceNormal, _rotatedVertices, _transformedVertices
void Engine::apply3D(const Triangle& _tr, bool& _doRenderTriangle, Vector3D& _triangleSurfaceNormal, Vector3D * _rotatedVertices, Vector3D * _transformedVertices) {
	_doRenderTriangle = true;
	
	// Apply 3D Rotation around _tr.rotationMidPoint
	std::memcpy(_rotatedVertices, _tr.vertices, SIZE_3_VECTOR3D);

	for (unsigned char v = 0; v < 3; v++) {
		_rotatedVertices[v] -= _tr.rotationMidPoint;

		Matrix4x4 rotationMatrix = EN::MATRIX4X4::getRotationXMatrix(_tr.rotation.x)
								 * EN::MATRIX4X4::getRotationYMatrix(_tr.rotation.y)
			                     * EN::MATRIX4X4::getRotationZMatrix(_tr.rotation.z);

		_rotatedVertices[v] *= rotationMatrix;

		_rotatedVertices[v] += _tr.rotationMidPoint;
	}

	_triangleSurfaceNormal = EN::TRIANGLE::getSurfaceNormal(_rotatedVertices);

	if (!this->camera.isTriangleFacingCamera(_rotatedVertices, _triangleSurfaceNormal)) _doRenderTriangle = false;

	// Apply Camera Translation, perspective and change coordinate system
	std::memcpy(_transformedVertices, _rotatedVertices, SIZE_3_VECTOR3D);

	for (unsigned char v = 0; v < 3; v++) {
		// Camera Translation
		_transformedVertices[v] -= this->camera.position;

		// Perspective
		_transformedVertices[v] = _transformedVertices[v] * this->perspectiveMatrix;

		if (_transformedVertices[v].w <= 0) _doRenderTriangle = false; 

		_transformedVertices[v] /= _transformedVertices[v].w;

		// Change coordinate system
		_transformedVertices[v].x = ((_transformedVertices[v].x - 1.f) / -2.f) * this->width;  // (-1 to 1)  to (0 to width)
		_transformedVertices[v].y = ((_transformedVertices[v].y + 1.f) / +2.f) * this->height; // (1  to -1) to (height to 0)
	}
}

// Public Methods

Engine::Engine(const unsigned int& _width, const unsigned int& _height, const unsigned int& _fov, const double& _zNear, const double& _zFar) : width(_width), height(_height), fov(_fov), zNear(_zNear), zFar(_zFar) {
	std::experimental::filesystem::create_directory("./out");
	std::experimental::filesystem::create_directory("./out/frames");
	std::experimental::filesystem::create_directory("./out/video");

	this->calculateValues();
}

Engine::~Engine() {
	delete[] this->depthBuffer;
}

void Engine::setWidth(unsigned int& _width)   { this->width  = _width;  this->calculateValues(); }
void Engine::setHeight(unsigned int& _height) { this->height = _height; this->calculateValues(); }

unsigned int Engine::getWidth()  const { return this->width; }
unsigned int Engine::getHeight() const { return this->height; }

unsigned int Engine::addLight(const Light& _light) { this->lights.push_back(_light); return static_cast<unsigned int>(this->lights.size() - 1); }
Light Engine::copyLight(const unsigned int& _lightId) { return this->lights[_lightId]; }
void Engine::setLight(const unsigned int& _lightId, const Light _light) { this->lights[_lightId] = _light; }

unsigned int Engine::addModel(const Model & _model) { this->models.push_back(_model); return static_cast<unsigned int>(this->models.size() - 1); }
Model Engine::copyModel(const unsigned int& _modelId) const { return this->models[_modelId]; }
void Engine::setModel(const unsigned int& _modelId, const Model _model) { this->models[_modelId] = _model; }

void Engine::drawPointNoVerif(const unsigned int& _x, const unsigned int& _y, const Color & _color) {
	renderImages[this->currentFrameBeingRendered].colorBuffer[getIndexInColorBuffer(_x, _y)] = _color;
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
			renderImages[this->currentFrameBeingRendered].colorBuffer[baseIndex + x] = _color;
		}
	}
}

void Engine::drawRectangle(const unsigned int& _x, const unsigned int& _y, const unsigned int& _w, const unsigned int& _h, const Color & _color) {
	for (unsigned int y = (_y >= 0) ? _y : 0; y < ((_y + _h < this->height - 1) ? (_y + _h) : (this->height - 1)); y++) {
		unsigned int baseIndex = y * this->width;

		for (unsigned int x = (_x >= 0) ? _x : 0; x < ((_x + _w < this->width) ? (_x + _w) : (this->width - 1)); x++) {
			renderImages[this->currentFrameBeingRendered].colorBuffer[baseIndex + x] = _color;
		}
	}
}

// Renders a triangle in 3D
void Engine::drawTriangle3D(const Triangle& _tr) {	
	bool     doRenderTriangle = true;
	Vector3D triangleSurfaceNormal;
	Vector3D rotatedVertices[3], transformedVertices[3];

	this->apply3D(_tr, doRenderTriangle, triangleSurfaceNormal, rotatedVertices, transformedVertices);

	if (!doRenderTriangle) return;

	Color vertexColorsWithLighting[3];
	EN::LIGHT::applyLightingToVertices(rotatedVertices, _tr.colors, triangleSurfaceNormal, this->lights, vertexColorsWithLighting);

	// PreCalculate Values (For Barycentric Interpolation)
	// Thanks to : https://codeplea.com/triangular-interpolation

	double denominator = (transformedVertices[1].y - transformedVertices[2].y)
				       * (transformedVertices[0].x - transformedVertices[2].x)
					   + (transformedVertices[2].x - transformedVertices[1].x)
					   * (transformedVertices[0].y - transformedVertices[2].y);

	double preCalc1 = (transformedVertices[1].y - transformedVertices[2].y);
	double preCalc2 = (transformedVertices[2].x - transformedVertices[1].x);
	double preCalc3 = (transformedVertices[2].y - transformedVertices[0].y);
	double preCalc4 = (transformedVertices[0].x - transformedVertices[2].x);

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
		unsigned int y = static_cast<unsigned int>(t0.y + i);

		if (y < 0 || y > this->height - 1) continue;

		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = static_cast<int>(second_half ? t2.y - t1.y : t1.y - t0.y);

		double alpha = static_cast<double>(i / total_height);
		double beta  = static_cast<double>((i - (second_half ? t1.y - t0.y : 0)) / segment_height);

		Vector3D A = t0 + (t2 - t0) * alpha;
		Vector3D B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;

		if (A.x > B.x) std::swap(A, B);

		// Limit X Between (0 and width - 1) in the loop to avoid trying to set a pixel out of the screen buffer and to maximize performance
		for (int j = static_cast<int>((A.x < 0) ? 0 : A.x); j <= static_cast<int>((B.x > this->width - 2) ? this->width - 2 : B.x); j++) {
			unsigned int x = j;

			// Continue Barycentric Interpolation
			// To calculate the w value (depth of the pixel to dicide if we should render it or not)

			double preCalc5 = (x - transformedVertices[2].x);
			double preCalc6 = (y - transformedVertices[2].y);

			double VertexPositionWeights[3] = {
				(preCalc1 * preCalc5 + preCalc2 * preCalc6) / denominator,
				(preCalc3 * preCalc5 + preCalc4 * preCalc6) / denominator,
				0,
			};

			VertexPositionWeights[2] = 1 - VertexPositionWeights[0] - VertexPositionWeights[1];

			double VertexPositionWeightSum = VertexPositionWeights[0] + VertexPositionWeights[1] + VertexPositionWeights[2];

			// Pixel Depth (w)
			double w = 0;

			// For every vertex
			for (unsigned char c = 0; c < 3; c++) {
				w += transformedVertices[c].w * VertexPositionWeights[c];
			}

			// Calculate W (depth of pixel)
			w /= VertexPositionWeightSum;

			// Render Pixel if it is in front of the pixel already there

			unsigned int pixelIndex = this->getIndexInColorBuffer(x, y);

			if (w < this->depthBuffer[pixelIndex] || this->depthBuffer[pixelIndex] == -1) {
				this->depthBuffer[pixelIndex] = w;

				// Triangle :D the pixel color

				Color pixelColor(0);

				// For every vertex
				for (unsigned char c = 0; c < 3; c++) {
					pixelColor += vertexColorsWithLighting[c] * VertexPositionWeights[c];
				}

				pixelColor /= VertexPositionWeightSum;
				pixelColor.constrain(0, 255);
				pixelColor.intify();

				renderImages[this->currentFrameBeingRendered].colorBuffer[pixelIndex] = pixelColor;
			}
		}
	}
}

// Renders And Writes to a folder with the name of the title given in the constructor the selected number of frames.
void Engine::writeFrames(const unsigned int& _frames) {
	std::vector<std::thread> renderThreads;

	// Alocate Space in std::vectors to limite unnecessary memory manipulation
	renderImages.reserve(_frames);
	renderThreads.reserve(_frames);

	// Render Each Frame
	for (unsigned int frame = 0; frame < _frames; frame++) {
		this->currentFrameBeingRendered = frame;
		renderImages.emplace_back(this->width, this->height);

		auto startTime = std::chrono::system_clock::now();

		// Call User Defined Functions
		this->render();
		this->update();

		// Render Each Model
		for (unsigned int i = 0; i < this->models.size(); i++) {
			if (this->models[i].doRender) {
				for (unsigned int j = 0; j < this->models[i].triangles.size(); j++) {
					this->drawTriangle3D(this->models[i].triangles[j]);
				}
			}
		}

		auto endTime = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

		std::cout << "Rendered Frame n" << (frame + 1) << " (" << elapsed.count() << "ms)" << std::endl;

		// Clear depthBuffer
		this->dealWithDepthBuffer();
		
		renderThreads.emplace_back(engineRenderImage, frame, this->renderImages[frame], "./out/frames/" + std::to_string(frame + 1) + ".ppm");
	}

	// Wait For Every Single Frame To Have Been Written
	for (unsigned int i = 0; i < _frames; i++) {
		renderThreads[i].join();
	}

	std::cout << "Finished Writing " << _frames << "!" << std::endl;
}

void Engine::writeVideo(const unsigned int& _fps) {	
	FILE* file;

	if (EN::UTIL::openFile(file, "./out/video/videoEncoder.py", "w")) {
		unsigned int nLines = sizeof(PYTHON_VIDEO_WRITER_SOURCE_CODE_LINES) / sizeof(const char *);

		for (unsigned int i = 0; i < nLines; i++) {
			fprintf(file, (PYTHON_VIDEO_WRITER_SOURCE_CODE_LINES[i] + std::string("\n")).c_str());
		}

		std::cout << "[VIDEO] Added Python Script (1/3)" << std::endl;
	} else {
		std::cout << "[ERROR] While Writing Python File To Encode Video (1/3 failed)" << std::endl;
	}

	fclose(file);

	std::cout << "[VIDEO] About To Write Video (2/3)" << std::endl;

	system(("cd ./out/video/ && python videoEncoder.py " + std::to_string(_fps)).c_str());

	std::cout << "[VIDEO] Wrote Video (3/3)" << std::endl;
}