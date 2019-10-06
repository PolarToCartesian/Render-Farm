#include "Engine/Include.h"

class App : Renderer {
	public:
		App() : Renderer(1920, 1080, new PerspectiveCamera(1920, 1080, 90.f, 0.1f, 1000.f), Color<>(0)) {
			this->textures.insert({ "Teapot", Texture("res/textures/teapot.ppm") });
			this->textures.insert({ "Dirt",   Texture("res/textures/dirt.ppm")   });
			this->textures.insert({ "Grass",  Texture("res/textures/grass.ppm")  });
			this->textures.insert({ "Ussr",   Texture("res/textures/ussr.ppm") });
			this->textures.insert({ "PlaneMetal", Texture("res/textures/planeMetal.ppm") });

			this->materials.insert({ "Metal",  Material(1.f, 5.f, std::array<Color<>, 3>{ Color<>(200), Color<>(200), Color<>(200) }) });
			this->materials.insert({ "Teapot", Material(1.f, 5.f, "Teapot") });
			this->materials.insert({ "Dirt",   Material(1.f, 5.f, "Dirt")   });
			this->materials.insert({ "Grass",  Material(1.f, 5.f, "Grass")  });
			this->materials.insert({ "Ussr",  Material(1.f, 5.f, "Ussr") });
			this->materials.insert({ "PlaneMetal",  Material(1.f, 5.f, "PlaneMetal") });


			this->models.insert({ "F-16", Model("res/models/f-16.txt", "Metal", Vec3(), Vec3(), Vec3(0, PI + 1, 0)) });
			this->models.insert({ "Quad", Model("res/models/quad.txt", "Dirt",  Vec3(), Vec3(), Vec3()) });
			this->models.insert({ "Box", Box(Vec3(), 0.5f, new std::string[6] {"Ussr", "Ussr", "Ussr", "Ussr", "Ussr", "Ussr"}, Vec3(), Vec3()) });

			this->models["Quad"].triangles[0].setTextureCoordinates(new Vec3[3] { Vec3(1, 1), Vec3(0, 1), Vec3(0, 0) });
			this->models["Quad"].triangles[1].setTextureCoordinates(new Vec3[3] { Vec3(1, 0), Vec3(1, 1), Vec3(0, 0) });

			this->lights.insert({ "Front-Light", Light(Vec3(0, 2, -4), Color<>(255), 1) });

			this->camera->setPosition(Vec3(0.f, 0.5f, -1.f));

			this->models["Box"].setRotation(Vec3(-0.45f, 0.f, 0.0f));

			this->run(30, 5);
		}

		void render2D() override {
			//this->fillRect(10, 10, 100, 100, Color<>(255));
			//this->fillDisk(500, 500, 100, Color<>(255));
			//this->textureRect(20, 50, 500, 500, "Teapot");
		}

		void render3D() override {
			//this->renderModels.push_back("F-16");
			//this->renderModels.push_back("Quad");
			this->renderModels.push_back("Box");
			this->renderLights.push_back("Front-Light");
		}

		void update() override {
			this->models["Box"].rotate(Vec3(0.f, 0.05f, 0.0f));
			//this->models["F-16"].rotate(Vec3(0.f, 0.05f, 0.f));
			//this->models["Quad"].rotate(Vec3(0.f, 0.05f, 0.f));
		}
};

int main() {
	App app;
}