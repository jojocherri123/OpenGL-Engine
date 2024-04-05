#version 460 core


out vec4 color;

in vec2 texCoords;
in vec3 normals;
in vec3 crntPos;

uniform sampler2D tex;
uniform vec4 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_CamPos;

void main(){

    float ambient = 0.02f;

    vec3 normal = normalize(normals);
	vec3 lightDirection = normalize(u_LightPosition - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specular = 0.0f;
    
    if (diffuse !=0.0f){
        float specularLight = 0.5f;
        vec3 viewDirection = normalize(u_CamPos-crntPos);
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        vec3 halfwayVec = normalize(viewDirection +lightDirection);
        float specularAmount = pow(max(dot(normal, halfwayVec), 0.0f), 8);
        specular = specularAmount * specularLight;

    };

    color = texture(tex,texCoords) *u_LightColor *(diffuse + ambient + specular);

}
