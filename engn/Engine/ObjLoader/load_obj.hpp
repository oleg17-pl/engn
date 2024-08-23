#ifndef LOAD_OBJ_HPP
#define LOAD_OBJ_HPP

#include <string>
#include <vector>
#include <exception>

#include <glm/glm.hpp>

#include "obj_exceptions.hpp"
#include "../Objects/Model/model.hpp"

class ObjLoader {
private:
	std::vector<RawMesh> tmp_rmeshes;
	Model model;
	unsigned int
		vertex_faces_offset,
		texture_faces_offset,
		normal_faces_offset;

	void createNewMesh(const std::string &name);
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
	Model getModel() const;
};

#endif // !LOAD_OBJ_HPP