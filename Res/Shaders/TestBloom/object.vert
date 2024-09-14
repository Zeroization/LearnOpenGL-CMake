#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat3 u_Normal;

out vec3 vs_objNormal;
out vec3 vs_objFragPos;
out vec2 vs_objTexCoords;

void main()
{
	vs_objNormal = u_Normal * normal;
	vs_objFragPos = vec3(u_Model * vec4(position, 1.0));
	vs_objTexCoords = texCoords;

	gl_Position = u_MVP * vec4(position, 1.0);
};