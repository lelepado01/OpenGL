#version 330 core

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

in vec3 objectNormal;
in vec3 fragPos;
in vec3 cameraPos;

in Material material;
in Light light;

out vec4 color;

vec3 getColorFromHeight(float height){
    if (height < 0.5) {
        return vec3(0,0,1);
    } else if (height < 2 ) {
        return vec3(1,1,0);
    }  else if (height < 6 ) {
        return vec3(0,1,0);
    }  else if (height < 8 ) {
        return vec3(0.5,0.5,0);
    } else {
        return vec3(1,1,1);
    }
}

void main() {

    vec3 norm = normalize(objectNormal);
    
    vec3 lightDir = normalize(-light.direction);
    
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 ambient = light.ambient * material.ambient;
    
    vec3 result = (ambient + diffuse + specular) * getColorFromHeight(fragPos.y);
    color = vec4(result, 1.0);
}
