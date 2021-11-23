#version 410 core

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 u_MVP;
uniform vec3 u_cameraPos;

uniform Material u_Material;
uniform Light u_Light;

//out VS_OUT {
//    vec3 objectNormal;
//    vec3 fragPos;
//    vec3 cameraPos;
//
//    Material material;
//    Light light;
//    mat4 modelViewProjMatrix;
//} vs_out;

out vec3 objectNormal;
out vec3 fragPos;
out vec3 cameraPos;
 
out Material material;
out Light light;

void main() {
    material = u_Material;
    objectNormal = normal;
    fragPos = position;
    cameraPos = u_cameraPos;
    light = u_Light;
 
    gl_Position = u_MVP * vec4(position, 1);
}
