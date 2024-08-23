#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

#include "../object.hpp"

enum class MoveDirection {
	FORWARD,
	LEFT,
	BACK,
	RIGHT,
	UP,
	DOWN,
};

const glm::vec3 DEFAULT_CAMERA_POS{ 0.0f, 0.0f, 0.0f };
const glm::vec3 DEFAULT_CAMERA_WORLD_UP{ 0.0f, 1.0f, 0.0f };
const float DEFAULT_CAMERA_YAW{ 180.0f };
const float DEFAULT_CAMERA_PITCH{ 0.0f };
const float DEFAULT_CAMERA_SENSITIVITY{ 0.1f };
const float DEFAULT_CAMERA_MOVE_SPEED{ 5.0f };

class Camera final : public Object {
private:
	const glm::vec3 WORLD_UP;
	glm::vec3 front, right, up;
	float yaw, pitch;

	void updateVectors();
	void yawPitchToOrientation();
	void orientationToYawPitch();
	glm::vec3 moveDirectionToVector(const MoveDirection &dir);

	void processMouseMovement(const glm::vec2 &offset);
	void processKeyboardInput(const MoveDirection &dir);
public:
	float sensitivity, move_speed;

	Camera(
		const glm::vec3 &position = DEFAULT_CAMERA_POS,
		const float yaw = DEFAULT_CAMERA_YAW,
		const float pitch = DEFAULT_CAMERA_PITCH,
		const glm::vec3 &world_up = DEFAULT_CAMERA_WORLD_UP);

	void rotate(const float angle, const glm::vec3 &axis);
	glm::mat4 getMat() const override;

	friend class Engine;
};

#endif // !CAMERA_HPP