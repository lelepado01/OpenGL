#version 330 core

struct Material {
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 objectNormal;
in vec3 fragPos;
in vec3 cameraPos;

in Material material;
in Light light;

out vec4 color;

void main() {

    vec3 norm = normalize(objectNormal);
    
    vec3 lightDir = normalize(light.position - fragPos);
    
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 ambient = light.ambient * material.ambient;
    
    vec3 result = (ambient + diffuse + specular) * material.color;
    color = vec4(result, 1.0);
}
