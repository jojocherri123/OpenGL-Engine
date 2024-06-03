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


struct Material {
    float shininess;
};
uniform Material material;


struct PointLight {    
    vec3 position;
    vec4 color;
};
const int NR_POINT_LIGHTS = 2;
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec4 calcPointLight(PointLight light,vec3 normalVar, vec3 crntPosVar, vec3 viewDirectionVar){

    vec3 lightVec = light.position - crntPosVar;
    float dist = length(lightVec);
    float a = 0.2f;
    float b = 0.1f;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

    float ambient = 0.02f;

    vec3 normal = normalize(normalVar);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specular = 0.0f;
    
    if (diffuse !=0.0f){
        float specularLight = 0.5f;
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        vec3 halfwayVec = normalize(viewDirectionVar +lightDirection);
        float specularAmount = pow(max(dot(normal, halfwayVec), 0.0f), material.shininess);
        specular = specularAmount * specularLight;

    };

    return (texture(texture_diffuse1,texCoords)*(diffuse * inten + ambient )+ texture(texture_specular1,texCoords).r * specular * inten)* u_LightColor;
};

struct DirectionalLight {
    vec3 angle;
    vec4 color;
};
const int NR_DIR_LIGHTS = 1;
uniform DirectionalLight directionalLight[NR_DIR_LIGHTS];

vec4 calcDirectionalLight(DirectionalLight light,vec3 normalVar, vec3 viewDirectionVar){


    float ambient = 0.02f;

    vec3 normal = normalize(normalVar);
	vec3 lightDirection = normalize(light.angle);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specular = 0.0f;
    
    if (diffuse !=0.0f){
        float specularLight = 1.0f;
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        vec3 halfwayVec = normalize(viewDirectionVar +lightDirection);
        float specularAmount = pow(max(dot(normal, halfwayVec), 0.0f), material.shininess);
        specular = specularAmount * specularLight;

    };

    return (texture(texture_diffuse1,texCoords)*(diffuse + ambient )+ texture(texture_specular1,texCoords).r * specular)* u_LightColor;
};

struct SpotLight{
    vec3 angle;
    vec3 position;
    vec4 color;
};

const int NR_SPT_LIGHTS = 1;
uniform SpotLight spotLight[NR_DIR_LIGHTS];

vec4 calcSpotLight(SpotLight light,vec3 normalVar, vec3 crntPosVar, vec3 viewDirectionVar ){   
    vec3 lightVec = light.position - crntPosVar;

    float dist = length(lightVec);
    float a = 0.02f;
    float b = 0.01f;
    float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(normalVar);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirectionVar, reflectionDirection), 0.0f), material.shininess);
	float specular = specAmount * specularLight;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(light.angle, -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(texture_diffuse1, texCoords) * (diffuse *intensity *inten + ambient) + texture(texture_specular1, texCoords).r * specular * intensity*inten) * u_LightColor;
}

void main(){

    vec4 result = vec4(0.0);
    vec3 viewDirection = normalize(u_CamPos-crntPos);
    
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        result += calcPointLight(pointLights[i],normals, crntPos, viewDirection);
    }
    
    for(int i = 0; i < NR_DIR_LIGHTS; i++){
        result += calcDirectionalLight(directionalLight[i],normals, viewDirection);
    }

    for(int i = 0; i < NR_SPT_LIGHTS; i++){
        result += calcSpotLight(spotLight[i],normals, crntPos, viewDirection);
    }

    color = result;
}