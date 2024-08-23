#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec3 normal;

out vec3 FragCoord, Normal;

uniform mat4 model, view, projection;

void main() {
	FragCoord = vec3(model * pos);
	Normal = mat3(transpose(inverse(model))) * normal;
	gl_Position = projection * view * model * pos;
}