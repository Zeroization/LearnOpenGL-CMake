#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D baseLayerImg;
uniform sampler2D blendLayerImg;
uniform vec3 blendLayerColor;
uniform bool useImageBlend;

uniform float opacity;
uniform int blendMode;

// 0 - Normal 正常
vec3 blendNormal(vec3 base, vec3 blend) {
	return blend;
}

// 1 - Dissolve 溶解
float random (vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))* 43758.5453123);
}
vec3 blendDissolve(vec3 base, vec3 blend, float weight)
{
	// 生成随机数
	float rand = random(TexCoord);
	rand = (rand + 1) * 0.5;
	if (rand * weight >= 0.4)
		return blend;
	else
		return base;
}

// 2 - Darken 变暗
float blendDarken(float base, float blend) {
	return min(blend, base);
}
vec3 blendDarken(vec3 base, vec3 blend) {
	return vec3(blendDarken(base.r, blend.r), blendDarken(base.g, blend.g), blendDarken(base.b, blend.b));
}

// 3 - Multiply 正片叠底
vec3 blendMultiply(vec3 base, vec3 blend) {
	return base * blend;
}

// 4 - ColorBurn 颜色加深
float blendColorBurn(float base, float blend) {
	return (blend == 0.0) ? blend : max((1.0 - ((1.0 - base) / blend)), 0.0);
}
vec3 blendColorBurn(vec3 base, vec3 blend) {
	return vec3(blendColorBurn(base.r, blend.r), blendColorBurn(base.g, blend.g), blendColorBurn(base.b, blend.b));
}

// 5 - LinearBurn 线性加深
float blendLinearBurn(float base, float blend) {
	return max(base + blend- 1.0, 0.0);
}
vec3 blendLinearBurn(vec3 base, vec3 blend) {
	return max(base + blend - vec3(1.0), vec3(0.0));
}

// 6 - DarkerColor 深色
vec3 blendDarkerColor(vec3 base, vec3 blend)
{
	float sumBase = base.r + base.g + base.b;
	float sumBlend = blend.r + blend.g + blend.b;
	return (sumBase <= sumBlend) ? base : blend;
}

// 7 - Lighten	变亮
float blendLighten(float base, float blend) {
	return max(blend, base);
}
vec3 blendLighten(vec3 base, vec3 blend) {
	return vec3(blendLighten(base.r, blend.r), blendLighten(base.g, blend.g), blendLighten(base.b, blend.b));
}

// 8 - Screen	滤色
float blendScreen(float base, float blend) {
	return 1.0 - ((1.0 - base) * (1.0 - blend));
}
vec3 blendScreen(vec3 base, vec3 blend) {
	return vec3(blendScreen(base.r, blend.r), blendScreen(base.g, blend.g), blendScreen(base.b, blend.b));
}

// 9 - ColorDodge,		// 颜色减淡
float blendColorDodge(float base, float blend) {
	return (blend == 1.0) ? blend : min(base / (1.0 - blend), 1.0);
}
vec3 blendColorDodge(vec3 base, vec3 blend) {
	return vec3(blendColorDodge(base.r, blend.r), blendColorDodge(base.g, blend.g), blendColorDodge(base.b, blend.b));
}

// 10 - LinearDodge,	// 线性减淡/添加
float blendLinearDodge(float base, float blend) {
	return min(base + blend, 1.0);
}
vec3 blendLinearDodge(vec3 base, vec3 blend) {
	return min(base + blend, vec3(1.0));
}

// 11 - LighterColor,	// 浅色
vec3 blendLighterColor(vec3 base, vec3 blend)
{
	float sumBase = base.r + base.g + base.b;
	float sumBlend = blend.r + blend.g + blend.b;
	return (sumBase >= sumBlend) ? base : blend;
}

// 12 - Overlay,		// 叠加
float blendOverlay(float base, float blend) {
	return base <= 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend));
}
vec3 blendOverlay(vec3 base, vec3 blend) {
	return vec3(blendOverlay(base.r, blend.r), blendOverlay(base.g, blend.g), blendOverlay(base.b, blend.b));
}

// 13 - SoftLight,		// 柔光
float blendSoftLight(float base, float blend) {
	return (blend <= 0.5) ? (2.0 * base * blend + base * base * (1.0 - 2.0 * blend)) : (sqrt(base) * (2.0 * blend - 1.0) + 2.0 * base * (1.0 - blend));
}
vec3 blendSoftLight(vec3 base, vec3 blend) {
	return vec3(blendSoftLight(base.r, blend.r), blendSoftLight(base.g, blend.g), blendSoftLight(base.b, blend.b));
}

// 14 - HardLight,		// 强光
float blendHardLight(float base, float blend) {
	return blend <= 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend));
}
vec3 blendHardLight(vec3 base, vec3 blend) {
	return vec3(blendHardLight(base.r, blend.r), blendHardLight(base.g, blend.g), blendHardLight(base.b, blend.b));
}

// 15 - VividLight,		// 亮光
float blendVividLight(float base, float blend) {
	return (blend<0.5) ? blendColorBurn(base, (2.0 * blend)) : blendColorDodge(base, (2.0 * (blend - 0.5)));
}
vec3 blendVividLight(vec3 base, vec3 blend) {
	return vec3(blendVividLight(base.r,blend.r),blendVividLight(base.g,blend.g),blendVividLight(base.b,blend.b));
}

// 16 - LinearLight,	// 线性光
float blendLinearLight(float base, float blend) {
	return base + 2.0 * blend - 1;
}
vec3 blendLinearLight(vec3 base, vec3 blend) {
	return vec3(blendLinearLight(base.r, blend.r), blendLinearLight(base.g, blend.g), blendLinearLight(base.b, blend.b));
}

// 17 - PinLight,		// 点光
float blendPinLight(float base, float blend) {
	return (blend <= 0.5) ? blendDarken(base, (2.0 * blend)) : blendLighten(base, (2.0 * ( blend - 0.5)));
}
vec3 blendPinLight(vec3 base, vec3 blend) {
	return vec3(blendPinLight(base.r, blend.r), blendPinLight(base.g, blend.g), blendPinLight(base.b, blend.b));
}

// 18 - HardMix,		// 实色混合
float blendHardMix(float base, float blend) {
	return (base + blend >= 1) ? 1.0 : 0.0;
}
vec3 blendHardMix(vec3 base, vec3 blend) {
	return vec3(blendHardMix(base.r, blend.r), blendHardMix(base.g, blend.g), blendHardMix(base.b, blend.b));
}

// 19 - Difference,		// 差值
vec3 blendDifference(vec3 base, vec3 blend) {
	return abs(base - blend);
}

// 20 - Exclusion,		// 排除
vec3 blendExclusion(vec3 base, vec3 blend) {
	return base + blend - 2.0 * base * blend;
}

// 21 - Subtract,		// 减去
vec3 blendSubstract(vec3 base, vec3 blend) {
	return max(base + blend - vec3(1.0), vec3(0.0));
}

// 22 - Divide,			// 划分
float blendDivide(float base, float blend) {
	return clamp(base / blend, 0.0, 1.0);
}
vec3 blendDivide(vec3 base, vec3 blend) {
	return vec3(blendDivide(base.r, blend.r), blendDivide(base.g, blend.g), blendDivide(base.b, blend.b));
}


vec3 blend(int mode, vec3 baseLayer, vec3 blendLayer) 
{
	switch (mode)
	{
		case 0: return blendNormal(baseLayer, blendLayer);
		case 1: return blendDissolve(baseLayer, blendLayer, opacity);
		case 2: return blendDarken(baseLayer, blendLayer);
		case 3: return blendMultiply(baseLayer, blendLayer);
		case 4: return blendColorBurn(baseLayer, blendLayer);
		case 5: return blendLinearBurn(baseLayer, blendLayer);
		case 6: return blendDarkerColor(baseLayer, blendLayer);
		case 7: return blendLighten(baseLayer, blendLayer);
		case 8: return blendScreen(baseLayer, blendLayer);
		case 9: return blendColorDodge(baseLayer, blendLayer);
		case 10:return blendLinearDodge(baseLayer, blendLayer);
		case 11:return blendLighterColor(baseLayer, blendLayer);
		case 12:return blendOverlay(baseLayer, blendLayer);
		case 13:return blendSoftLight(baseLayer, blendLayer);
		case 14:return blendHardLight(baseLayer, blendLayer);
		case 15:return blendVividLight(baseLayer, blendLayer);
		case 16:return blendLinearLight(baseLayer, blendLayer);
		case 17:return blendPinLight(baseLayer, blendLayer);
		case 18:return blendHardMix(baseLayer, blendLayer);
		case 19:return blendDifference(baseLayer, blendLayer);
		case 20:return blendExclusion(baseLayer, blendLayer);
		case 21:return blendSubstract(baseLayer, blendLayer);
		case 22:return blendDivide(baseLayer, blendLayer);
	}

	return baseLayer;
}

void main()
{
	vec3 baseLayer = texture(baseLayerImg, TexCoord).rgb;
	vec3 blendLayer = useImageBlend ? texture(blendLayerImg, TexCoord).rgb : blendLayerColor;
	vec3 blendResult = blend(blendMode, baseLayer, blendLayer);

	// 透明度处理
	float alpha = useImageBlend ? texture(blendLayerImg, TexCoord).a : 1.0;
	blendResult = blendResult * (alpha * opacity) + baseLayer * (1.0 - opacity * alpha);

	FragColor = vec4(blendResult, 1.0);
}