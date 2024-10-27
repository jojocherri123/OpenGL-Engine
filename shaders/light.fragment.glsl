#version 460 core

out vec4 color;

uniform vec4 u_LightColor;

void main(){

    color = u_LightColor;
}