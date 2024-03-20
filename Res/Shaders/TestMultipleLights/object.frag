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

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float linear;
    float quadratic;
};
uniform PointLight u_PointLight;

uniform vec3 u_CameraPos;

in vec3 vs_objNormal;
in vec3 vs_objFragPos;
in vec2 vs_objTexCoords;

vec3 dirLighting(DriectionalLight light, vec3 normal, vec3 viewDir);
vec3 pointLighting(PointLight light, vec3 normal, vec3 viewDir);


void main()
{
    vec3 norm = normalize(vs_objNormal);
    vec3 viewDir = normalize(u_CameraPos - vs_objFragPos);
    vec3 result = vec3(0.0);

    result += dirLighting(u_DirLight, norm, viewDir);
    result += pointLighting(u_PointLight, norm, viewDir);

    FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
}

vec3 dirLighting(DriectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 half = normalize(viewDir + lightDir);

    // 环境光照
    vec3 ambient = texture(u_Material.diffuse, vs_objTexCoords).rgb * light.ambient;
    // 漫反射
    vec3 diffuse = texture(u_Material.diffuse, vs_objTexCoords).rgb * light.diffuse * max(0.0, dot(normal, lightDir));
    // 镜面反射
    vec3 specular = texture(u_Material.specular, vs_objTexCoords).rgb * light.specular * pow(max(0.0, dot(normal, half)), u_Material.shininess * 128);

    return ambient + diffuse + specular;
}

vec3 pointLighting(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - vs_objFragPos);
    vec3 half = normalize(viewDir + lightDir);

    // 环境光照
    vec3 ambient = texture(u_Material.diffuse, vs_objTexCoords).rgb * light.ambient;
    // 漫反射
    vec3 diffuse = texture(u_Material.diffuse, vs_objTexCoords).rgb * light.diffuse * max(0.0, dot(normal, lightDir));
    // 镜面反射
    vec3 specular = texture(u_Material.specular, vs_objTexCoords).rgb * light.specular * pow(max(0.0, dot(normal, half)), u_Material.shininess * 128);
    // 衰减计算
    float distance = length(light.position - vs_objFragPos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));

    return (ambient + diffuse + specular) * attenuation;
}