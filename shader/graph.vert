#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 mvp;
uniform mat4 model;
out vec3 FragPos;
out vec3 Normal;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aPos;  // For sphere, normals = normalized positions
}
