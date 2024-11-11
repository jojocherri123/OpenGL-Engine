#version 460 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float gamma;

const float offset_x = 1.0f / 1280.0f;  
const float offset_y = 1.0f / 720.0f;  

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
);

float kernel[9] = float[]
(
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
);

void main()
{
    vec3 color = vec3(0.0f);
    for(int i = 0; i < 9; i++)
        color += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * kernel[i];
    vec4 fragment = vec4(color, 1.0f);


    FragColor.rgba = pow(fragment.rgba, vec4(1.0f / gamma));

    // FragColor = vec4(vec3(1.0 - texture(screenTexture, texCoords)), 1.0);
    // FragColor = texture(screenTexture, texCoords);
}