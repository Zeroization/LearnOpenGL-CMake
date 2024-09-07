#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D shadowMap;
uniform sampler2D diffuseTexture;

uniform vec3 lightPos;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // 标准化齐次坐标
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 将NDC空间坐标[-1, 1]映射到uv[0, 1]
    projCoords = projCoords * 0.5 + 0.5;

    // 从深度贴图采样
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main()
{   
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);

    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, normal, lightDir);   
    vec3 diffuse = texture(diffuseTexture, fs_in.TexCoords).rgb;
    FragColor = vec4((1.0 - shadow) * diffuse, 1.0);
}