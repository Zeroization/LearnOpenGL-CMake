#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace[5];
} fs_in;

uniform sampler2D shadowMap[5];
uniform sampler2D diffuseTexture;

uniform vec3 lightPos[5];

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, int idx)
{
    // 标准化齐次坐标
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 将NDC空间坐标[-1, 1]映射到uv[0, 1]
    projCoords = projCoords * 0.5 + 0.5;

    // 离光太远了, 假设没有影子
    if (projCoords.z > 1)
        return 0.0;

    // 从深度贴图采样, 简单PCF(3x3采样)
    float shadow = 0.0;
    float currentDepth = projCoords.z;
    vec2 texelSize = 1.0 / textureSize(shadowMap[idx], 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap[idx], projCoords.xy + vec2(x, y) * texelSize).r;
            // 解决Shadow Acne问题
            float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
 
    return shadow;
}

void main()
{   
    vec3 normal = normalize(fs_in.Normal);

    // calculate shadow
    float shadow = 0.0;
    for (int i = 0; i < 5; ++i)
    {
        vec3 lightDir = normalize(lightPos[i] - fs_in.FragPos);
        shadow += ShadowCalculation(fs_in.FragPosLightSpace[i], normal, lightDir, i);
    }
    shadow = clamp(shadow, 0.0, 1.0);

    vec3 diffuse = texture(diffuseTexture, fs_in.TexCoords).rgb;
    FragColor = vec4((1.0 - shadow) * diffuse, 1.0);
}