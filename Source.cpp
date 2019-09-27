#include "Engine/Include.h"

class App : Renderer {
public:
	App() : Renderer(1920, 1080, Color<>(0)) {
		this->materials.insert({ "Metal", Material(1, 5, std::array<Color<>, 3>{ Color<>(150), Color<>(150), Color<>(150) }) });
		this->models.insert({ "F-16", Model("models/f-16.txt", "Metal", Vec3(), Vec3(), Vec3(0, PI + 1, 0)) });
		this->lights.insert({ "Front-Light", Light(Vec3(0, 2, -4), Color<>(255), 1) });

		this->camera.position = Vec3(0.f, 0.75f, -2.f);

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

	void update() override {
		this->models["F-16"].rotate(Vec3(0.f, 0.05f, 0.f));
	}
};

int main() {
	App app;

	std::cin.get();
	return 0;
}