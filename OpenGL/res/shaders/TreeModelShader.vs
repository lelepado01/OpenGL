#version 410 core

#include "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/Common.glsl"

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 oldPosition;
layout (location = 3) in vec3 color;

layout (location = 4) in mat4 instanceModelMatrix;

layout (std140) uniform SharedResources {
    mat4 u_MVP;
    mat4 u_View;
    mat4 u_Projection;
    Light u_Light;
    float u_Time;
};

out vec3 vertexColor;

void main() {
    vertexColor = color;
        
    gl_Position = u_Projection * u_View * instanceModelMatrix * vec4(position, 1);
}
