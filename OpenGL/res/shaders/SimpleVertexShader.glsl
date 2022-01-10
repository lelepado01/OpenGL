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

void main() {
    gl_Position = u_MVP * vec4(position, 1);
}
