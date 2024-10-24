#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat3 u_Normal;

out vs_objOUT
{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
} vs_objData;


const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
	vec4 totalPosition = vec4(0.0f);
	vec3 totalNormal = vec3(0.0f);
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (boneIds[i] == -1)
			continue;
		if (boneIds[i] >= MAX_BONES)
		{
			totalPosition = vec4(position, 1.0f);
			totalNormal = normal;
			break;
		}
		vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(position, 1.0f);
		totalPosition += localPosition * weights[i];
		vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normal;
		totalNormal += localNormal * weights[i];
	}

	vs_objData.Normal = u_Normal * normalize(totalNormal);
	vs_objData.FragPos = vec3(u_Model * totalPosition);
	vs_objData.TexCoords = texCoords;

	gl_Position = u_MVP * totalPosition;
};