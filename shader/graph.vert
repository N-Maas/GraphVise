#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;  // normal used to calculate lighting after edge rotation
uniform mat4 mvp;
uniform mat4 model;
uniform mat3 normalMatrix;  // properly orienting normal
out vec3 FragPos;
out vec3 Normal;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    //Normal = aPos;  // For sphere, normals = normalized positions
    Normal = normalize(normalMatrix * aNormal);  // Transform normals correctly
}