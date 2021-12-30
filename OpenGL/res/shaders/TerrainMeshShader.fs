#version 410 core

#include "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/Common.glsl"
#include "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/NoiseUtils.glsl"

layout (std140) uniform SharedResources {
    mat4 u_MVP;
    mat4 u_View;
    mat4 u_Projection;
    Light u_Light;
    float u_Time;
};

in vec3 objectNormal;
in vec3 fragPos;
in vec3 cameraPos;

in Material material;

out vec4 fragColor;


// Shore Foam Effect
const float MinFoamSize = 0.03f;
const float MaxFoamSize = 0.06f;
const vec3 FoamColor = vec3(0.5,0.8,1);

// Terrain Colors
const vec3 DeepWaterColor = vec3(0.3,0.3,1);
const vec3 SandColor = vec3(0.9,0.9,0.1);
const vec3 LightGrassColor = vec3(0.1,0.9,0.0);
const vec3 DarkGrassColor = vec3(0.0,0.7,0.0);
const vec3 RockColor = vec3(0.5,0.5,0.5);
const vec3 SnowColor = vec3(1,1,1);

// Terrain Levels
const float DeepWaterLevel = 4097.3;
const float SandLevel = 4097.45;
const float LightGrassLevel = 4098;
const float DarkGrassLevel = 4099;
const float RockLevel = 4102;
const float SnowLevel = 5000;


vec3 getColorFromHeight(vec3 vertexPos){
    float height = distance(fragPos, vec3(0,0,0));
    
    float foam = simplex_noise(vertexPos * (u_Time/ 20000)) * (MaxFoamSize - MinFoamSize) + MinFoamSize;
    
    if (height < DeepWaterLevel - foam) {
        return DeepWaterColor;
    } else if (height < DeepWaterLevel) {
        return FoamColor;
    } else if (height < SandLevel ) {
        return SandColor;
    } else if (height < LightGrassLevel ) {
        return LightGrassColor;
    }  else if (height < DarkGrassLevel ) {
        return DarkGrassColor;
    }  else if (height < RockLevel ) {
        return RockColor;
    }
        
    return SnowColor;
}


vec3 getFaceNormal(vec3 position) {
    vec3 dx = vec3(dFdx(position.x), dFdx(position.y), dFdx(position.z));
    vec3 dy = vec3(dFdy(position.x), dFdy(position.y), dFdy(position.z));
    return normalize(cross(dy, dx));
}

void main() {
    vec3 norm = mix(getFaceNormal(fragPos), normalize(-objectNormal), 0.5f);

    vec3 lightDir = normalize(-u_Light.direction.xyz);

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.shininess);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 specular = u_Light.specular.xyz * (spec * material.specular);
    vec3 ambient = u_Light.ambient.xyz * material.ambient;
    vec3 diffuse = u_Light.diffuse.xyz * (diff * material.diffuse);

    vec3 result = (ambient + diffuse + specular) * getColorFromHeight(fragPos);
    fragColor = vec4(result, 1.0);

}
