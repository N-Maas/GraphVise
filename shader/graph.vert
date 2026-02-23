#version 330 core

// Per-vertex attributes
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

// Sphere instance attributes (used when rendering spheres)
layout(location = 2) in vec3 aSpherePos;
layout(location = 3) in vec4 aSphereColor;
layout(location = 4) in uint aSphereId;

// Cylinder instance attributes (used when rendering cylinders)
layout(location = 5) in mat4 aCylinderMatrix;
layout(location = 9) in vec4 aCylinderColor;
layout(location = 10) in uint aCylinderId;

uniform mat4 mvp;            // Combined model-view-projection matrix
uniform float sphereRadius;  // Only used for spheres
uniform float cylinderRadius;
uniform bool renderingSpheres;  // Uniform to control which attributes to use

// Outputs to fragment shader
out vec4 VertexColor;
flat out uint InstanceId;
out vec3 FragPos;      // World space position for lighting
out vec3 Normal;       // World space normal for lighting
flat out float Transparency;  // Alpha value for transparency

void main() {
    mat4 model = mat4(1.0);
    vec4 worldPos;

    if (renderingSpheres) {
        // Sphere transformation
        model[0].x = sphereRadius;
        model[1].y = sphereRadius;
        model[2].z = sphereRadius;
        model[3].xyz = aSpherePos;

        VertexColor = aSphereColor;
        InstanceId = aSphereId;

        worldPos = model * vec4(aPos, 1.0);

        // Sphere Normal for lighting equals the position
        Normal = aPos;

    } else {
        // Cylinder transformation
        model = aCylinderMatrix;

        model[0] = model[0] * cylinderRadius;   // scale x column
        model[2] = model[2] * cylinderRadius;   // scale z column
        // y axis is already scaled correctly in initRenderingMatrixForEdge

        VertexColor = aCylinderColor;
        InstanceId = aCylinderId;

        worldPos = model * vec4(aPos, 1.0);

        // Calculate normal in world space for lighting
        Normal = mat3(transpose(inverse(model))) * aNormal;
    }


    // Calculate world space position for lighting
    FragPos = vec3(worldPos);
    Transparency = VertexColor.a;

    // Calculate clip space position
    gl_Position = mvp * worldPos;
}