#include "object.hpp"

Object::Object(const glm::vec3 &position, const glm::quat &orientation)
	: position{ position },
	orientation{ orientation } {}

void Object::move(const glm::vec3 &translation) {
	position += translation;
}

void Object::rotate(const float angle, const glm::vec3 &axis) {
	glm::quat rotation = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
	orientation = rotation * orientation;
}

glm::mat4 Object::getMat() const {
	glm::mat4 model{ glm::mat4{ 1.0f } };
	if (position != glm::vec3{ 0.0f }) {
		model = glm::translate(model, position);
	}
	if (orientation != glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f }) {
		glm::mat4 orientation_mat{ glm::toMat4(orientation) };
		model = orientation_mat * model;
	}
	return model;
}
