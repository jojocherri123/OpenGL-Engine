#version 460 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 norms;

uniform mat4 u_ModelMatrixLight;
uniform mat4 u_ViewMatrixLight;
uniform mat4 u_ProjectionLight;


vec3 crntPos;

void main(){

    crntPos = vec3(u_ModelMatrixLight* vec4(position,1.0f));

    gl_Position = u_ProjectionLight*u_ViewMatrixLight*u_ModelMatrixLight* vec4(crntPos,1.0f);


}
