#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader final {
private:
	enum class CheckErrorsType { VERTEX, FRAGMENT, PROGRAM };
	static std::string checkErrorsTypeToStr(const CheckErrorsType type);

	GLuint ID;

	bool tryReadFromFile(const std::string path, std::string &content) const;
	void checkCompileErrors(const GLuint shader, const CheckErrorsType type) const;
public:
	void read(const std::string &vertex_path, const std::string &fragment_path);
	void use();

	Shader();
	Shader(const std::string &vertex_path, const std::string &fragment_path);
	~Shader();

	void setBool(const std::string &name, const bool value);
	void setInt(const std::string &name, const int value);
	void setFloat(const std::string &name, const float value);

	void setVec(const std::string &name, const float x, const float y);
	void setVec(const std::string &name, const float x, const float y, const float z);
	void setVec(const std::string &name, const float x, const float y, const float z, const float w);

	void setVec(const std::string &name, const glm::vec2 &value);
	void setVec(const std::string &name, const glm::vec3 &value);
	void setVec(const std::string &name, const glm::vec4 &value);

	void setMat(const std::string &name, const glm::mat2 &value);
	void setMat(const std::string &name, const glm::mat3 &value);
	void setMat(const std::string &name, const glm::mat4 &value);
};

#endif // !SHADERS_HPP