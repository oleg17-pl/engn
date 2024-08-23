#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scene.hpp"
#include "../engine.hpp"

void Scene::forEachMembers(std::function<void(Model &)> func) {
	for (auto &model : models) {
		func(model);
	}
}

Scene::Scene(const std::vector<Model> &models) : models{ models } {}

void Scene::add(const Model &model) {
	models.push_back(model);
}

void Scene::del(const std::string &name) {
	models.erase(
		std::remove_if(models.begin(), models.end(),
			[&name](const Model &model) {
				return model.name == name;
			}),
		models.end());
}

void Scene::moveAll(const glm::vec3 &translation) {
	forEachMembers(
		[&translation](Model &model) { model.move(translation); }
	);
}

void Scene::rotateAll(const float angle, const glm::vec3 &axis) {
	forEachMembers(
		[angle, &axis](Model &model) { model.rotate(angle, axis); }
	);
}

void Scene::setupAllBuffers() {
	forEachMembers(
		[](Model &model) { model.setupBuffers(); }
	);
}

void Scene::draw() {
	const float aspect{ Engine::getInstance()->getScreenAspect() };
	auto camera{ Engine::getInstance()->getCamera() };
	auto shader{ Engine::getInstance()->getShader() };
	auto dr{ [&shader, &camera, &aspect](Model &model) {
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
		glm::mat4 view = camera->getMat();
		glm::mat4 model_mat = model.getMat();
		shader->setMat("projection", projection);
		shader->setMat("view", view);
		shader->setMat("model", model_mat);
		shader->setVec("lightPos", glm::vec3{ 10.0f, 7.0f, 10.0f });
		model.draw();
	} };
	forEachMembers(dr);
	glBindVertexArray(0);
}

Model *Scene::getByName(const std::string &name) {
	Model *founded{ nullptr };
	forEachMembers(
		[&name, &founded](Model &model) {
			if (model.name == name) {
				founded = &model;
				return;
			}
		}
	);
	return founded;
}