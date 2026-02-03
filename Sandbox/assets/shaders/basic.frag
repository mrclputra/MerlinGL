#version 460 core

in vec2 v_UV;
in vec3 v_Normal;
in vec3 v_WorldPos;

out vec4 FragColor;

uniform sampler2D u_Albedo;

// this is a directional light, hardcoded
// to be replaced with actual uniforms later on
const vec3 lightDir = normalize(vec3(1.0, -1.0, 0.5));
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const float ambient = 0.15;

void main() {
    vec3 albedo = texture(u_Albedo, v_UV).rgb;
    vec3 normal = normalize(v_Normal);

    // diffuse
    float diff = max(dot(normal, -lightDir), 0.0);

    vec3 result = 0.5 * (ambient + diff * lightColor);
    FragColor = vec4(result, 1.0);
}
