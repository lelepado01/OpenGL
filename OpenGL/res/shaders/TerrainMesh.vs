#version 410 core

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 u_MVP;
uniform vec3 u_cameraPos;

uniform Material u_Material;
uniform Light u_Light;

uniform float u_Time;

out vec3 objectNormal;
out vec3 fragPos;
out vec3 cameraPos;
 
out Material material;
out Light light;

const float pi = 3.14159;
int numWaves = 2;

float waveHeight(vec3 pos) {
    float frequency = 2*pi/0.9;
    float phase = 2 * frequency;
    float theta = dot(vec2(1,1), vec2(pos.x, pos.y));
    return sin(theta * frequency + u_Time * phase);
}

void main() {
    material = u_Material;
    objectNormal = normal;
    fragPos = position;
    cameraPos = u_cameraPos;
    light = u_Light;
 
    gl_Position = u_MVP * vec4(position, 1);
    
    if (distance(fragPos, vec3(0,0,0)) < 513){
        vec3 wave = vec3(1,1,1) * waveHeight(position);
        gl_Position = u_MVP * vec4(position + wave, 1);
    }
}
