#version 330 core

// Per-vertex attributes
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

// Sphere instance attributes
layout(location = 2) in vec3 aSpherePos;
layout(location = 3) in vec4 aSphereColor;
layout(location = 4) in uint aSphereId;

// Cylinder instance attributes
layout(location = 5) in vec3 aCylinderStart;
layout(location = 6) in vec3 aCylinderEnd;
layout(location = 7) in vec4 aCylinderColor;
layout(location = 8) in uint aCylinderId;

uniform mat4 mvp;
uniform float sphereRadius;
uniform float cylinderRadius;
uniform bool renderingSpheres;

out vec4 VertexColor;
flat out uint InstanceId;
out vec3 Normal;       // World space normal for lighting

void main() {
    vec4 worldPos;

    if (renderingSpheres) {
        // SIMPLE SPHERE: just translate and scale
        worldPos = vec4(aPos * sphereRadius + aSpherePos, 1.0);
        VertexColor = aSphereColor;
        InstanceId = aSphereId;
        Normal = aPos;  // For sphere, normal = position direction
    } else {
        // SIMPLE CYLINDER: just position between start and end
        vec3 direction = aCylinderEnd - aCylinderStart;
        float len = length(direction);

        if (len < 0.001) {
            gl_Position = vec4(0,0,0,1);
            return;
        }

        // Simple approach: just position the cylinder
        // For a cylinder aligned with Y axis, we need to:
        // 1. Scale Y by length
        // 2. Rotate to align with direction
        // 3. Translate to midpoint

        vec3 yAxis = normalize(direction);
        vec3 xAxis = normalize(cross(vec3(0,1,0), yAxis));
        vec3 zAxis = cross(yAxis, xAxis);

        // Build transform matrix
        mat4 transform = mat4(1.0);
        transform[0].xyz = xAxis * cylinderRadius;
        transform[1].xyz = yAxis * len;
        transform[2].xyz = zAxis * cylinderRadius;
        transform[3].xyz = (aCylinderStart + aCylinderEnd) * 0.5;

        worldPos = transform * vec4(aPos, 1.0);
        VertexColor = aCylinderColor;
        InstanceId = aCylinderId;

        // For normals, use the transformed normal
        Normal = mat3(transform) * aNormal;
    }

    gl_Position = mvp * worldPos;
}