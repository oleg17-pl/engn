#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object {
public:
	glm::vec3 position;
	glm::quat orientation;

	Object(
		const glm::vec3 &position = glm::vec3{ 0.0f },
		const glm::quat &orientation = glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f });

	void move(const glm::vec3 &translation);
	void rotate(const float angle, const glm::vec3 &axis);
	virtual glm::mat4 getMat() const;
};

#endif // !OBJECT_HPP