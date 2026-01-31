#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aUV;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_Normal;
out vec3 v_FragPos;

void main() {
    vec4 worldPos = u_Model * vec4(aPosition, 1.0);
    v_FragPos = worldPos.xyz;
    v_Normal = mat3(transpose(inverse(u_Model))) * aNormal;

    gl_Position = u_Projection * u_View * worldPos;
}
