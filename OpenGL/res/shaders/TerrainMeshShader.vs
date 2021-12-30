#version 410 core

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 oldPosition;

layout (std140) uniform CameraMatrices {
    mat4 u_MVP;
    mat4 u_View;
    mat4 u_Projection;
};

uniform vec3 u_cameraPos;

uniform Material u_Material;
uniform Light u_Light;


// Wave Motion Parameters
const float pi = 3.14159;

uniform float u_Time;


// Terrain Geomorphing Parameters
uniform float u_TerrainAnimationPercentage;


out vec3 objectNormal;
out vec3 fragPos;
out vec3 cameraPos;
 
out Material material;
out Light light;

// Wave Motion Functions
float waveHeight(vec3 pos) {
    float frequency = 2*pi/0.9;
    float phase = 0.2 * frequency;
    float theta = dot(vec2(1,1), vec2(pos.x, pos.y));
    return sin(theta * frequency + u_Time * phase) * 0.1;
}

// Terrain Geomorphing Functions

void main() {
    material = u_Material;
    objectNormal = normal;
    fragPos = position;
    cameraPos = u_cameraPos;
    light = u_Light;
    
    gl_Position = u_MVP * vec4(position, 1);

    if (u_TerrainAnimationPercentage != -1){
        gl_Position = u_MVP * vec4( oldPosition + (position - oldPosition) * (u_TerrainAnimationPercentage), 1);
    }
}
