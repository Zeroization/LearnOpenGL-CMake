#version 330 core
layout(location = 0) out vec4 FragColor;

struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material u_Material;

// vv---------- Lights -----------vv
#define MAX_LIGHTS 5
struct DriectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DriectionalLight u_DirLight[MAX_LIGHTS];

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float linear;
    float quadratic;
};
uniform PointLight u_PointLight[MAX_LIGHTS];

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float linear;
    float quadratic;
};
uniform SpotLight u_SpotLight[MAX_LIGHTS];
// ^^---------- Lights ------------^^

uniform vec3 u_CameraPos;

in vec3 vs_objNormal;
in vec3 vs_objFragPos;
in vec2 vs_objTexCoords;

vec3 dirLighting(DriectionalLight light, vec3 normal, vec3 viewDir);
vec3 pointLighting(PointLight light, vec3 normal, vec3 viewDir);
vec3 spotLighting(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 norm = normalize(vs_objNormal);
    vec3 viewDir = normalize(u_CameraPos - vs_objFragPos);
    vec3 result = vec3(0.0);

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        result += dirLighting(u_DirLight[i], norm, viewDir);
        result += pointLighting(u_PointLight[i], norm, viewDir);
        result += spotLighting(u_SpotLight[i], norm, viewDir);
    }

    FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
}

vec3 dirLighting(DriectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 halfVec = normalize(viewDir + lightDir);

    // 环境光照
    vec3 ambient = texture(u_Material.diffuse, vs_objTexCoords).rgb * light.ambient;
    // 漫反射
    vec3 diffuse = texture(u_Material.diffuse, vs_objTexCoords).rgb * light.diffuse * max(0.0, dot(normal, lightDir));
    // 镜面反射
    vec3 specular = texture(u_Material.specular, vs_objTexCoords).rgb * light.specular * pow(max(0.0, dot(normal, halfVec)), u_Material.shininess * 128);

    return ambient + diffuse + specular;
}

vec3 pointLighting(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - vs_objFragPos);
    vec3 halfVec = normalize(viewDir + lightDir);

    // 环境光照
    vec3 ambient = texture(u_Material.diffuse, vs_objTexCoords).rgb * light.ambient;
    // 漫反射
    vec3 diffuse = texture(u_Material.diffuse, vs_objTexCoords).rgb * light.diffuse * max(0.0, dot(normal, lightDir));
    // 镜面反射
    vec3 specular = texture(u_Material.specular, vs_objTexCoords).rgb * light.specular * pow(max(0.0, dot(normal, halfVec)), u_Material.shininess * 128);
    // 衰减计算
    float dist = length(light.position - vs_objFragPos);
    float attenuation = 1.0 / (1.0 + light.linear * dist + light.quadratic * (dist * dist));

    return (ambient + diffuse + specular) * attenuation;
}

vec3 spotLighting(SpotLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - vs_objFragPos);
    vec3 halfVec = normalize(viewDir + lightDir);

    // 环境光
    vec3 ambient = light.ambient * texture(u_Material.diffuse, vs_objTexCoords).rgb;

    // 漫反射
    vec3 diffuse = light.diffuse * texture(u_Material.diffuse, vs_objTexCoords).rgb * max(0.0, dot(normal, lightDir));

    // 镜面光
    vec3 specular = light.specular * texture(u_Material.specular, vs_objTexCoords).rgb * pow(max(0.0, dot(normal, halfVec)), u_Material.shininess);

    // 聚光相关计算
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // 距离衰减
    float dist = length(light.position - vs_objFragPos);
    float attenuation = 1.0 / (1.0 + light.linear * dist + light.quadratic * (dist * dist));

    return (ambient + (diffuse + specular) * intensity) * attenuation;
}