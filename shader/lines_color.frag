#version 330 core
/*
lighting is currently not enabeled for lines because of trouble with normals for one dimentional object
and how to light a one dimensional object
*/

in vec4 vColor;
flat in uint InstanceId;

out vec4 FragColor; // To screen (RGBA8)
out uvec2 PickingData; // For picking framebuffer (ID rendering)

void main() {
    FragColor = vColor;
    PickingData = uvec2(0xFFFFFFFF, InstanceId);
}