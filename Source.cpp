#define USESPECULAR a

#include "Engine/Include.h"

class App : Renderer {
	public:
		int i = 0;

		App() : Renderer(1920, 1080) {
			//this->addModel(Model("models/town.txt", Vec3(), false, Color<>(255), Vec3(0), Vec3(), 1));
			//this->addModel(Box(Vec3(0), 1, true, Color<>(255, 0, 255), Vec3(0)).model);
			this->addModel(Model("models/f-16.txt", Vec3(), false, Color<>(82, 81, 89), Vec3(0), Vec3(0, 4, 0), 0.5));
			
			this->addLight(Light(Vec3(0.5, 1, -1.5), Color<>(255), 1));
			this->camera.position.x = -0.4;
			this->camera.position.y = 1;
			this->camera.position.z = -2;

			const uint32_t fps = 30; // frames per second
			const uint32_t duration = 5; // seconds
			const uint32_t nFrames = fps * duration;

			this->renderAndWriteFrames(nFrames);
			this->writeVideo(nFrames, fps);
		}

		void render() override {
			this->drawModel(0);
		}

		void update() override {
			//this->getModelRef(0).rotate(Vec3(0.f, 0.05f, 0.f));
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