#include "Engine/Include.h"

class App : Renderer {
public:
	int i = 1;

	App() : Renderer(1920 / 2, 1080 / 2, Color<>(0)) {
		this->materials.insert({ "Metal", Material(1, 5, std::array<Color<>, 3>{ Color<>(105), Color<>(105), Color<>(105) }) });
		this->models.insert({ "F-16", Model("models/f-16.txt", "Metal") });
		this->lights.insert({ "Front-Light", Light(Vec3(0, 0.5, -2), Color<>(255), 1) });

		this->camera.position.x = 0;
		this->camera.position.y = 1;
		this->camera.position.z = -3;

		const uint32_t fps = 30; // frames per second
		const uint32_t duration = 5; // seconds
		const uint32_t nFrames = fps * duration;

		this->renderAndWriteFrames(nFrames);
		this->writeVideo(nFrames, fps);
	}

	void render3D() override {
		this->renderModels.push_back("F-16");
		this->renderLights.push_back("Front-Light");
	}

	void render2D() override {

	}

	void update() override {
		this->models["F-16"].rotate(Vec3(0.f, 0.05f, 0.f));
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