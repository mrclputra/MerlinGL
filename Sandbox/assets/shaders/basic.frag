#version 460 core

in vec3 v_Normal;
in vec3 v_FragPos;

out vec4 FragColor;

void main() {
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 normal = normalize(v_Normal);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 ambient = vec3(0.2);
    vec3 diffuse = vec3(0.8) * diff;

    vec3 color = vec3(0.8, 0.3, 0.2);
    FragColor = vec4(color * (ambient + diffuse), 1.0);
}
