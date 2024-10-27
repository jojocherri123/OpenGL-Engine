#version 460 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 norms;
layout(location=2) in vec2 textureCoords;

uniform mat4 u_ModelMatrixLight;
uniform mat4 u_ViewMatrixLight;
uniform mat4 u_ProjectionLight;

out DATA
{
    vec3 crntPos;
    vec3 normals;
    vec2 texCoords;
    mat4 projection;
}data_out;

void main(){

    data_out.crntPos = vec3(u_ModelMatrixLight* vec4(position,1.0f));

    gl_Position = u_ViewMatrixLight*u_ModelMatrixLight* vec4(data_out.crntPos,1.0f);

    data_out.texCoords = textureCoords;
    data_out.normals = norms;
    data_out.projection = u_ProjectionLight;

}
