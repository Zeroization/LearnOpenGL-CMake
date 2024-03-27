#version 330 core
layout(location = 0) out vec4 FragColor;

in vec3 vs_TexCoords;

uniform samplerCube u_Skybox;

void main()
{    
    FragColor = texture(u_Skybox, vs_TexCoords);
}