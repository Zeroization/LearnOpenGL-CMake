#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat3 u_Normal;

out vs_objOUT
{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
} vs_objData;


void main()
{
	vs_objData.Normal = u_Normal * normal;
	vs_objData.FragPos = vec3(u_Model * vec4(position, 1.0));
	vs_objData.TexCoords = texCoords;

	gl_Position = u_MVP * vec4(position, 1.0);
};