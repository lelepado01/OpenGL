#version 410 core

in vec3 vertexColor;

out vec4 fragColor;

void main(){
    fragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b,1); 
}
