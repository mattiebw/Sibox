#version 450 core

layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec3 TexCoords;

uniform samplerCube u_Skybox;

void main()
{
    FragColor = texture(u_Skybox, TexCoords);
}
