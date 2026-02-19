#version 330 core

// Per-vertex attributes
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

// Sphere instance attributes (used when rendering spheres)
layout(location = 2) in vec3 aSpherePos;
layout(location = 3) in vec4 aSphereColor;
layout(location = 4) in uint aSphereId;

// Cylinder instance attributes (used when rendering cylinders)
layout(location = 5) in vec3 aCylinderStart;
layout(location = 6) in vec3 aCylinderEnd;
layout(location = 7) in float aCylinderRadius;
layout(location = 8) in vec4 aCylinderColor;
layout(location = 9) in uint aCylinderId;

uniform mat4 mvp;            // Combined model-view-projection matrix
uniform float sphereRadius;  // Only used for spheres
uniform bool renderingSpheres;  // Uniform to control which attributes to use

// Outputs to fragment shader
out vec4 VertexColor;
flat out uint InstanceId;
out vec3 FragPos;      // World space position for lighting
out vec3 Normal;       // World space normal for lighting
flat out float Transparency;  // Alpha value for transparency

void main() {
    mat4 model = mat4(1.0);

    if (renderingSpheres) {
        // Sphere transformation
        model[0].x = sphereRadius;
        model[1].y = sphereRadius;
        model[2].z = sphereRadius;
        model[3].xyz = aSpherePos;

        VertexColor = aSphereColor;
        InstanceId = aSphereId;

        // Sphere Normal for lighting equals the position
        Normal = aPos;

    } else {
        // Cylinder transformation
        vec3 direction = aCylinderEnd - aCylinderStart;
        float length = length(direction);

        if (length < 0.001) {
            gl_Position = vec4(0,0,0,1);
            return;
        }

        vec3 yAxis = normalize(direction);
        vec3 xAxis = normalize(cross(vec3(0, 1, 0), yAxis));
        vec3 zAxis = cross(yAxis, xAxis);

        model[0] = vec4(xAxis * aCylinderRadius, 0);
        model[1] = vec4(yAxis * length, 0);
        model[2] = vec4(zAxis * aCylinderRadius, 0);
        model[3] = vec4((aCylinderStart + aCylinderEnd) * 0.5, 1);

        VertexColor = aCylinderColor;
        InstanceId = aCylinderId;

        // Calculate normal in world space for lighting
        Normal = mat3(transpose(inverse(model))) * aNormal;
    }

    // Calculate world space position for lighting
    FragPos = vec3(model * vec4(aPos, 1.0));
    Transparency = VertexColor.a;

    // Calculate clip space position
    gl_Position = mvp * vec4(aPos, 1.0);
}