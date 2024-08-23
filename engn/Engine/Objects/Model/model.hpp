#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>

#include <glad/glad.h>

#include "../object.hpp"
#include "../Interfaces/foreachable.hpp"
#include "../Interfaces/drawable.hpp"
#include "mesh.hpp"

class Model final : public Object, private IForeachable<Mesh>, private IDrawable {
private:
	void forEachMembers(std::function<void(Mesh &)> func) override;
	void draw() override;
public:
	std::string name;
	std::vector<Mesh> meshes;

	Model(
		const glm::vec3 &position = glm::vec3{ 0.0f },
		const glm::quat &orientation = glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f });

	void setupBuffers();
	void activate();

	friend class Scene;
};

#endif // !MODEL_HPP
