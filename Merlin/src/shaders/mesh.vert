#version 330 core

// inputs
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// outputs, to frag shader
out vec3 vFragPos;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBitangent;
out vec2 vUV;

void main() {
    // transform attributes to world space; we can do this using the model matrix
    vFragPos = vec3(model * vec4(aPos, 1.0));
    vNormal = mat3(model) * aNormal;
    vTangent = mat3(model) * aTangent; // mat4 to mat3 conversion, translation does not matter
    vBitangent = mat3(model) * aBitangent;
    vUV = aUV;

    gl_Position = projection * view * vec4(vFragPos, 1.0);
}