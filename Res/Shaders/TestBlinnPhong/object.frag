#version 330 core

layout(location = 0) out vec4 FragColor;

uniform vec3 u_Color;

uniform vec3 u_LightColor;
uniform vec3 u_LightPos;

uniform vec3 u_CameraPos;

uniform bool u_EnableAmbient;
uniform bool u_EnableDiffuse;
uniform bool u_EnableSpecular;

in vec3 vs_objNormal;
in vec3 vs_objFragPos;

void main()
{
    // 环境光照
    float Ka = 0.1;
    vec3 ambient = vec3(0.0);
    if (u_EnableAmbient)
        ambient = Ka * u_LightColor;

    // 漫反射
    float Kd = 2.5;
    float r = distance(u_LightPos, vs_objFragPos);
    vec3 norm = normalize(vs_objNormal);
    vec3 lightDir = normalize(u_LightPos - vs_objFragPos);
    vec3 diffuse = vec3(0.0);
    if (u_EnableDiffuse)
        diffuse = Kd * (u_LightColor / (r * r)) * max(0.0, dot(norm, lightDir));

    // 镜面反射
    float Ks = 0.6;
    vec3 viewDir = normalize(u_CameraPos - vs_objFragPos);
    vec3 half = normalize(viewDir + lightDir);
    vec3 specular = vec3(0.0);
    if (u_EnableSpecular)
        specular = Ks * (u_LightColor / (r * r)) * pow(max(0.0, dot(norm, half)), 32);

    vec3 result = (ambient + diffuse + specular) * u_Color;
    FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
}