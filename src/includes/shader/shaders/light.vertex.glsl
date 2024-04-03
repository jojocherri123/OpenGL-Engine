#version 460 core

layout(location=0) in vec4 position;


void main(){


    vec4 newPosition = u_Projection*u_ViewMatrix*u_ModelMatrix* vec4(position,1.0f);

    gl_Position = vec4(position.x,position.y,position.z,position.w);

}