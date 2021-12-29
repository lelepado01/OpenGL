#version 410 core

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 oldPosition;
layout (location = 3) in vec3 color;

//layout (location = 4) in vec3 instanceOffset;
//layout (location = 5) in float instanceSize;
layout (location = 4) in mat4 instanceModelMatrix;

layout (std140) uniform CameraMatrices {
    mat4 u_MVP;
    mat4 u_View;
    mat4 u_Projection;
};

out vec3 vertexColor;

void main() {
    vertexColor = color;
        
    gl_Position = u_Projection * u_View * instanceModelMatrix * vec4(position, 1);
}
