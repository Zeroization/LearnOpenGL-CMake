#version 330 core
layout(location = 0) out vec4 FragColor;

in vec2 vs_quadTexCoords;

uniform sampler2D u_ScreenTexture;

void main()
{
	FragColor = texture(u_ScreenTexture, vs_quadTexCoords);
}