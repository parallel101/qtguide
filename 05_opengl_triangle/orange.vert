#version 330 core

in vec3 position;
in vec3 normal;
out vec3 fragPosition;
out vec3 fragNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(position, 1);
    fragPosition = vec3(view * model * vec4(position, 1));
    fragNormal = normalize(transpose(inverse(mat3(view * model))) * normal);
}
