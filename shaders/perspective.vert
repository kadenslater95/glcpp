#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uCamera;
uniform mat4 uProjection;

uniform mat4 uNormalMatrix;

out vec3 vWorldPosition;
out vec3 vNormal;

void main() {
    vec4 worldPosition = uModel * vec4(aPos, 1.0);

    vWorldPosition = worldPosition.xyz;

    vNormal = normalize(uNormalMatrix * vec4(aNormal, 1.0)).xyz;

    gl_Position = uProjection * uCamera * worldPosition;
}
