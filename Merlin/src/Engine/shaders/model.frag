#version 330 core
out vec4 FragColor;

// lights
struct DirectionalLight {
    vec3 direction;
    vec3 color;
};
uniform DirectionalLight dirLights[8];
uniform int numDirLights;

// attributes
// todo: need to implement a proper modelloader, cuz need normals for lighting and stuff
in vec3 vColor;

void main() {
    FragColor = vec4(vColor.r, vColor.g, vColor.b, 1.0f);
//    FragColor = vec4(dirLights[0].color, 1.0f); // debug
}