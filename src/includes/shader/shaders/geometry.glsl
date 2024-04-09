#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 normals;
out vec3 crntPos;
out vec2 texCoords;

in DATA
{
    vec3 crntPos;
    vec3 normals;
    vec2 texCoords;
    mat4 projection;
} data_in[];


// Default main function
void main()
{
    gl_Position = data_in[0].projection * gl_in[0].gl_Position;
    normals = data_in[0].normals;
    crntPos = data_in[0].crntPos;
    texCoords = data_in[0].texCoords;
    EmitVertex();

    gl_Position = data_in[1].projection * gl_in[1].gl_Position;
    normals = data_in[1].normals;
    crntPos = data_in[1].crntPos;
    texCoords = data_in[1].texCoords;
    EmitVertex();

    gl_Position = data_in[2].projection * gl_in[2].gl_Position;
    normals = data_in[2].normals;
    crntPos = data_in[2].crntPos;
    texCoords = data_in[2].texCoords;
    EmitVertex();

    EndPrimitive();
}