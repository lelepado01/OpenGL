#version 410 core

#include "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/utils/Common.glsl"

in vec3 vertexColor;

out vec4 fragColor;

void main(){
    fragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b,1); 
}
