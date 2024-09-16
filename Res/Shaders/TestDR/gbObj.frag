#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gSpecular;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{
	// 存储位置信息
	gPosition = FragPos;
	// 存储法线信息
	gNormal = normalize(Normal);
	// 存储diffuse材质颜色 - albedo
	gAlbedo = texture(texture_diffuse1, TexCoords).rgb;
	// 存储specular材质值 - specular
	gSpecular = texture(texture_specular1, TexCoords).rgb;
}