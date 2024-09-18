#version 330 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];
uniform int kernelSize;		// 64
uniform float sampleRadius;	// 0.5
uniform float sampleBias;	// 0.025

uniform int screenWidth;
uniform int screenHeight;

uniform mat4 projection;

// 将噪声纹理平铺到屏幕上
vec2 noiseScale = vec2(1280.0 / 16.0, 768.0 / 16.0);

void main()
{
	// 获取SSAO算法的输入
	vec3 fragPos = texture(gPosition, TexCoords).xyz;
	vec3 normal = normalize(texture(gNormal, TexCoords).xyz);
	vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);

	// 计算将切线空间向量转换到观察空间的TBN矩阵
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = normalize(cross(normal, tangent));
	mat3 TBN = mat3(tangent, bitangent, normal);

	// 迭代采样核中的样本, 得到遮蔽因子
	float occlusion = 0.0;
	for (int i = 0; i < kernelSize; ++i)
	{
		// 获取采样的位置
		vec3 samplePos = TBN * samples[i];	// 切线空间 -> 观察空间
		samplePos = fragPos + samplePos * sampleRadius;

		// 将采样位置投影到采样材质上, 从而获取屏幕位置
		vec4 offset = vec4(samplePos, 1.0);
		offset = projection * offset;		// 观察空间 -> 裁剪空间
		offset.xyz /= offset.w;				// 透视除法
		offset.xyz = offset.xyz * 0.5 + 0.5;// 映射到[0, 1]上

		// 获取采样点的深度
		float sampleDepth = texture(gPosition, offset.xy).z;

		// 获得遮蔽因子
		float rangeCheck = smoothstep(0.0, 1.0, sampleRadius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + sampleBias ? 1.0 : 0.0) * rangeCheck;
	}

	// 标准化缩放因子
	occlusion = 1.0 - (occlusion / kernelSize);
	FragColor = occlusion;
}