#version 330 core

in vec3 FragCoord, Normal;

uniform vec3 lightPos;

const float ambientStrength = 0.2;
const vec3 lightColor = vec3(1.0, 1.0, 1.0) / 1.5;
const vec3 objColor = vec3(0.0, 1.0, 1.0);

vec3 getAmbient() {
	return lightColor * ambientStrength;
}

vec3 getDiffuse(vec3 normal, vec3 lightDir) {
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;
	return diffuse;
}

void main() {
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragCoord);
    vec3 ambient = getAmbient();
    float diffuse = getDiffuse(norm, lightDir);
    vec3 lighting = ambient + diffuse;
    vec3 res = lighting * objColor;
	gl_FragColor = vec4(res, 1.0);
}