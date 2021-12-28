#version 410 core

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 oldPosition;
layout (location = 3) in vec3 color;

uniform mat4 u_MVP;
uniform vec3 instancePositions[1000];

out vec3 vertexColor;

void main() {
    vertexColor = color;
    
    vec3 pos = instancePositions[gl_InstanceID];
    gl_Position = u_MVP * vec4(pos + position*0.5f , 1);
}
