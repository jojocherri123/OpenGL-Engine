#version 460 core

layout(location=0) in vec2 position;
layout(location=1) in vec2 texCoords;

out DATA
{
    vec2 texCoords;
}data_out;

void main(){


    gl_Position = vec4(position.x,position.y,0.0,1.0);

    data_out.texCoords = texCoords;

}
