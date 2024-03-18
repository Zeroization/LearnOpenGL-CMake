#version 330 core

layout(location = 0) out vec4 FragColor;

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material u_Material;

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
    vec3 ambient = vec3(0.0);
    if (u_EnableAmbient)
        ambient = u_Material.ambient * u_LightColor;

    // 漫反射
    float r = distance(u_LightPos, vs_objFragPos);
    vec3 norm = normalize(vs_objNormal);
    vec3 lightDir = normalize(u_LightPos - vs_objFragPos);
    vec3 diffuse = vec3(0.0);
    if (u_EnableDiffuse)
        diffuse = u_Material.diffuse * (u_LightColor / (r * r)) * max(0.0, dot(norm, lightDir));

    // 镜面反射
    vec3 viewDir = normalize(u_CameraPos - vs_objFragPos);
    vec3 half = normalize(viewDir + lightDir);
    vec3 specular = vec3(0.0);
    if (u_EnableSpecular)
        specular = u_Material.specular * (u_LightColor / (r * r)) * pow(max(0.0, dot(norm, half)), u_Material.shininess * 128);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
}