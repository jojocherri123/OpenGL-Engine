#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec2 texCoords;

in DATA
{
    vec2 texCoords;

} data_in[];


// Default main function
void main()
{   
    gl_Position = gl_in[0].gl_Position;
    texCoords = data_in[0].texCoords;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    texCoords = data_in[1].texCoords;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    texCoords = data_in[2].texCoords;
    EmitVertex();

    EndPrimitive();
}