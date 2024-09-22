#version 330 core
out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 camPos;

// 物体材质参数
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

uniform bool enableTexture;
uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// 光源信息
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

const float PI = 3.14159265359;

// D项: Trowbridge-Reitz GGX
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

// F项: Fresnel-Schlick近似
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// G项: Schlick-GGX
float GeometrySchlickGGX(float NdotV, float roughness)
{
	// 重映射粗糙度
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}
// G项: Smith's method
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 *ggx2;
}

// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anyways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
	vec3 N = normalize(Normal);								// normal
	vec3 V = normalize(camPos - WorldPos);					// viewDir

	vec3 Albedo = albedo;
	float Metallic =  metallic;
	float Roughness =  roughness;
	float Ao = ao;

	if (enableTexture)
	{
		Albedo    = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
		Metallic  = texture(metallicMap, TexCoords).r;
		Roughness = texture(roughnessMap, TexCoords).r;
		Ao        = texture(aoMap, TexCoords).r;
		N = getNormalFromMap();
	}

	// 特殊指定F0
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, Albedo, Metallic);
	
	// 求反射方程
	vec3 Lo = vec3(0.0);
	for (int i = 0; i <4; ++i)
	{
		vec3 L = normalize(lightPositions[i] - WorldPos);	// lightDir
		vec3 H = normalize(V + L);							// halfwayVec

		// 求每个光源的辐射亮度Radiance
		float distance = length(lightPositions[i] - WorldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lightColors[i] * attenuation;
		
		// 求BRDF
		// 求Cook-Torrance镜面反射BRDF
		float NDF = DistributionGGX(N, H, Roughness);
		float G = GeometrySmith(N, V, L, Roughness);
		vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
		vec3 kS = F;

		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) *max(dot(N, L), 0.0) + 0.0001;
		vec3 specular = numerator / denominator;

		// 求Lambertian漫反射BRDF
		vec3 diffuse = Albedo / PI;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - Metallic;

		// 最终结果
		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * diffuse + specular) * radiance * NdotL;
	}

	// 环境光照项(后续通过IBL采样)
	vec3 ambient = vec3(0.03) * Albedo * Ao;
	
	vec3 color = ambient + Lo;
	// HDR的色调映射: Rainhard方法
	color = color / (color + vec3(1.0));
	// Gamma校正
	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);
}