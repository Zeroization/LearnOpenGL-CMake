#version 330 core
layout(location = 0) out vec4 FragColor;

in vec3 vs_Normal;
in vec3 vs_Position;

uniform vec3 u_CameraPos;
uniform samplerCube u_Skybox;
uniform bool u_EnableReflection;
uniform bool u_EnableRefraction;
uniform bool u_EnableDynamicEnvMap;
uniform float u_RefractionRatio;

vec4 processReflection();
vec4 processRefraction();

void main()
{
	if (u_EnableReflection || u_EnableDynamicEnvMap)
	{
		FragColor = processReflection();
	}
	else if (u_EnableRefraction)
	{
		FragColor = processRefraction();
	}
	else 
	{
		FragColor = vec4(1.0, 0.0, 1.0, 1.0);
	}
}

vec4 processReflection()
{
	vec3 viewDir = normalize(vs_Position - u_CameraPos);
	vec3 reflectDir = reflect(viewDir, normalize(vs_Normal));
	return vec4(texture(u_Skybox, reflectDir).rgb, 1.0);
}

vec4 processRefraction()
{
	vec3 viewDir = normalize(vs_Position - u_CameraPos);
	vec3 refractDir = refract(viewDir, normalize(vs_Normal), u_RefractionRatio);
	return vec4(texture(u_Skybox, refractDir).rgb, 1.0);
}
