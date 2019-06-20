#include "Engine/Include.h"

class App : Renderer {
	public:
		App() : Renderer(1920 / 2, 1080 / 2) {
			this->addModel(Model("models/teapot.txt", Vec3(), true, Color(255)));
			this->addLight(Light(Vec3(0, 0, -4), Color(255), 6));
			this->camera.position.y = 2.f;
			this->camera.position.z = -4;
			this->renderAndWriteFrames(15 * 5);
			this->writeVideo(15 * 5, 15);
		}

		void render(const bool _doRenderOver3D) override {}

		void update() override {
			this->models[0].rotate(Vec3(0.f, 0.1f, 0.f));

			//this->camera.rotation.y += 0.05f;
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