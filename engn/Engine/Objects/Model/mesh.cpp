#include "mesh.hpp"
#include <iostream>

Mesh RawMesh::ToMesh() {
	std::vector<glm::vec4> sorted_vertices;
	std::vector<glm::vec3>sorted_normals;
	for (auto &face : faces) {
		for (auto &element : face.indices) {
			unsigned int vertex_index = element.v - 1;
			unsigned int normal_index = element.n - 1;
			sorted_vertices.push_back(vertices[vertex_index]);
			sorted_normals.push_back(normals[normal_index]);
		}
	}
	std::vector<Mesh::Triangle> triangles;
	for (size_t i = 0; i < sorted_vertices.size(); i += 3) {
		Mesh::Triangle triangle;
		for (size_t ti = 0; ti < 3; ti++) {
			triangle.vectices[ti] = sorted_vertices[i + ti];
			triangle.normals[ti] = sorted_normals[i + ti];
		}
		triangles.push_back(triangle);
	}
	Mesh mesh;
	mesh.name = name;
	mesh.triangles = triangles;
	return mesh;
}

RawMesh Mesh::toRMesh() {
	RawMesh rmesh;
	rmesh.name = name;
	size_t f_index{ 1 };
	for (auto &tr : triangles) {
		RawMesh::Face face;
		for (size_t i = 0; i < 3; i++) {
			rmesh.vertices.push_back(tr.vectices[i]);
			rmesh.normals.push_back(tr.normals[i]);
			face.indices[i].v = f_index;
			face.indices[i].n = f_index;
			f_index++;
		}
		rmesh.faces.push_back(face);
	}
	return rmesh;
}


void Mesh::setupBuffers() {
	RawMesh rmesh{ toRMesh() };
	std::vector<float> v_array, n_array;
	v_array.reserve(rmesh.vertices.size() * 4);
	n_array.reserve(rmesh.normals.size() * 3);
	for (size_t i = 0; i < rmesh.vertices.size(); i++) {
		v_array.push_back(rmesh.vertices[i].x);
		v_array.push_back(rmesh.vertices[i].y);
		v_array.push_back(rmesh.vertices[i].z);
		v_array.push_back(rmesh.vertices[i].w);

		n_array.push_back(rmesh.normals[i].x);
		n_array.push_back(rmesh.normals[i].y);
		n_array.push_back(rmesh.normals[i].z);
	}
	const size_t vertices_size = v_array.size() * sizeof(float);
	const size_t normals_size = n_array.size() * sizeof(float);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices_size + normals_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_size, v_array.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertices_size, normals_size, n_array.data());

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)vertices_size);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}