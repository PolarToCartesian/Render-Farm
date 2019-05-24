#include "Engine/Engine.h"

class App : Engine {
	public:
		App() : Engine(1920, 1080) {
			this->addModel(Model("models/armadillo.txt", Vector3D(), false));
			this->addLight(Light(Vector3D(0, 0,-5), Vector3D(255, 255, 255), 5));
			this->camera.position.z = -3.f;
			this->renderAndWriteFrames(15 * 5);
			this->writeVideo(15);
		}

		void render() override {}
		void update() override {
			this->models[0].rotate(Vector3D(0.05f, 0.05f, 0.05f));
		}
};

int main() {
	App app;

	std::cin.get();

	return 0;
}