#version 330 core

precision mediump float;

in vec3 vWorldPos;
in vec3 vNormal;

uniform vec3 uCameraPos; // in world space
uniform vec3 uLightPos; // in world space

uniform vec3 uLightColor;
uniform vec3 uObjectColor;

out vec4 FragColor;

void main() {
    // Direction from fragment to light
    vec3 L = normalize(uLightPos - vWorldPos);

    // Diffuse
    float diff = max(dot(vNormal, L), 0.0);

    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 V = normalize(uCameraPos - vWorldPos);
    vec3 H = normalize(L + V); // half vector
    float spec = pow(max(dot(vNormal, H), 0.0), 32.0);

    vec3 diffuse = diff * uLightColor;
    vec3 specular = specularStrength * spec * uLightColor;

    vec3 result = (ambient + diffuse + specular) * uObjectColor;

    FragColor = vec4(result, 1.0);
}
