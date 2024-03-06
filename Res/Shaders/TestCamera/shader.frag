#version 330 core

in vec2 v_TexCoord;

// uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
	// vec4 texColor = texture(u_Texture, v_TexCoord);
	// color = texColor;

	gl_FragColor = u_Color;
};