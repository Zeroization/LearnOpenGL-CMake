#version 330 core
layout(location = 0) out vec4 FragColor;

struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material u_Material;

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light u_Light;

uniform vec3 u_CameraPos;

uniform bool u_EnableAmbient;
uniform bool u_EnableDiffuse;
uniform bool u_EnableSpecular;

in vec3 vs_objNormal;
in vec3 vs_objFragPos;
in vec2 vs_objTexCoords;

void main()
{
    // 环境光照
    vec3 ambient = vec3(0.0);
    if (u_EnableAmbient)
        ambient = texture(u_Material.diffuse, vs_objTexCoords).rgb * u_Light.ambient;

    // 漫反射
    float r = distance(u_Light.position, vs_objFragPos);
    vec3 norm = normalize(vs_objNormal);
    vec3 lightDir = normalize(u_Light.position - vs_objFragPos);
    vec3 diffuse = vec3(0.0);
    if (u_EnableDiffuse)
        diffuse = texture(u_Material.diffuse, vs_objTexCoords).rgb * (u_Light.diffuse / (r * r)) * max(0.0, dot(norm, lightDir));

    // 镜面反射
    vec3 viewDir = normalize(u_CameraPos - vs_objFragPos);
    vec3 halfVec = normalize(viewDir + lightDir);
    vec3 specular = vec3(0.0);
    if (u_EnableSpecular)
        specular = texture(u_Material.specular, vs_objTexCoords).rgb * (u_Light.specular / (r * r)) * pow(max(0.0, dot(norm, halfVec)), u_Material.shininess * 128);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
}