#include "Engine/rendering/Renderer.h"
#include "Engine/files/Video.h"

class App : Renderer {
	public:
		App() : Renderer(1920, 1080) {
			this->addModel(Model("models/quad.txt", Vec3(), true));
			this->addLight(Light(Vec3(0, 0,-5), Vec3(255, 255, 255), 5));
			this->camera.position.z = -1.f;
			this->renderAndWriteFrames(15 * 5);
			this->writeVideo(15);
		}

		void render() override {}
		void update() override {
			//this->models[0].rotate(Vec3(0.05f, 0.05f, 0.05f));

			this->getLightRef(0).intensity -= 0.05f;
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