#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
    mat3 TBN;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool useNormalMap;
uniform bool useParallaxMap;
uniform float heightScale;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    // 给深度分层
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    
    // 记录当前层的深度值
    float currentLayerDepth = 0.0;
    
    // 每层的偏移量
    vec2 P = viewDir.xy * heightScale;
    vec2 deltaTexCoords = P / numLayers;
    
    // 步进找到合适的采样点
    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    // 获取上一个采样点
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
    // 获取两采样点的深度值
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
    // 线性插值
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoores = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoores; 
}

void main()
{
	vec3 normal;
	if (useNormalMap)
	{
		// 获得[0, 1]的法向量 
		normal = texture(normalMap, fs_in.TexCoords).rgb;
		// [0, 1] -> [-1, 1]
		normal = normalize(normal * 2.0 - 1.0);
        // 用TBN矩阵将法线转换到世界空间
        normal = normalize(fs_in.TBN * normal);
	}
	else
		normal = fs_in.Normal;

    // 视差贴图
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec2 texCoords = useParallaxMap ? ParallaxMapping(fs_in.TexCoords, viewDir) : fs_in.TexCoords;
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    vec3 color = texture(diffuseMap, texCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}