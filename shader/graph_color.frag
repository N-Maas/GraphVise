#version 330 core
in vec3 FragPos;
in vec3 Normal;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
out vec4 FragColor;

void main() {
    // Simple lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.3);  // 0.3 = ambient

    vec3 result = objectColor * diff;
    FragColor = vec4(result, 1.0);
}