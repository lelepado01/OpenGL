#version 410 core

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform int u_ChunkSize;
uniform int u_ChunkNumber; 

uniform mat4 u_MVP;
uniform vec3 u_cameraPos;

uniform Material u_Material;
uniform Light u_Light;

out VS_OUT {
    int chunkSize;
    int chunkNumber;
    
    vec3 objectNormal;
    vec3 fragPos;
    vec3 cameraPos;

    Material material;
    Light light;
    
    mat4 modelViewProjMatrix;
} vs_out;

void main() {
    vs_out.chunkSize = u_ChunkSize;
    vs_out.chunkNumber = u_ChunkNumber;

    vs_out.material = u_Material;
    vs_out.objectNormal = normal;
    vs_out.fragPos = position;
    vs_out.cameraPos = u_cameraPos;
    vs_out.light = u_Light;
    
    vs_out.modelViewProjMatrix = u_MVP;
 
    gl_Position = vec4(position, 1);
}
