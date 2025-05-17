#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_ModelTransform;
uniform mat4 u_ViewProjection;

struct VertexOutput
{
    vec3 WorldPos;
    vec3 Normal;
    highp vec2 TexCoord;
};

layout (location = 0) out VertexOutput v_Out;

void main()
{
    v_Out.WorldPos = (u_ModelTransform * vec4(a_Position, 1.0)).xyz;
    gl_Position = u_ViewProjection * vec4(v_Out.WorldPos, 1.0);
    v_Out.Normal = mat3(transpose(inverse(u_ModelTransform))) * a_Normal; // MW @todo: Awful, calculate this on the CPU.
    v_Out.TexCoord = a_TexCoord;
}
