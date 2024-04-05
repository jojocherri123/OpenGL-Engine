#version 460 core

layout(location=0) in vec3 position;

uniform mat4 u_ModelMatrixLight;
uniform mat4 u_ViewMatrixLight;
uniform mat4 u_ProjectionLight;

void main(){


    vec4 newPosition = u_ProjectionLight*u_ViewMatrixLight*u_ModelMatrixLight* vec4(position,1.0f);

    gl_Position = vec4(newPosition.x,newPosition.y,newPosition.z,newPosition.w);

}
