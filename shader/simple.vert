#version 330 core

layout (location = 0) in vec3 in_pos;

uniform mat4 mvp;

void main()
{
   vec4 pos = mvp * vec4(in_pos, 1.f);
   gl_Position = vec4(pos.x, pos.y, pos.z, pos.w);
}
