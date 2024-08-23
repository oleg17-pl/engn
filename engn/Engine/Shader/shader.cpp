#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

bool Shader::tryReadFromFile(const std::string path, std::string &content) const {
	std::ifstream file{ path };
	if (!file.is_open()) {
		return false;
	}
	std::stringstream content_stream;
	content_stream << file.rdbuf();
	file.close();
	content = content_stream.str();
	return true;
}

std::string Shader::checkErrorsTypeToStr(const CheckErrorsType type) {
	switch (type) {
	case CheckErrorsType::PROGRAM:
		return std::string{ "PROGRAM" };
	case CheckErrorsType::VERTEX:
		return std::string{ "VERTEX" };
	case CheckErrorsType::FRAGMENT:
		return std::string{ "FRAGMENT" };
	default:
		return std::string{ "ERRORTYPE" };
	}
}

void Shader::checkCompileErrors(const GLuint shader, const CheckErrorsType type) const {
	int success;
	char info_log[1024];
	std::string type_str{ checkErrorsTypeToStr(type) };

	if (type != CheckErrorsType::PROGRAM) {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, info_log);
			throw std::exception{ ("Shader compilation error of type: " + type_str + "\n" + info_log + "\n").c_str() };
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, info_log);
			throw std::exception{ ("Program linking error of type: " + type_str + "\n" + info_log + "\n").c_str() };
		}
	}
}

void Shader::read(const std::string &vertex_path, const std::string &fragment_path) {
	std::string vertex_code, fragment_code;
	if (!tryReadFromFile(vertex_path, vertex_code)) {
		throw std::exception{ "Failed to read vertex shader file!" };
	}
	if (!tryReadFromFile(fragment_path, fragment_code)) {
		throw std::exception{ "Failed to read fragment shader file!" };
	}
	const char *v_shader_code{ vertex_code.c_str() };
	const char *f_shader_code{ fragment_code.c_str() };

	GLuint vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &v_shader_code, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, CheckErrorsType::VERTEX);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &f_shader_code, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, CheckErrorsType::FRAGMENT);

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, CheckErrorsType::PROGRAM);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::Shader() : ID{} {}
Shader::Shader(const std::string &vertex_path, const std::string &fragment_path) : Shader() {
	read(vertex_path, fragment_path);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, const bool value) {
	GLint location{ glGetUniformLocation(ID, name.c_str()) };
	glUniform1i(location, (int)value);
}
void Shader::setInt(const std::string &name, const int value) {
	GLint location{ glGetUniformLocation(ID, name.c_str()) };
	glUniform1i(location, value);
}
void Shader::setFloat(const std::string &name, const float value) {
	GLint location{ glGetUniformLocation(ID, name.c_str()) };
	glUniform1f(location, value);
}

void Shader::setVec(const std::string &name, const float x, const float y) {
	GLint location{ glGetUniformLocation(ID, name.c_str()) };
	glUniform2f(location, x, y);
}
void Shader::setVec(const std::string &name, const float x, const float y, const float z) {
	GLint location{ glGetUniformLocation(ID, name.c_str()) };
	glUniform3f(location, x, y, z);
}
void Shader::setVec(const std::string &name, const float x, const float y, const float z, const float w) {
	GLint location{ glGetUniformLocation(ID, name.c_str()) };
	glUniform4f(location, x, y, z, w);
}

void Shader::setVec(const std::string &name, const glm::vec2 &value) {
	GLint location{ glGetUniformLocation(ID, name.c_str()) };
	glUniform2f(location, value.x, value.y);
}
void Shader::setVec(const std::string &name, const glm::vec3 &value) {
	GLint location{ glGetUniformLocation(ID, name.c_str()) };
	glUniform3f(location, value.x, value.y, value.z);
}
void Shader::setVec(const std::string &name, const glm::vec4 &value) {
	GLint location{ glGetUniformLocation(ID, name.c_str()) };
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::setMat(const std::string &name, const glm::mat2 &value) {
	GLint location{ glGetUniformLocation(ID, name.c_str()) };
	glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setMat(const std::string &name, const glm::mat3 &value) {
	GLint location{ glGetUniformLocation(ID, name.c_str()) };
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setMat(const std::string &name, const glm::mat4 &value) {
	GLint location{ glGetUniformLocation(ID, name.c_str()) };
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}