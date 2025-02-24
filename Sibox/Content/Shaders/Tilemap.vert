#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoordTopLeft;
layout (location = 2) in float a_Rot;

uniform mat4 uViewProjection;
uniform vec3 uPos;
uniform ivec2 uChunkSize;
uniform vec2 uTileSize;

layout (location = 0) out vec2 v_TexCoord;

void main()
{
    if (a_TexCoordTopLeft.x < 0.0)
    {
        gl_Position = vec4(2, 2, 2, 1); // Skip this vertex.
        return;
    }
    
    vec2 coord = vec2(gl_InstanceID % uChunkSize.x, gl_InstanceID / uChunkSize.x);
    vec3 pos = uPos + vec3(coord, 0.0) + a_Position;
    gl_Position = uViewProjection * vec4(pos, 1.0);

    vec2 texCoord = a_TexCoordTopLeft;
    int vertexID = gl_VertexID;
    vertexID = (vertexID + int(a_Rot)) % 4;
    // Vertex ID layout: top left is 3, top right is 2, bottom right is 1, bottom left is 0.
    switch (vertexID)
    {
        case 3:
            break;
        case 2:
            texCoord.x = texCoord.x + uTileSize.x;
            break;
        case 1:
            texCoord.x = texCoord.x + uTileSize.x;
            texCoord.y = texCoord.y + uTileSize.y;
            break;
        case 0:
            texCoord.y = texCoord.y + uTileSize.y;
            break;
    }

    v_TexCoord = texCoord;
}
