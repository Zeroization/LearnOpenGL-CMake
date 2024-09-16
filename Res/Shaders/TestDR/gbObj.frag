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
	// �洢λ����Ϣ
	gPosition = FragPos;
	// �洢������Ϣ
	gNormal = normalize(Normal);
	// �洢diffuse������ɫ - albedo
	gAlbedo = texture(texture_diffuse1, TexCoords).rgb;
	// �洢specular����ֵ - specular
	gSpecular = texture(texture_specular1, TexCoords).rgb;
}