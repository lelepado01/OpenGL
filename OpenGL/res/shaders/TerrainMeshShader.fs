#version 410 core

#include "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/utils/Common.glsl"

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

#include "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/utils/MathUtils.glsl"
#include "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/utils/NoiseUtils.glsl"
#include "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/utils/TerrainUtils.glsl"

vec3 getFaceNormal(vec3 position) {
    vec3 dx = vec3(dFdx(position.x), dFdx(position.y), dFdx(position.z));
    vec3 dy = vec3(dFdy(position.x), dFdy(position.y), dFdy(position.z));
    return normalize(cross(dy, dx));
}

void main() {
    vec3 norm = mix(getFaceNormal(fragPos), normalize(-objectNormal), 0.3f);

    vec3 lightDir = normalize(-u_Light.direction.xyz);

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.shininess);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 specular = u_Light.specular.xyz * (spec * material.specular);
    vec3 ambient = u_Light.ambient.xyz * material.ambient;
    vec3 diffuse = u_Light.diffuse.xyz * (diff * material.diffuse);

    vec3 result = (ambient + diffuse + specular) * getTerrainColorFromHeight(fragPos);
    fragColor = vec4(result, 1.0);

}
