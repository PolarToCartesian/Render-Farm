#include "Engine/Engine.h"

#include "Engine/objects/Triangle.h"

class App : Engine {
	public:
		App() : Engine(1920, 1080) {
			this->addModel(Model("models/quad.txt", Vector3D(), true));
			this->addLight(Light(Vector3D(0, 0,-5), Vector3D(255, 255, 255), 5));
			this->camera.position.z = -4.f;
			this->renderAndWriteFrames(15 * 5);
			this->writeVideo(15);
		}

		void render() override {}
		void update() override {
			this->models[0].rotate(Vector3D(0.05f, 0.05f, 0.05f));

			this->models[0].applyFunctionToEachTriangle([](Triangle& _tr) {
				for (char i = 0; i < 3; i++) {
					_tr.colors[i].r -= 3;
					_tr.colors[i].g -= 0;
					_tr.colors[i].b -= 2;
					_tr.colors[i].constrain(0, 255);
				}
			});
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