#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

uniform mat4 uModelMtx;
uniform mat4 uCameraMtx;
uniform mat4 uProjectionMtx;

uniform mat4 uNormalMtx;

out vec3 vWorldPos;
out vec3 vNormal;

void main() {
    vec4 worldPos = uModelMtx * vec4(aPos, 1.0);

    vWorldPos = worldPos.xyz;

    vNormal = normalize(uNormalMtx * vec4(aNormal, 1.0)).xyz;

    gl_Position = uProjectionMtx * uCameraMtx * worldPos;
}
