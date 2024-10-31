#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;

out vs_objOUT
{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
} vs_objData;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat3 u_Normal;
uniform bool u_useDualQuat;
uniform mat4 u_FinalBonesMatrices[MAX_BONES];
uniform mat2x4 u_FinalBonesDQs[MAX_BONES];

void useDirectQuat()
{
	vec4 totalPosition = vec4(0.0f);
	vec3 totalNormal = vec3(0.0f);
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (boneIds[i] == -1)
			continue;
		if (boneIds[i] >= MAX_BONES)
		{
			totalPosition = vec4(position, 1.0f);
			totalNormal = normal;
			break;
		}
		vec4 localPosition = u_FinalBonesMatrices[boneIds[i]] * vec4(position, 1.0f);
		totalPosition += localPosition * weights[i];
		vec3 localNormal = mat3(u_FinalBonesMatrices[boneIds[i]]) * normal;
		totalNormal += localNormal * weights[i];
	}

	vs_objData.Normal = u_Normal * normalize(totalNormal);
	vs_objData.FragPos = vec3(u_Model * totalPosition);
	vs_objData.TexCoords = texCoords;

	gl_Position = u_MVP * totalPosition;
}

// 获取加权且插值后的对偶四元数
mat2x4 getBoneTransform()
{
	// 获取影响该顶点的对偶四元数
	mat2x4 dq0 = u_FinalBonesDQs[boneIds.x];
	mat2x4 dq1 = u_FinalBonesDQs[boneIds.y];
	mat2x4 dq2 = u_FinalBonesDQs[boneIds.z];
	mat2x4 dq3 = u_FinalBonesDQs[boneIds.w];

	// 根据最短旋转路径加权, 防止动作发生突变
	vec4 shortestWeights = weights;
	shortestWeights.y *= sign(dot(dq0[0], dq1[0]));
	shortestWeights.z *= sign(dot(dq0[0], dq2[0]));
	shortestWeights.w *= sign(dot(dq0[0], dq3[0]));

	// 得到加权插值后的对偶四元数, 别忘了标准化
	mat2x4 result = shortestWeights.x * dq0 +
					shortestWeights.y * dq1 +
					shortestWeights.z * dq2 +
					shortestWeights.w * dq3;
	float norm = length(result[0]);
	return result / norm;
}

mat4 getSkinMatFromDQ()
{
	mat2x4 boneDQ = getBoneTransform();

	vec4 r = boneDQ[0];		// 旋转
	vec4 t = boneDQ[1];		// 平移

	return mat4(
		1.0 - (2.0 * r.y * r.y) - (2.0 * r.z * r.z),
              (2.0 * r.x * r.y) + (2.0 * r.w * r.z),
              (2.0 * r.x * r.z) - (2.0 * r.w * r.y),
        0.0,

              (2.0 * r.x * r.y) - (2.0 * r.w * r.z),
        1.0 - (2.0 * r.x * r.x) - (2.0 * r.z * r.z),
              (2.0 * r.y * r.z) + (2.0 * r.w * r.x),
        0.0,

              (2.0 * r.x * r.z) + (2.0 * r.w * r.y),
              (2.0 * r.y * r.z) - (2.0 * r.w * r.x),
        1.0 - (2.0 * r.x * r.x) - (2.0 * r.y * r.y),
        0.0,

        2.0 * (-t.w * r.x + t.x * r.w - t.y * r.z + t.z * r.y),
        2.0 * (-t.w * r.y + t.x * r.z + t.y * r.w - t.z * r.x),
        2.0 * (-t.w * r.z - t.x * r.y + t.y * r.x + t.z * r.w),
        1
	);
}

void useDualQuat()
{
	mat4 skinMat = getSkinMatFromDQ();
	
	vs_objData.Normal = u_Normal * normalize(mat3(skinMat) * normal);
	vs_objData.FragPos = vec3(u_Model * skinMat * vec4(position, 1.0));
	vs_objData.TexCoords = texCoords;

	gl_Position = u_MVP * skinMat * vec4(position, 1.0);
}

void main()
{
	if (u_useDualQuat)
	{
		useDualQuat();
	}
	else 
	{
		useDirectQuat();
	}
};