#ifndef LOAD_OBJ_HPP
#define LOAD_OBJ_HPP

#include <string>
#include <vector>
#include <exception>

#include <glm/glm.hpp>

#include "obj_exceptions.hpp"
#include "../Objects/Model/model.hpp"
#include "../Scene/scene.hpp"

const std::string DEFAULT_NAME{ "default" };

class ObjLoader {
private:
	RawMesh tmp_rmesh;
	std::vector<Model> models;
	Scene scene;

	unsigned int
		vertex_faces_offset,
		texture_faces_offset,
		normal_faces_offset;

	std::string prev_name;

	void createNewModel(const std::string &name);

	void processVertexLine(std::stringstream &ls);
	void processNormalLine(std::stringstream &ls);
	void processTexCordLine(std::stringstream &ls);
	void processFaceLine(std::stringstream &ls);
protected:
	bool opened;
	int current_row;

	static std::string getFileName(const std::string &path);
	static std::string getTag(const std::string &line);
	static bool checkValidTag(const std::string &tag);
	void processLine(const std::string &value);
public:
	void read(const std::string &path);
	ObjLoader();
	ObjLoader(const std::string &path);
	Scene getScene() const;
};

#endif // !LOAD_OBJ_HPP