#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "model.hpp"

void Model::forEachMembers(std::function<void(Mesh &)> func) {
	for (auto &mesh : meshes) {
		func(mesh);
	}
}

void Model::setupBuffers() {
	auto setup{ [](Mesh &mesh) {
		mesh.setupBuffers();
	} };
	forEachMembers(setup);
}

void Model::activate() {
	forEachMembers([](Mesh &mesh) { glBindVertexArray(mesh.VAO); });
}

void Model::draw() {
	activate();
	forEachMembers([](Mesh &mesh) {
		glDrawArrays(GL_TRIANGLES, 0, mesh.triangles.size() * 3);
	});
}

Model::Model(const glm::vec3 &position, const glm::quat &orientation)
	: Object(position, orientation) {}
