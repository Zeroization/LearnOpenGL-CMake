#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

out vec3 vs_Normal;
out vec3 vs_Position;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat3 u_Normal;

void main()
{
	vs_Normal = u_Normal * Normal;
	vs_Position = vec3(u_Model * vec4(Position, 1.0));
	gl_Position = u_MVP * vec4(Position, 1.0);
}