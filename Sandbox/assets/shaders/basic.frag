#version 460 core

in vec2 v_UV;

out vec4 FragColor;

uniform sampler2D u_Albedo;

void main() {
    FragColor = texture(u_Albedo, v_UV);
}
