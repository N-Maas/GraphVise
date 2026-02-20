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
uniform bool renderingSpheres;

// Outputs
out vec4 FragColor;
out uvec2 PickingData;  // For picking framebuffer (ID rendering)

void main() {
    // ===== LIGHTING CALCULATIONS =====

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.3);     // 0.3 = ambient

    // Combine lighting with object color
    vec3 result = VertexColor.rgb * diff;

    // ===== OUTPUT FOR NORMAL RENDERING =====
    // Use the combined lighting result with object's alpha for transparency
    FragColor = vec4(result, Transparency);
    //FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0); // for debugging

    // ===== OUTPUT FOR PICKING (to color attachment 1) =====
    // Pass instance ID for object selection
    // For spheres: InstanceId is vertex ID
    // For cylinders: InstanceId is edge ID
    if (renderingSpheres) {
    PickingData = uvec2(InstanceId, 0xFFFFFFFF);
    } else {
    PickingData = uvec2(0xFFFFFFFF, InstanceId);
    }
}