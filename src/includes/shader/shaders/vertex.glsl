#version 460 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 norms;
layout(location=2) in vec2 textureCoords;
layout(location=3) in vec3 vertTanjent;
layout(location=3) in vec3 vertBiTanjent;


out DATA
{
    vec3 crntPos;
    vec3 normals;
    vec2 texCoords;
    mat4 projection;

} data_out;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

void main(){


    data_out.crntPos = vec3(u_ModelMatrix* vec4(position,1.0f));

    gl_Position = u_ViewMatrix*u_ModelMatrix* vec4(data_out.crntPos,1.0f);

    data_out.normals = norms;
    data_out.texCoords = textureCoords;
    data_out.projection = u_Projection;
}