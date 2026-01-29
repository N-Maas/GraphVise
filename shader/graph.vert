#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;  // Add normal attribute

uniform mat4 mvp;
uniform mat4 model;  // Add model matrix to transform normals

// Output to fragment shader
out vec3 FragPos;
out vec3 Normal;

void main()
{
   // Transform position
   vec4 worldPos = model * vec4(in_pos, 1.f);
   FragPos = worldPos.xyz;

   // Transform normal (using normal matrix for correct scaling)
   Normal = mat3(transpose(inverse(model))) * in_normal;

   // Final position
   gl_Position = mvp * vec4(in_pos, 1.f);
}
/*
optional: calculate like this:
void main() {
    // Debug: visualize normals
    // FragColor = vec4(Normal * 0.5 + 0.5, 1.0);
    // return;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.3);

    vec3 result = objectColor * diff;
    FragColor = vec4(result, 1.0);
}
*/