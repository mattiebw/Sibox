#version 450 core

layout (location = 0) in vec3 a_Position;

layout (location = 0) out vec3 TexCoords;

uniform mat4 u_ViewProjection;

void main()
{
    TexCoords = a_Position;
    vec4 pos = u_ViewProjection * vec4(a_Position, 1.0);
    gl_Position = pos.xyww; // Z coordinate will always be 1, meaning depth is always 1.
}
