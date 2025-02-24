#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in float a_FontAtlas;

uniform mat4 u_ViewProjection;

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
};

layout (location = 0) out VertexOutput v_Out;
layout (location = 2) out flat int v_FontAtlas;

void main()
{
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
    v_Out.Color = a_Color;
    v_Out.TexCoord = a_TexCoord;
    v_FontAtlas = int(a_FontAtlas);
}
