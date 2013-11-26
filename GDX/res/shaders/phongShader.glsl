#version 330

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

varying vec2 texCoord0;
varying vec3 normal0;

uniform mat4 MVP;
uniform mat4 transform;
uniform sampler2D diffuse;

struct BaseLight
{
    vec3 color;
    float intensity;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

vec4 CalcLight(BaseLight base, vec3 direction, vec3 normal)
{
    float diffuseFactor = dot(normal, -direction);
    
    vec4 diffuseColor = vec4(0,0,0,0);
    //vec4 specularColor = vec4(0,0,0,0);
    
    if(diffuseFactor > 0)
    {
        diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
        
        //vec3 directionToEye = normalize(eyePos - worldPos0);
        //vec3 reflectDirection = normalize(reflect(direction, normal));
        
        //float specularFactor = dot(directionToEye, reflectDirection);
        //specularFactor = pow(specularFactor, specularPower);
        
        //if(specularFactor > 0)
        //{
        //    specularColor = vec4(base.color, 1.0) * specularIntensity * specularFactor;
        //}
    }
    
    return diffuseColor;// + specularColor;
}

vec4 CalcDirectionalLight(DirectionalLight directionalLight, vec3 normal)
{
    return CalcLight(directionalLight.base, -directionalLight.direction, normal);
}

void VSmain()
{
	gl_Position = MVP * vec4(position, 1.0);
	texCoord0 = texCoord;
	normal0 = (transform * vec4(normal, 0.0)).xyz;
}

void FSmain()
{
	vec4 color = texture(diffuse, texCoord0.xy);
	vec3 normal = normalize(normal0);
	
	DirectionalLight light;
	light.base.color = vec3(1,1,1);
	light.base.intensity = 0.8;
	light.direction = normalize(vec3(1,1,1));
	
	vec4 totalLight = vec4(0.1, 0.1, 0.1, 1.0);
	totalLight += CalcDirectionalLight(light, normal);

	gl_FragColor = color * totalLight; 
}
