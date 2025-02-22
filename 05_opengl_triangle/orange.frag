#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;
out vec4 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightDir;

vec3 unreal(vec3 x) {
  return x / (x + 0.155) * 1.019;
}

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 viewPos = -vec3(view * vec4(0, 0, 0, 1));
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 halfDir = normalize(viewDir + lightDir);

    vec3 copper = pow(vec3(0xb6 / 255.0, 0x71 / 255.0, 0x30 / 255.0), vec3(2.2));
    vec3 specColor = mix(copper, vec3(1, 1, 1), 0.1) * 1.5;
    vec3 diffColor = copper;
    float shineness = 40;

    float specular = pow(max(0, dot(halfDir, normal)), shineness);
    float diffuse = max(0, dot(lightDir, normal));
    float ambient = 0.05;
    vec3 finalColor = (diffuse + ambient) * diffColor + specular * specColor;

    finalColor = unreal(finalColor);
    fragColor = vec4(finalColor, 1);
}
