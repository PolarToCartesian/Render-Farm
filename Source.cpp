#include "Engine/Include.h"

class App : Renderer {
	public:
		App() : Renderer(1920, 1080, new PerspectiveCamera(90.f, 0.1f, 1000.f), Color<>(0)) {
			this->materials.insert({ "Metal", Material(1, 5, std::array<Color<>, 3>{ Color<>(150), Color<>(150), Color<>(150) }) });
			this->models.insert({ "F-16", Model("models/f-16.txt", "Metal", Vec3(), Vec3(), Vec3(0, PI + 1, 0)) });
			this->lights.insert({ "Front-Light", Light(Vec3(0, 2, -4), Color<>(255), 1) });

			this->camera->position.x = 0;
			this->camera->position.y = 0.75;
			this->camera->position.z = -2;

			this->run(30, 5);
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
}