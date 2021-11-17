#version 410 core

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 u_MVP;
uniform vec3 u_cameraPos;

uniform Material u_Material;
uniform Light u_Light;

out VS_OUT {
    vec3 objectNormal;
    vec3 fragPos;
    vec3 cameraPos;

    Material material;
    Light light;
    mat4 modelViewProjMatrix;
} vs_out;

void main() {
    vs_out.material = u_Material;
    vs_out.objectNormal = normal;
    vs_out.fragPos = position;
    vs_out.cameraPos = u_cameraPos;
    vs_out.light = u_Light;
    
    vs_out.modelViewProjMatrix = u_MVP;
 
    gl_Position = vec4(position, 1);
}
