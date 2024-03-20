#version 330 core
layout(location = 0) out vec4 FragColor;

struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material u_Material;

struct DriectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DriectionalLight u_DirLight;

uniform vec3 u_CameraPos;

in vec3 vs_objNormal;
in vec3 vs_objFragPos;
in vec2 vs_objTexCoords;

vec3 dirLighting()
{
    // 环境光照
    vec3 ambient = texture(u_Material.diffuse, vs_objTexCoords).rgb * u_DirLight.ambient;

    // 漫反射
    float r = distance(abs(u_DirLight.direction), vs_objFragPos);
    vec3 norm = normalize(vs_objNormal);
    vec3 lightDir = normalize(-u_DirLight.direction);
    vec3 diffuse = texture(u_Material.diffuse, vs_objTexCoords).rgb * (u_DirLight.diffuse / (r * r)) * max(0.0, dot(norm, lightDir));

    // 镜面反射
    vec3 viewDir = normalize(u_CameraPos - vs_objFragPos);
    vec3 half = normalize(viewDir + lightDir);
    vec3 specular = texture(u_Material.specular, vs_objTexCoords).rgb * (u_DirLight.specular / (r * r)) * pow(max(0.0, dot(norm, half)), u_Material.shininess * 128);

    vec3 result = ambient + diffuse + specular;

    return result;
}

void main()
{
    vec3 result = dirLighting();
    FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
}