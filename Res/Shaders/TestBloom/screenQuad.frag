#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D hdrScene;
uniform sampler2D bloomBlur;
uniform float exposure;

void main()
{   
    // µþ¼Ó»ìºÏ
    vec3 hdrColor = texture(hdrScene, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    hdrColor += bloomColor;

    // reinhard tone mapping
    // vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    FragColor = vec4(mapped, 1.0);
}