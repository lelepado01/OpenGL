#version 410 core

#include "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/utils/Common.glsl"
#include "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/utils/MathUtils.glsl"

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 deltaPosition;

layout (std140) uniform SharedResources {
    mat4 u_MVP;
    mat4 u_View;
    mat4 u_Projection;
    Light u_Light;
    float u_Time;
};

uniform vec3 u_cameraPos;
uniform Material u_Material;

out vec3 objectNormal;
out vec3 fragPos;
out vec3 cameraPos;
out Material material;

// Wave Motion Parameters

// Wave Motion Functions
float waveHeight(vec3 pos) {
    float frequency = 2*M_PI/0.9;
    float phase = 0.2 * frequency;
    float theta = dot(vec2(1,1), vec2(pos.x, pos.y));
    return sin(theta * frequency + u_Time * phase) * 0.1;
}

// Terrain Geomorphing Parameters
uniform float u_TerrainAnimationPercentage;

// Terrain Geomorphing Functions
vec3 getCurrentPosition(vec3 newPos, vec3 deltaOldNewPos, float perc){
    return newPos - deltaOldNewPos * (1-perc);
}

void main() {
    material = u_Material;
    objectNormal = normal;
    fragPos = position;
    cameraPos = u_cameraPos;
    
    gl_Position = u_MVP * vec4(position, 1);

    if (u_TerrainAnimationPercentage != -1){
        vec3 currentPosition = getCurrentPosition(position, deltaPosition, u_TerrainAnimationPercentage);
        gl_Position = u_MVP * vec4(currentPosition, 1);
        fragPos = currentPosition;
    }
}
