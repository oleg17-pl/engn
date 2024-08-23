#include <iostream>

#include "Engine/engine.hpp"
#include "Engine/ObjLoader/load_obj.hpp"

const int SCR_WIDTH{ 800 }, SCR_HEIGHT{ 600 };

int main() {
	WindowConfig wconf{ SCR_WIDTH, SCR_HEIGHT, "chinazes" };
	auto engine{ Engine::getInstance() };
	if (!engine->init(wconf)) {
		return 1;
	}

	auto camera{ std::make_shared<Camera>() };
	auto scene{ std::make_shared<Scene>() };
	auto shader{ std::make_shared<Shader>() };
	auto loader{ std::make_unique<ObjLoader>() };

	camera->position = glm::vec3{ 0.0f, 0.0f, 3.0f };
	bool resourses_loaded{ false };
	try {
		loader->read("Objs\\abizana.obj");
		shader->read(
			"Shaders\\vertex.glsl",
			"Shaders\\fragment.glsl");
		resourses_loaded = true;
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	if (resourses_loaded) {
		std::cout << "Resourses loaded successfully" << std::endl;
	}
	
	auto model{ loader->getModel() };
	scene->add(model);
	loader.reset();
	scene->setupAllBuffers();
	engine->setCamera(camera);
	engine->setScene(scene);
	engine->setShader(shader);

	engine->start();
	return 0;
}