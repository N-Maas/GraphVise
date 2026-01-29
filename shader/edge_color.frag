#version 330 core
in vec3 FragPos;
in vec3 Normal;
uniform vec4 objectColor;       // What user sees (RGB)

layout(location = 0) out vec4 FragColor;   // To screen (RGBA8)

uniform vec3 lightPos;
uniform vec4 lightColor;

void main() {
    // Simple lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.3);  // 0.3 = ambient

    vec4 result = objectColor * diff * lightColor;
    FragColor = result;                     // visual output, what user sees
}