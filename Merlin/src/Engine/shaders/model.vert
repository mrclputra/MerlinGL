#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

//uniform vec3 model;
uniform mat4 view;
uniform mat4 projection;

// stuff to pass to fragment shader
out vec3 vColor;

void main() {
    vColor = aColor;
    gl_Position = projection * view * vec4(aPos, 1.0);
}