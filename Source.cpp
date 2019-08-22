#define USESPECULAR a

#include "Engine/Include.h"

class App : Renderer {
	public:
		int i = 0;

		App() : Renderer(1920 / 2, 1080 / 2) {
			//this->addModel(Model("models/town.txt", Vec3(), false, Color<>(255), Vec3(0), Vec3(), 1));
			//this->addModel(Box(Vec3(0), 1, true, Color<>(255, 0, 255), Vec3(0)).model);
			
			for (float x = -2; x <= 2; x += 0.5) {
				for (float y = -2; y <= 2; y += 0.5) {
					i++;
					this->addModel(Box(Vec3(x, y, 0), 0.5, false, Color<>(255, 0, 0), Vec3(), Vec3(), 1).model);
				}
			}
			
			this->addLight(Light(Vec3(0, 1, -3), Color<>(255), 1));
			this->camera.position.x = 0;
			this->camera.position.y = 0;
			this->camera.position.z = -3;

			const uint32_t fps = 30; // frames per second
			const uint32_t duration = 5; // seconds
			const uint32_t nFrames = fps * duration;

			this->renderAndWriteFrames(nFrames);
			this->writeVideo(nFrames, fps);
		}

		void render() override {
			for (int c = 0; c < i; c++) {
				this->drawModel(c);
			}
		}

		void update() override {
			//this->getModelRef(0).rotate(Vec3(0.f, 0.05f, 0.f));

			this->camera.position.x += 0.05f;
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