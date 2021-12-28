#version 410 core

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 oldPosition;
layout (location = 3) in vec3 color;

layout (location = 4) in vec3 instanceOffset;
layout (location = 5) in float instanceSize;

layout (std140) uniform Matrices {
    mat4 u_MVP;
};

out vec3 vertexColor;

void main() {
    vertexColor = color;
    
    vec3 offset = instanceOffset;
    gl_Position = u_MVP * vec4(offset + position * instanceSize, 1); 
}
