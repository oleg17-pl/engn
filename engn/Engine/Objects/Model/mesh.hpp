#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <vector>
#include <array>

#include <glm/glm.hpp>
#include <glad/glad.h>

class Mesh;

class RawMesh final {
public:
	struct Face {
		struct Element {
			unsigned int v, t, n;
		};
		std::array<Element, 3> indices;
	};
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec3> normals, tex_coords;
	std::vector<Face> faces;

	Mesh ToMesh();
};

class Mesh final {
private:
	GLuint VAO, VBO;
public:
	struct Triangle {
		std::array<glm::vec4, 3> vectices;
		std::array<glm::vec3, 3> normals;
	};
	std::vector<Triangle> triangles;

	void setupBuffers();
	RawMesh toRMesh();
	~Mesh();

	friend class Model;
};

#endif // !MESH_HPP