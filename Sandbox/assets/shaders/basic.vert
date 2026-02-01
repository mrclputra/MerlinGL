#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aUV;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 v_UV;
out vec3 v_Normal;
out vec3 v_WorldPos;

void main() {
    v_UV = aUV;
    v_WorldPos = vec3(u_Model * vec4(aPosition, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    gl_Position = u_Projection * u_View * vec4(v_WorldPos, 1.0);
}
