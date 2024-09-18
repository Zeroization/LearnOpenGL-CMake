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

// ����������ƽ�̵���Ļ��
vec2 noiseScale = vec2(1280.0 / 16.0, 768.0 / 16.0);

void main()
{
	// ��ȡSSAO�㷨������
	vec3 fragPos = texture(gPosition, TexCoords).xyz;
	vec3 normal = normalize(texture(gNormal, TexCoords).xyz);
	vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);

	// ���㽫���߿ռ�����ת�����۲�ռ��TBN����
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = normalize(cross(normal, tangent));
	mat3 TBN = mat3(tangent, bitangent, normal);

	// �����������е�����, �õ��ڱ�����
	float occlusion = 0.0;
	for (int i = 0; i < kernelSize; ++i)
	{
		// ��ȡ������λ��
		vec3 samplePos = TBN * samples[i];	// ���߿ռ� -> �۲�ռ�
		samplePos = fragPos + samplePos * sampleRadius;

		// ������λ��ͶӰ������������, �Ӷ���ȡ��Ļλ��
		vec4 offset = vec4(samplePos, 1.0);
		offset = projection * offset;		// �۲�ռ� -> �ü��ռ�
		offset.xyz /= offset.w;				// ͸�ӳ���
		offset.xyz = offset.xyz * 0.5 + 0.5;// ӳ�䵽[0, 1]��

		// ��ȡ����������
		float sampleDepth = texture(gPosition, offset.xy).z;

		// ����ڱ�����
		float rangeCheck = smoothstep(0.0, 1.0, sampleRadius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + sampleBias ? 1.0 : 0.0) * rangeCheck;
	}

	// ��׼����������
	occlusion = 1.0 - (occlusion / kernelSize);
	FragColor = occlusion;
}