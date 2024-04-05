#version 460 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 norms;
layout(location=2) in vec2 textureCoords;


uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;


out vec2 texCoords;
out vec3 normals;
out vec3 crntPos;

void main(){

    crntPos = vec3(u_ModelMatrix* vec4(position,1.0f));

    vec4 newPosition = u_Projection*u_ViewMatrix*u_ModelMatrix* vec4(crntPos,1.0f);

    gl_Position = vec4(newPosition.x,newPosition.y,newPosition.z,newPosition.w);

    texCoords = textureCoords;
    normals = norms;
}
