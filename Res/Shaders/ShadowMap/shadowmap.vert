#version 330 core
layout(location = 0) in vec3 Position;

uniform mat4 u_LightSpaceMVP;

void main()
{
	gl_Position = u_LightSpaceMVP * vec4(Position, 1.0);
}