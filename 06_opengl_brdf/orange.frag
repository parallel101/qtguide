#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;
out vec4 fragColor;

struct Camera {
    mat4 model;
    mat4 view;
    mat4 projection;
};
uniform Camera camera;

struct Light {
    vec3 direction;
};
uniform Light light;

struct PBRParams {
    vec3 baseColor;
    float roughness;
    float metallic;
    float specular;
    float emission;
};

uniform PBRParams pbrParams;

vec3 unreal(vec3 x) {
  return x / (x + 0.155) * 1.019;
}

vec3 fresnelFactor(float metallic, vec3 albedo, float specular) {
    return metallic * albedo + (1 - metallic) * 0.16 * specular;
}

vec3 cookTorrance(vec3 nrm, vec3 idir, vec3 odir, float roughness, vec3 f0) {
    const float EPS = 1e-10;
    const float PI = 3.14159265359;

    vec3 half = normalize(idir + odir);
    float NoH = max(EPS, dot(half, nrm));
    float NoL = max(EPS, dot(idir, nrm));
    float NoV = max(EPS, dot(odir, nrm));
    float VoH = min(1.0 - EPS, max(EPS, dot(half, odir)));
    float LoH = min(1.0 - EPS, max(EPS, dot(half, idir)));

    float alpha2 = max(EPS, roughness * roughness);
    float denom = 1.0 - (NoH * NoH) * (1.0 - alpha2);
    float ndf = alpha2 / (denom * denom);

    float k = alpha2 * 0.5;
    float vdf = 1.0 / ((NoV * k + 1.0 - k));
    vdf *= 1.0 / ((NoL * k + 1.0 - k));
    vdf /= mix(1.0, 4.0 * PI, alpha2);

    vec3 fdf = f0 + (1.0 - f0) * pow(1.0 - VoH, 5);

    return fdf * vdf * ndf;
}

vec3 calcPBR(vec3 normal, vec3 viewDir, vec3 lightDir, vec3 color, float metallic, float roughness, float specular) {
    vec3 f0 = fresnelFactor(metallic, mix(color, vec3(1, 1, 1), metallic), specular);
    float lambert = max(0.0, dot(normal, lightDir));
    vec3 cook = cookTorrance(normal, lightDir, viewDir, roughness, f0);
    return mix(lambert * color, cook * color, f0);
}

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 viewPos = -vec3(camera.view * vec4(0, 0, 0, 1));
    vec3 viewDir = normalize(viewPos - fragPosition);

    vec3 finalColor = calcPBR(normal, viewDir, normalize(light.direction), pbrParams.baseColor, pbrParams.metallic, pbrParams.roughness, pbrParams.specular);
    finalColor = pbrParams.emission * pbrParams.baseColor + (1.0 - pbrParams.emission) * finalColor;

    finalColor = unreal(finalColor);
    fragColor = vec4(finalColor, 1);
}
