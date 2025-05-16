#version 450 core

struct VertexOutput
{
    vec3 WorldPos;
    vec3 Normal;
    highp vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;

uniform vec3 u_WireframeColor;

layout (location = 0) out vec4 o_Color;

void main()
{
    o_Color = vec4(u_WireframeColor, 1);
}
