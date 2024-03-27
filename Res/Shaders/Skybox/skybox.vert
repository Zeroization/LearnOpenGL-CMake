#version 330 core
layout (location = 0) in vec3 Position;

out vec3 vs_TexCoords;

uniform mat4 u_VP;

void main()
{
    vs_TexCoords = Position;
    vec4 pos = u_VP * vec4(Position, 1.0);
    gl_Position = pos.xyww;
}