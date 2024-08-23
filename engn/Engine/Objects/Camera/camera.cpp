#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.hpp"
#include "../../engine.hpp"

void Camera::updateVectors() {
	glm::mat4 orientation_mat = glm::toMat4(orientation);
	front = glm::normalize(glm::vec3{ orientation_mat * glm::vec4{ 0.0f, 0.0f, 1.0f, 0.0f } });
	right = glm::normalize(glm::cross(front, WORLD_UP));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::yawPitchToOrientation() {
	glm::quat yaw_q = glm::angleAxis(glm::radians(yaw), WORLD_UP);
	glm::quat pitch_q = glm::angleAxis(glm::radians(pitch), glm::vec3{ 1.0f, 0.0f, 0.0f });
	orientation = yaw_q * pitch_q;
	orientation = glm::normalize(orientation);
}

void Camera::orientationToYawPitch() {
	yaw = glm::degrees(glm::yaw(orientation));
	pitch = glm::degrees(glm::pitch(orientation));
}

glm::vec3 Camera::moveDirectionToVector(const MoveDirection &dir) {
	switch (dir) {
	case MoveDirection::FORWARD:
		return front;
	case MoveDirection::LEFT:
		return -right;
	case MoveDirection::BACK:
		return -front;
	case MoveDirection::RIGHT:
		return right;
	case MoveDirection::UP:
		return WORLD_UP;
	case MoveDirection::DOWN:
		return -WORLD_UP;
	default:
		return glm::vec3{ 0.0f };
	}
}

Camera::Camera(
	const glm::vec3 &position,
	const float yaw,
	const float pitch,
	const glm::vec3 &world_up)

	: Object(position),
	yaw{ yaw },
	pitch{ pitch },
	WORLD_UP(world_up),
	sensitivity(DEFAULT_CAMERA_SENSITIVITY),
	move_speed(DEFAULT_CAMERA_MOVE_SPEED)
{
	processMouseMovement(glm::vec2{ 0.0f });
}

void Camera::processMouseMovement(const glm::vec2 &offset) {
	glm::vec2 sens_offset{ offset * sensitivity };
	yaw += sens_offset.x;
	pitch += sens_offset.y;
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	yawPitchToOrientation();
	updateVectors();
}

void Camera::processKeyboardInput(const MoveDirection &dir) {
	const float dt{ Engine::getInstance()->getDeltaTime() };
	float velocity = move_speed * dt;
	move(-moveDirectionToVector(dir) * velocity);
}

void Camera::rotate(const float angle, const glm::vec3 &axis) {
	Object::rotate(angle, axis);
	orientationToYawPitch();
	updateVectors();
}

glm::mat4 Camera::getMat() const {
	glm::vec3 target{ position + front };
	return glm::lookAt(position, target, up);
}