#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	mat3 TBN;
	// vec3 TangentLightPos;
	// vec3 TangentViewPos;
    // vec3 TangentFragPos;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vs_out.Normal = normalMatrix * aNormal;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;

	// 计算TBN
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	// 格拉姆-施密特正交化TN
	T = normalize(T - dot(T, N) * N);
	// 用正交化后的TN求B
	vec3 B = normalize(cross(N, T));
	// 法1: 用TBN矩阵将表面法向量转换到世界空间
	vs_out.TBN = mat3(T, B, N);
	// 法2: 用TBN的逆矩阵将光照相关变量转换到TBN空间
	// mat3 inv_TBN = transpose(mat3(T, B, N));
	// vs_out.TangentLightPos = inv_TBN * lightPos;
	// vs_out.TangentViewPos  = inv_TBN * viewPos;
    // vs_out.TangentFragPos  = inv_TBN * vec3(model * vec4(aPos, 1.0));
	
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}