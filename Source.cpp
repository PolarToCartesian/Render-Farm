#include "Engine/Include.h"

class App : Renderer {
	public:
		App() : Renderer(1920, 1080, new PerspectiveCamera(90.f, 0.1f, 1000.f), Color<>(0)) {
			this->textures.insert({ "Teapot", Texture("textures/teapot.ppm") });
			this->textures.insert({ "Dirt",   Texture("textures/dirt.ppm") });

			this->materials.insert({ "Metal",  Material(1.f, 5.f, std::array<Color<>, 3>{ Color<>(200), Color<>(200), Color<>(200) }) });
			this->materials.insert({ "Teapot", Material(1.f, 5.f, "Teapot") });
			this->materials.insert({ "Dirt",   Material(1.f, 5.f, "Dirt") });
			this->models.insert({ "F-16", Model("models/f-16.txt", "Metal",  Vec3(), Vec3(), Vec3(0, PI + 1, 0)) });
			this->models.insert({ "Quad", Model("models/quad.txt", "Dirt", Vec3(), Vec3(), Vec3()) });
			this->lights.insert({ "Front-Light", Light(Vec3(0, 2, -4), Color<>(255), 1) });

			this->models["Quad"].triangles[0].vertices[2].textureCoord = Vec3();
			this->models["Quad"].triangles[0].vertices[1].textureCoord = Vec3(0, 1);
			this->models["Quad"].triangles[0].vertices[0].textureCoord = Vec3(1, 1);

			this->models["Quad"].triangles[1].vertices[0].textureCoord = Vec3(1, 0);
			this->models["Quad"].triangles[1].vertices[1].textureCoord = Vec3(1, 1);
			this->models["Quad"].triangles[1].vertices[2].textureCoord = Vec3(0, 0);

			this->camera->position.x = 0;
			this->camera->position.y = 0.75;
			this->camera->position.z = -2;


			this->run(30, 5);
		}

		void render2D() override {
			//this->fillRect(10, 10, 100, 100, Color<>(255));
			//this->fillDisk(500, 500, 100, Color<>(255));
			//this->textureRect(20, 50, 500, 500, "Teapot");
		}

		void render3D() override {
			//this->renderModels.push_back("F-16");
			this->renderModels.push_back("Quad");
			this->renderLights.push_back("Front-Light");
		}

		void update() override {
			//this->models["F-16"].rotate(Vec3(0.f, 0.05f, 0.f));
			this->models["Quad"].rotate(Vec3(0.f, 0.05f, 0.f));
		}
};

int main() {
	App app;
}