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
in vec3 vTangent;
in vec3 vBitangent;
in vec2 vUV;

uniform sampler2D uAlbedoMap;
uniform bool hasAlbedoMap;
uniform sampler2D uNormalMap;
uniform bool hasNormalMap;

void main() {
    vec3 N;
    if (hasNormalMap) {
        // TBN matrix is the transformation from the cotangentspace to worldspace
        // this is needed to apply object transformations to the normal map
        mat3 TBN = mat3(normalize(vTangent), normalize(vBitangent), normalize(vNormal));
        N = normalize(TBN * (texture(uNormalMap, vUV).rgb * 2.0 - 1.0));
    } else {
        N = normalize(vNormal);
    }
    vec3 albedo = hasAlbedoMap ? texture(uAlbedoMap, vUV).rgb : material.albedo;
//    FragColor = vec4(albedo, 1.0f);

    // silly formulas for diffuse
    // https://lisyarus.github.io/blog/posts/a-silly-diffuse-shading-model.html
    vec3 diff = pow(0.5 + 0.5 * dot(-normalize(dirLights[0].direction), N), 2.0) * albedo * dirLights[0].color;
    FragColor = vec4(diff, 1.0f);
}