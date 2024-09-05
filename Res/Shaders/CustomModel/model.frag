#version 330 core
layout(location = 0) out vec4 FragColor;

uniform vec3 u_CameraPos;

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
// vv---------- Model -------------vv
struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material u_Material;

uniform bool u_HasTextures;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

in vs_objOUT
{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
} vs_objData;
// ^^---------- Model -------------^^

vec3 dirLighting(DriectionalLight light, vec3 normal, vec3 viewDir);
vec3 pointLighting(PointLight light, vec3 normal, vec3 viewDir);
vec3 spotLighting(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 norm = normalize(vs_objData.Normal);
    vec3 viewDir = normalize(u_CameraPos - vs_objData.FragPos);
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

    vec3 ambient, diffuse, specular;
    if (u_HasTextures)
    {
        ambient = texture(texture_diffuse1, vs_objData.TexCoords).rgb * light.ambient;
        diffuse = texture(texture_diffuse1, vs_objData.TexCoords).rgb * light.diffuse * max(0.0, dot(normal, lightDir));
        specular = texture(texture_specular1, vs_objData.TexCoords).rgb * light.specular * pow(max(0.0, dot(normal, halfVec)), 32.0);
    } 
    else 
    {
        ambient = u_Material.ambient * light.ambient;
        diffuse = u_Material.diffuse * light.diffuse * max(0.0, dot(normal, lightDir));
        specular = u_Material.specular * light.specular * pow(max(0.0, dot(normal, halfVec)), u_Material.shininess * 128);
    }

    return ambient + diffuse + specular;
}

vec3 pointLighting(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - vs_objData.FragPos);
    vec3 halfVec = normalize(viewDir + lightDir);

    vec3 ambient, diffuse, specular;
    if (u_HasTextures)
    {
        ambient = texture(texture_diffuse1, vs_objData.TexCoords).rgb * light.ambient;
        diffuse = texture(texture_diffuse1, vs_objData.TexCoords).rgb * light.diffuse * max(0.0, dot(normal, lightDir));
        specular = texture(texture_specular1, vs_objData.TexCoords).rgb * light.specular * pow(max(0.0, dot(normal, halfVec)), 32.0);
    } 
    else 
    {
        ambient = u_Material.ambient * light.ambient;
        diffuse = u_Material.diffuse * light.diffuse * max(0.0, dot(normal, lightDir));
        specular = u_Material.specular * light.specular * pow(max(0.0, dot(normal, halfVec)), u_Material.shininess * 128);
    }
    
    // 衰减计算
    float dist = length(light.position - vs_objData.FragPos);
    float attenuation = 1.0 / (1.0 + light.linear * dist + light.quadratic * (dist * dist));

    return (ambient + diffuse + specular) * attenuation;
}

vec3 spotLighting(SpotLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - vs_objData.FragPos);
    vec3 halfVec = normalize(viewDir + lightDir);

    vec3 ambient, diffuse, specular;
    if (u_HasTextures)
    {
        ambient = texture(texture_diffuse1, vs_objData.TexCoords).rgb * light.ambient;
        diffuse = texture(texture_diffuse1, vs_objData.TexCoords).rgb * light.diffuse * max(0.0, dot(normal, lightDir));
        specular = texture(texture_specular1, vs_objData.TexCoords).rgb * light.specular * pow(max(0.0, dot(normal, halfVec)), 32.0);
    } 
    else 
    {
        ambient = u_Material.ambient * light.ambient;
        diffuse = u_Material.diffuse * light.diffuse * max(0.0, dot(normal, lightDir));
        specular = u_Material.specular * light.specular * pow(max(0.0, dot(normal, halfVec)), u_Material.shininess * 128);
    }

    // 聚光相关计算
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // 距离衰减
    float dist = length(light.position - vs_objData.FragPos);
    float attenuation = 1.0 / (1.0 + light.linear * dist + light.quadratic * (dist * dist));

    return (ambient + diffuse + specular) * intensity * attenuation;
}