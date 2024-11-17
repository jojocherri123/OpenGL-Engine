#version 460 core
out vec4 color;

in vec2 texCoords;
in vec3 normals;
in vec3 crntPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 u_LightPosition;
uniform vec3 u_CamPos;

uniform int NR_POINT_LIGHTS;
uniform int NR_SPT_LIGHTS;
uniform int NR_DIR_LIGHTS;

struct Material {
    float shininess;
};
uniform Material material;

struct PointLight {
    vec3 position;
    vec4 color;
    float intensity;
};
const int MAX_POINT_LIGHTS = 128;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

struct SpotLight{
    vec3 angle;
    vec3 position;
    vec4 color;
    float intensity;
};
const int MAX_SPT_LIGHTS = 128;
uniform SpotLight spotLight[MAX_SPT_LIGHTS];

struct DirectionalLight {
    vec3 angle;
    vec4 color;
    float intensity;
};

const int MAX_DIR_LIGHTS = 16;
uniform DirectionalLight directionalLight[MAX_DIR_LIGHTS];


vec4 calcPointLight(PointLight light,vec3 normalVar, vec3 crntPosVar, vec3 viewDirectionVar){

    vec3 lightVec = light.position - crntPosVar;
    float dist = length(lightVec);
    float a = 0.2f;
    float b = 0.1f;
    float inten = light.intensity / (a * dist * dist + b * dist + 1.0f);

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

    if (texture(texture_diffuse1,texCoords).a < 0.01){
        discard;
    }

    return (texture(texture_diffuse1,texCoords)*(diffuse * inten + ambient )+ texture(texture_specular1,texCoords).r * specular * inten)* light.color;
};


vec4 calcSpotLight(SpotLight light,vec3 normalVar, vec3 crntPosVar, vec3 viewDirectionVar ){   
    vec3 lightVec = light.position - crntPosVar;

    float dist = length(lightVec);
    float a = 0.2f;
    float b = 0.1f;
    float intensity = light.intensity / (a * dist * dist + b * dist + 1.0f);

	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.02f;

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

    if (texture(texture_diffuse1,texCoords).a < 0.01){
        discard;
    }


	return (texture(texture_diffuse1, texCoords) * (diffuse *intensity *inten + ambient) + texture(texture_specular1, texCoords).r * specular * intensity*inten) * light.color;
}

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

    if (texture(texture_diffuse1,texCoords).a < 0.01){
        discard;
    }


    return (texture(texture_diffuse1,texCoords)*(diffuse + ambient )+ texture(texture_specular1,texCoords).r * specular)* light.color;
};




float near = 0.1f;
float far = 100.0f;

uniform float FogDensity;
uniform vec3 FogColor;

float linearizeDepth(float depth)
{
	return (FogDensity * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness = 0.5f, float offset = 5.0f)
{
	float zVal = linearizeDepth(depth);
	return (1 / (1 + exp(-steepness * (zVal - offset))));
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


    float depth = logisticDepth(gl_FragCoord.z);
    color = result * (1.0f - depth) + vec4(depth * FogColor, 1.0f);

    

    // color = vec4(vec3(gl_FragCoord.z),1.0);

}