#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../Objects/Interfaces/foreachable.hpp"
#include "../Objects/Interfaces/drawable.hpp"
#include "../Objects/Model/model.hpp"
#include "../Shader/shader.hpp"
#include "../Objects/Camera/camera.hpp"

class Scene final : private IForeachable<Model>, private IDrawable {
private:
	std::vector<Model> models;

	void forEachMembers(std::function<void(Model &)> func) override;
	void draw() override;
public:
	Scene(const std::vector<Model> &models = std::vector<Model>{});
	void add(const Model &model);
	void del(const std::string &name);
	void moveAll(const glm::vec3 &translation);
	void rotateAll(const float angle, const glm::vec3 &axis);
	void setupAllBuffers();
	Model *getByName(const std::string &name);

	friend class Engine;
};

#endif // !SCENE_HPP