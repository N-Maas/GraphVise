#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
uniform mat4 mvp;
uniform mat4 model;
out vec3 FragPos;
out vec3 Normal;

void main() {
   gl_Position = mvp * vec4(aPos, 1.0);
   FragPos = vec3(model * vec4(aPos, 1.0));
   Normal = normalize(aPos);  // For sphere, normals = normalized positions
}