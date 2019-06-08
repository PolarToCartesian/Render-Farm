#include "Engine/Include.h"

class App : Renderer {
	public:
		App() : Renderer(1920 / 2, 1080 / 2) {
			this->addModel(Model("models/armadillo.txt", Vec3(), false));
			this->addLight(Light(Vec3(0, 0,-5), Color(255, 255, 255), 5));
			this->camera.position.z = -2.5f;
			this->renderAndWriteFrames(15 * 20);
			this->writeVideo(15 * 20, 15);
		}

		void render(const bool& _doRenderOver3D) override {}

		void update() override {
			this->models[0].rotate(Vec3(0.05f, 0.05f, 0.05f));

			this->getLightRef(0).intensity -= 0.005f;
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