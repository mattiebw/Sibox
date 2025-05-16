#version 450 core

struct VertexOutput
{
    vec3 WorldPos;
    vec3 Normal;
    vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;

layout (binding = 0) uniform sampler2D u_Textures[32];

layout (location = 0) out vec4 o_Color;

void main()
{
    o_Color = texture(u_Textures[0], Input.TexCoord);
}
