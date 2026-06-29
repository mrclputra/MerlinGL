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
in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUV;

uniform sampler2D uAlbedoMap;
uniform bool hasAlbedoMap;
uniform sampler2D uNormalMap;
uniform bool hasNormalMap;

void main() {
    //    FragColor = vec4(material.albedo, 1.0);

    vec3 albedo = hasAlbedoMap ? texture(uAlbedoMap, vUV).rgb : material.albedo;
    vec3 normal = hasNormalMap ? normalize(texture(uNormalMap, vUV).rgb * 2.0 - 1.0) : vec3(1.0f, 0.0f, 1.0f);
//    FragColor = vec4(normal, 1.0f);

    // preview vertex normals
//    vec3 nm = vNormal * 0.5 + 0.5;
//    FragColor = vec4(nm, 1.0f);

//    vec3 diff = dot(-normalize(dirLights[0].direction), normalize(vNormal)) * dirLights[0].color * material.albedo;

    // silly formulas for diffuse
    // https://lisyarus.github.io/blog/posts/a-silly-diffuse-shading-model.html
    vec3 diff = pow(0.5 + 0.5 * dot(-normalize(dirLights[0].direction), normalize(vNormal)), 2.0) * albedo * dirLights[0].color;
    FragColor = vec4(diff, 1.0f);
}