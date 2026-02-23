#version 330 core

// Inputs from vertex shader
in vec3 VertexColor;
flat in uint InstanceId;
//in vec3 FragPos;
in vec3 Normal;
//flat in float Transparency;

// Uniforms for lighting
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform bool renderingSpheres;

// Outputs
out vec4 FragColor;
out uvec2 PickingData;  // For picking framebuffer (ID rendering)

void main() {
    // Just output the vertex color with full brightness, no lighting
    FragColor = vec4(VertexColor.rgb, 1.0);
    PickingData = uvec2(InstanceId, 0xFFFFFFFF);
}