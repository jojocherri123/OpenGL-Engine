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

    vec3 lightVec = u_LightPosition - crntPos;
    float dist = length(lightVec);
    float a = 0.5;
    float b = 0.1;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

    float ambient = 0.02f;

    vec3 normal = normalize(normals);
	vec3 lightDirection = normalize(lightVec);
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

    return (texture(texture_diffuse1,texCoords)*(diffuse * inten + ambient )+ texture(texture_specular1,texCoords).r * specular * inten)* u_LightColor;
}

vec4 directionalLight(){

    float ambient = 0.02f;

    vec3 normal = normalize(normals);
	vec3 lightDirection = normalize(u_LightPosition);
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

    return (texture(texture_diffuse1,texCoords)*(diffuse + ambient )+ texture(texture_specular1,texCoords).r * specular)* u_LightColor;
}

vec4 spotLight(){
    vec3 lightVec = u_LightPosition - crntPos;

    float outerCone = 0.90f;
    float innerCone = 0.95f;

    float ambient = 0.02f;

    vec3 normal = normalize(normals);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);


    float specularLight = 0.5f;
    vec3 viewDirection = normalize(u_CamPos-crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    vec3 halfwayVec = normalize(viewDirection +lightDirection);
    float specularAmount = pow(max(dot(normal, halfwayVec), 0.0f), 8);
    float specular = specularAmount * specularLight;

    float angle = dot(vec3(0.0f,0.0f,1.0f),-lightDirection);
    float inten = clamp((angle-outerCone)/(innerCone-outerCone), 0.0f,1.0f);

    return (texture(texture_diffuse1,texCoords)*(diffuse * inten + ambient )+ texture(texture_specular1,texCoords).r * specular * inten)* u_LightColor;
}

void main(){


    color = directionalLight();

}
