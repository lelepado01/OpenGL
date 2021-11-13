#version 410 core

struct Material {
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    //vec3 position;
    vec3 direction; 
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 u_MVP;
uniform vec3 u_cameraPos;

uniform Material u_Material;
uniform Light u_Light;

out vec3 objectNormal;
out vec3 fragPos;
out vec3 cameraPos;

out Material material;
out Light light;

void main() {
    light = u_Light;
    material = u_Material;
    
    objectNormal = normal;
    fragPos = position;
    cameraPos = u_cameraPos; 
 
    gl_Position = u_MVP * vec4(position, 1);
}
