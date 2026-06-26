#version 330 core
out vec4 FragColor;

// lights
struct DirectionalLight {
    vec3 direction;
    vec3 color;
};
uniform DirectionalLight dirLights[8];
uniform int numDirLights;

struct Material {
    vec3 albedo;
    float roughness;
    float metallic;
    float ao;
};
uniform Material material;

// attributes
in vec3 vNormal;
in vec3 vFragPos;

void main() {
    //    FragColor = vec4(material.albedo, 1.0);

    vec3 nm = vNormal * 0.5 + 0.5;
    FragColor = vec4(nm, 1.0f);

//    vec3 diff = dot(-normalize(dirLights[0].direction), normalize(vNormal)) * dirLights[0].color * material.albedo;
    // some silly formulas
    // https://lisyarus.github.io/blog/posts/a-silly-diffuse-shading-model.html
//    vec3 diff = pow(0.5 + 0.5 * dot(-normalize(dirLights[0].direction), normalize(vNormal)), 2.0) * vNormal * dirLights[0].color;
//    vec3 diff = pow(0.5 + 0.5 * dot(-normalize(dirLights[0].direction), normalize(vNormal)), 2.0) * material.albedo * dirLights[0].color;
//    FragColor = vec4(diff, 1.0f);
}