#version 330 core
/*
lighting is currently not enabeled for lines because of trouble with normals for one dimentional object
and how to light a one dimensional object
*/

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColor;  // Location 2 matches your vertex attribute
layout(location = 3) in uint id;

uniform mat4 mvp;

out vec4 vColor;
flat out uint InstanceId;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
    vColor = aColor;

    InstanceId = id;
}

