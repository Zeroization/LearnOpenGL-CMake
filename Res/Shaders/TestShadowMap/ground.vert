#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace[5];
} vs_out;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_Normal;
uniform mat4 lightSpaceMatrix[5];

void main()
{
	vs_out.FragPos = vec3(u_Model * vec4(aPos, 1.0));
	vs_out.Normal = transpose(inverse(mat3(u_Model))) * aNormal;
	vs_out.TexCoords = aTexCoords;
	for (int i = 0; i < 5; ++i)
	{
		vs_out.FragPosLightSpace[i] = lightSpaceMatrix[i] * vec4(vs_out.FragPos, 1.0);
	}
	
    gl_Position = u_MVP * vec4(aPos, 1.0);
}