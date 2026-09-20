#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

// TODO: Bring uniforms for Model, Camera, Projection and Normal Matrices as well as transforming vPos and vNormal
// Include a comment for the Normal matrix that it is the inverse transpose of the upper 3x3 matrix

uniform mat4 uModel;
uniform mat4 uCamera;
uniform mat4 uProjection;

uniform mat4 uNormalMatrix;

out vec3 vPos;
out vec3 vNormal;

void main() {
    // Just pass the position and normal directly through to the fragment shader
    vPos = aPos;
    vNormal = aNormal;

    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

/*
attribute vec2 aPosition;

attribute vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uCamera;
uniform mat4 uProjection;

uniform float rho;

// Inverse-Transpose of upper-left 3x3 matrix of uModel
uniform mat3 uNormalMatrix;

varying vec3 vNormal;
varying vec3 vWorldPosition;

void main() {
    float x = rho * sin(aPosition.y) * cos(aPosition.x);
    float y = rho * cos(aPosition.y);
    float z = rho * sin(aPosition.y) * sin(aPosition.x);

    vec4 worldPosition = uModel * vec4(x, y, z, 1.0);

    vWorldPosition = worldPosition.xyz;

    vNormal = normalize(uNormalMatrix * aNormal);

    gl_Position = uProjection * uCamera * worldPosition;
}
*/