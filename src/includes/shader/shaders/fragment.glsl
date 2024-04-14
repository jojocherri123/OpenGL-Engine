#version 460 core
out vec4 color;

in vec2 texCoords;
in vec3 normals;
in vec3 crntPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec4 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_CamPos;

vec4 pointLight(){


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

    return texture(texture_diffuse1,texCoords)* u_LightColor *(diffuse + ambient )+ texture(texture_specular1,texCoords) * specular * u_LightColor;
}

void main(){


    color = pointLight();

}
