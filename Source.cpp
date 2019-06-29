#include "Engine/Include.h"

class App : Renderer {
	public:
		App() : Renderer(1920 / 2, 1080 / 2) {
			this->addModel(Model("models/armadillo.txt", Vec3(), false, Color<>(255)));
			this->addLight(Light(Vec3(0, 0, -2), Color<>(255), 2));
			this->camera.position.y = 0.5;
			this->camera.position.z = -2;

			uint32_t fps = 60; // frames per second
			uint32_t duration = 30; // seconds
			uint32_t nFrames = fps * duration;

			this->renderAndWriteFrames(nFrames);
			this->writeVideo(nFrames, fps);
		}

		void render() override {
			this->drawModel(0);
		}

		void update() override {
			this->getModelRef(0).rotate(Vec3(0.f, 0.25f, 0.f));
		}
};

int main() {
	auto start = std::chrono::system_clock::now();
	App app;
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << elapsed.count() << "ms" << std::endl;
	
	std::cin.get();

	return 0;
}