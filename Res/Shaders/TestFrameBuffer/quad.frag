#version 330 core
layout(location = 0) out vec4 FragColor;

uniform sampler2D u_ScreenTexture;
uniform int u_PostProcessParam;

in vec2 vs_quadTexCoords;

vec4 convolution();

void main()
{
	switch(u_PostProcessParam)
	{
		// Default
		case 0:
			FragColor = texture(u_ScreenTexture, vs_quadTexCoords);
			break;
		// Inversion
		case 1:
			FragColor = vec4(vec3(1.0) - texture(u_ScreenTexture, vs_quadTexCoords).rgb, 1.0);
			break;
		// GrayScale
		case 2:
			FragColor = texture(u_ScreenTexture, vs_quadTexCoords);
			float avg = (FragColor.r + FragColor.g + FragColor.b) / 3.0; 
			FragColor = vec4(avg, avg, avg, 1.0);
			break;
		// convolution
		default:
			FragColor = convolution();
			break;
	}
}

vec4 convolution()
{
	// 用于卷积计算的偏移量数组
	const float offset = 1.0 / 300.0;  
	vec2 offsets[9] = vec2[](
		vec2(-offset,  offset), // 左上
		vec2( 0.0f,    offset), // 正上
		vec2( offset,  offset), // 右上
		vec2(-offset,  0.0f),   // 左
		vec2( 0.0f,    0.0f),   // 中
		vec2( offset,  0.0f),   // 右
		vec2(-offset, -offset), // 左下
		vec2( 0.0f,   -offset), // 正下
		vec2( offset, -offset)  // 右下
	);

	// 卷积核
	float kernel[9];
	switch (u_PostProcessParam)
	{
		// Sharpen
		case 3:
			kernel = float[](
				-1.0f, -1.0f, -1.0f,
				-1.0f,  9.0f, -1.0f,
				-1.0f, -1.0f, -1.0f
			);
			break;
		// Blur
		case 4:
			kernel = float[](
				1.0f / 16, 2.0f / 16, 1.0f / 16,
				2.0f / 16, 4.0f / 16, 2.0f / 16,
				1.0f / 16, 2.0f / 16, 1.0f / 16
			);
			break;
		// Edge-Detection
		case 5:
			kernel = float[](
				1.0f,  1.0f, 1.0f,
				1.0f, -8.0f, 1.0f,
				1.0f,  1.0f, 1.0f
			);
			break;
	}

	// 卷积计算 - 先算分量
	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(u_ScreenTexture, vs_quadTexCoords.st + offsets[i]));
	}
	// 卷积计算 - 再卷积和
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];

	return vec4(col, 1.0);
}