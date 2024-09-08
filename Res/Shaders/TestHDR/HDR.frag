#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform float exposure;

void main()
{             
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
  
    // reinhard tone mapping
    // vec3 mapped = hdrColor / (hdrColor + vec3(1.0));

    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
  
    FragColor = vec4(mapped, 1.0);
}