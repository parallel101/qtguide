#version 330 core

in vec3 position;
in vec3 normal;
out vec3 fragPosition;
out vec3 fragNormal;

struct Camera {
    mat4 model;
    mat4 view;
    mat4 projection;
};
uniform Camera camera;

void main() {
    gl_Position = camera.projection * camera.view * camera.model * vec4(position, 1);
    fragPosition = vec3(camera.view * camera.model * vec4(position, 1));
    fragNormal = normalize(transpose(inverse(mat3(camera.view * camera.model))) * normal);
}
