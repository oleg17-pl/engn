#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "load_obj.hpp"

void ObjLoader::createNewMesh(const std::string &name) {
	if (!tmp_rmeshes.empty()) {
		vertex_faces_offset += tmp_rmeshes.back().vertices.size();
		normal_faces_offset += tmp_rmeshes.back().normals.size();
		texture_faces_offset += tmp_rmeshes.back().tex_coords.size();
	}
	RawMesh mesh;
	mesh.name = name;
	tmp_rmeshes.push_back(mesh);
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
	if (tag == "#" ||
		tag == "mtllib" ||
		tag == "usemtl" ||
		tag == "s" ||
		tag == "o" ||
		tag == "v" ||
		tag == "vn" ||
		tag == "vt" ||
		tag == "f" ||
		tag == "m" ||
		tag == "g") {
		return true;
	}
	else {
		return false;
	}
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
	if (tmp_rmeshes.empty() && tag != "o") {
		createNewMesh(DEFAULT_NAME);
	}
	if (tag == "o") {
		createNewMesh(value);
	}
	else if (tag == "v") {
		glm::vec4 vec{};
		vs >> vec.x >> vec.y >> vec.z;
		if (!(vs >> vec.w)) {
			vec.w = 1.0f;
		}
		tmp_rmeshes.back().vertices.push_back(vec);
	}
	else if (tag == "vn") {
		glm::vec3 vec{};
		vs >> vec.x >> vec.y >> vec.z;
		tmp_rmeshes.back().normals.push_back(vec);
	}
	else if (tag == "vt") {
		glm::vec3 vec{};
		vs >> vec.x >> vec.y >> vec.z;
		tmp_rmeshes.back().tex_coords.push_back(vec);
	}
	else if (tag == "f") {
		std::array<RawMesh::Face::Element, 3> indices{};
		RawMesh::Face::Element tmp_triangle{};
		std::string token;
		size_t el_index{};
		while (vs >> token) {
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
		tmp_rmeshes.back().faces.push_back(RawMesh::Face{ indices });
	}
}

void ObjLoader::read(const std::string &path) {
	std::ifstream file{ path };
	if (!file.is_open()) {
		throw ObjFileOpenException{ current_row, path };
	}
	std::string line;
	for (current_row; std::getline(file, line); current_row++) {
		if (!line.empty()) {
			processLine(line);
		}
	}
	std::vector<Mesh> meshes;
	meshes.reserve(tmp_rmeshes.size());
	for (auto &rmesh : tmp_rmeshes) {
		meshes.push_back(rmesh.ToMesh());
	}
	model.name = getFileName(path);
	model.meshes = meshes;
	tmp_rmeshes.clear();

	opened = true;
	file.close();
}

ObjLoader::ObjLoader()
	: tmp_rmeshes{},
	opened{ false },
	current_row{ 1 },
	vertex_faces_offset{},
	texture_faces_offset{},
	normal_faces_offset{}
{}

ObjLoader::ObjLoader(const std::string &path) : ObjLoader() { read(path); }

Model ObjLoader::getModel() const {
	if (!opened) {
		throw FileNotOpenedException{ current_row };
	}
	return model;
}