#version 330 core

// Inputs from vertex shader
in vec4 VertexColor;
flat in uint InstanceId;
in vec3 FragPos;
in vec3 Normal;
flat in float Transparency;

// Uniforms for lighting
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;  // Camera position for specular lighting

// Outputs
out vec4 FragColor;
out uvec2 PickingData;  // For picking framebuffer (ID rendering)

void main() {
    // ===== LIGHTING CALCULATIONS =====

    // Ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting (optional, adds shine)
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine lighting with object color
    vec3 result = (ambient + diffuse + specular) * VertexColor.rgb;

    // ===== OUTPUT FOR NORMAL RENDERING =====
    // Use the combined lighting result with object's alpha for transparency
    FragColor = vec4(result, Transparency);

    // ===== OUTPUT FOR PICKING (to color attachment 1) =====
    // Pass instance ID for object selection
    // For spheres: InstanceId is vertex ID
    // For cylinders: InstanceId is edge ID
    PickingData = uvec2(InstanceId, 0xFFFFFFFF);
}