#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "load_obj.hpp"

void ObjLoader::createNewModel(const std::string &name) {
	vertex_faces_offset += tmp_rmesh.vertices.size();
	normal_faces_offset += tmp_rmesh.normals.size();
	texture_faces_offset += tmp_rmesh.tex_coords.size();
	Model model;
	model.name = name;
	model.mesh = tmp_rmesh.ToMesh();
	models.push_back(model);
	tmp_rmesh = RawMesh{};
}

void ObjLoader::processVertexLine(std::stringstream &ls) {
	glm::vec4 vec{};
	ls >> vec.x >> vec.y >> vec.z;
	if (!(ls >> vec.w)) {
		vec.w = 1.0f;
	}
	tmp_rmesh.vertices.push_back(vec);
}

void ObjLoader::processNormalLine(std::stringstream &ls) {
	glm::vec3 vec{};
	ls >> vec.x >> vec.y >> vec.z;
	tmp_rmesh.normals.push_back(vec);
}

void ObjLoader::processTexCordLine(std::stringstream &ls) {
	glm::vec3 vec{};
	ls >> vec.x >> vec.y >> vec.z;
	tmp_rmesh.tex_coords.push_back(vec);
}

void ObjLoader::processFaceLine(std::stringstream &ls) {
	std::array<RawMesh::Face::Element, 3> indices{};
	RawMesh::Face::Element tmp_triangle{};
	std::string token;
	size_t el_index{};
	while (ls >> token) {
		std::stringstream token_stream{ token };
		std::string index;
		int part{ 1 };
		for (part; std::getline(token_stream, index, '/'); part++) {
			if (part >= 4) {
				throw InvalidFaceElement{ current_row, part };
			}
			if (index.empty()) {
				if (part == 2) {
					continue;
				}
				else {
					throw InvalidFaceElement{ current_row, part };
				}
			}
			int num{ std::stoi(index) };
			if (num <= 0) {
				throw InvalidFaceElement{ current_row, part };
			}
			if (part == 1) {
				tmp_triangle.v = num - vertex_faces_offset;
			}
			else if (part == 2) {
				tmp_triangle.t = num - texture_faces_offset;
			}
			else if (part == 3) {
				tmp_triangle.n = num - normal_faces_offset;
			}
		}
		if (part <= 3) {
			if (part == 2) {
				tmp_triangle.t = 0;
			}
			else if (part == 2 || part == 3) {
				tmp_triangle.n = 0;
			}
			else {
				throw InvalidFaceElement{ current_row, part };
			}
		}
		indices[el_index] = tmp_triangle;
		el_index++;
	}
	tmp_rmesh.faces.push_back(RawMesh::Face{ indices });
}

std::string ObjLoader::getFileName(const std::string &path) {
	size_t pos{ path.find_last_of("\\/") };
	std::string name;
	if (pos == std::string::npos) {
		name = path;
	}
	else {
		name = path.substr(pos + 1);
	}
	size_t dot_pos{ name.find_last_of('.') };
	if (dot_pos != std::string::npos) {
		name = name.substr(0, dot_pos);
	}
	return name;
}

std::string ObjLoader::getTag(const std::string &line) {
	std::stringstream ls{ line };
	std::string tag;
	ls >> tag;
	return tag;
}

bool ObjLoader::checkValidTag(const std::string &tag) {
	return (
		tag == "#" ||
		tag == "mtllib" ||
		tag == "usemtl" ||
		tag == "s" ||
		tag == "o" ||
		tag == "v" ||
		tag == "vn" ||
		tag == "vt" ||
		tag == "f" ||
		tag == "m" ||
		tag == "g"
		);
}

void ObjLoader::processLine(const std::string &line) {
	static const std::string DEFAULT_NAME{ "default" };
	static const short LINE_OFFSET{ 2 };
	const std::string value{ line.substr(LINE_OFFSET, line.length() - LINE_OFFSET) };
	std::stringstream vs{ value };
	std::string tag{ getTag(line) };
	if (!checkValidTag(tag)) {
		throw UnderfinedTagException{ current_row, tag };
	}
	if (tag == "#") {
		return;
	}
	else if (tag == "o") {
		if (current_row != 1) {
			createNewModel(prev_name);
		}
		prev_name = value;
	}
	else if (tag == "v") {
		processVertexLine(vs);
	}
	else if (tag == "vn") {
		processNormalLine(vs);
	}
	else if (tag == "vt") {
		processTexCordLine(vs);
	}
	else if (tag == "f") {
		processFaceLine(vs);
	}
}

void ObjLoader::read(const std::string &path) {
	std::ifstream file{ path };
	if (!file.is_open()) {
		throw ObjFileOpenException{ current_row, path };
	}
	std::string line;
	while (std::getline(file, line)) {
		if (!line.empty()) {
			processLine(line);
		}
		current_row++;
	}
	createNewModel(prev_name);
	scene = Scene{ getFileName(path), models };
	opened = true;
	file.close();
}

ObjLoader::ObjLoader()
	: tmp_rmesh{},
	models{},
	scene{},
	opened{ false },
	current_row{ 1 },
	vertex_faces_offset{},
	texture_faces_offset{},
	normal_faces_offset{},
	prev_name{ DEFAULT_NAME }
{}

ObjLoader::ObjLoader(const std::string &path) : ObjLoader() { read(path); }

Scene ObjLoader::getScene() const {
	if (!opened) {
		throw FileNotOpenedException{ current_row };
	}
	return scene;
}