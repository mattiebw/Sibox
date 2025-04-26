#version 450 core

layout (location = 0) in vec2 v_TexCoord;

uniform sampler2D uTexture;

layout (location = 0) out vec4 o_Color;

void main()
{
    vec4 texColor = texture(uTexture, v_TexCoord);
    // texColor = vec4(v_TexCoord.x, v_TexCoord.y, 0.0f, 1.0f);
    // texColor = vec4(v_TestCol, 1.0f);

    if (texColor.a == 0.0) discard;

    o_Color = texColor;
}
