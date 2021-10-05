#version 330 core

layout (location = 0) in vec3 in_pos;

void main()
{
   gl_Position = vec4(in_pos.x, in_pos.y, in_pos.z, 1.0);
}
