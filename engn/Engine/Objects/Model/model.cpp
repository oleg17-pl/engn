#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "model.hpp"

void Model::setupBuffers() {
	mesh.setupBuffers();
}

void Model::activate() {
	glBindVertexArray(mesh.VAO);
}

void Model::draw() {
	activate();
	glDrawArrays(GL_TRIANGLES, 0, mesh.triangles.size() * 3);
}

Model::Model(const glm::vec3 &position, const glm::quat &orientation)
	: Object(position, orientation) {}
