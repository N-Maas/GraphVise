#version 330 core
in vec3 FragPos;
in vec3 Normal;
uniform vec3 objectColor;
uniform float transparency;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform uint vertexId;         // Vertex ID for picking
uniform uint edgeId;           // edge ID for picking

layout(location = 0) out vec4 FragColor;   // To screen (RGBA8)
layout(location = 1) out uvec2 pickingOutput;  // To picking buffer (R32UI) (vertexId, edgeId)


void main() {
    // Simple lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.3);  // 0.3 = ambient

    vec3 result = objectColor * diff;
    FragColor = vec4(result, transparency);

    // Picking output - just the ID as unsigned integer
    pickingOutput = uvec2(vertexId, edgeId);
}