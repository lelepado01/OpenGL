#version 410 core

in vec3 objectNormal;
in vec3 fragPos;
in vec3 cameraPos;

in Material material;
in Light light;

out vec4 color;

vec3 getColorFromHeight(float height){
    if (height < 512) {
        return vec3(0,0,1);
    } else if (height < 515 ) {
        return vec3(0.3,0.3,1);
    } else if (height < 520 ) {
        return vec3(1,1,0.1);
    }  else if (height < 525 ) {
        return vec3(0,1,0);
    }  else if (height < 535 ) {
        return vec3(0.1,0.7,0.1);
    }  else if (height < 540 ) {
        return vec3(0.5,0.5,0.5);
    }
        
    return vec3(1,1,1);
}


vec3 getFaceNormal(vec3 position) {
    vec3 dx = vec3(dFdx(position.x), dFdx(position.y), dFdx(position.z));
    vec3 dy = vec3(dFdy(position.x), dFdy(position.y), dFdy(position.z));
    return normalize(cross(dy, dx));
}


void main() {

    vec3 norm = mix(getFaceNormal(fragPos), normalize(-objectNormal), 0.5f);
    
    vec3 lightDir = normalize(-light.direction);
    
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.shininess);

    float diff = max(dot(norm, lightDir), 0.0);
    
    vec3 specular = light.specular * (spec * material.specular);
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    vec3 result = (ambient + diffuse + specular) * getColorFromHeight(distance(fragPos, vec3(0,0,0)));
    color = vec4(result, 1.0);
}
