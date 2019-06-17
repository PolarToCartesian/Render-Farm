#include "Engine/Include.h"

class App : Renderer {
	public:
		App() : Renderer(1920 / 2, 1080 / 2) {
			this->addModel(Model("models/town.txt", Vec3(), false, Color(255)));
			this->addLight(Light(Vec3(), Color(255), 10));
			//this->addLight(Light(Vec3(0, 0,-5), Color(255, 255, 255), 5));
			this->camera.position.y = 7.f;
			this->renderAndWriteFrames(15 * 5);
			this->writeVideo(15 * 5, 15);
		}

		void render(const bool _doRenderOver3D) override {
			this->drawPointNoVerif(10, 10, Color(255, 0, 0));

			if (_doRenderOver3D) {
				this->drawDisk(Vec3(this->getWidth() / 2, this->getHeight() / 2), 30, Color(255));
			}
		}

		void update() override {
			//this->models[0].rotate(Vec3(0.05f, 0.05f, 0.05f));

			this->camera.rotation.y += 0.05f;

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