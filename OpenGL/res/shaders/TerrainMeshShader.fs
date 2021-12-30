#version 410 core

layout (std140) uniform SharedResources {
    mat4 u_MVP;
    mat4 u_View;
    mat4 u_Projection;
    Light u_Light;
    float u_Time;
};

in vec3 objectNormal;
in vec3 fragPos;
in vec3 cameraPos;

in Material material;

out vec4 fragColor;


vec3 getColorFromHeight(float height){
    if (height < 4097.3) {
        return vec3(0.3,0.3,1);
    } else if (height < 4097.7 ) {
        return vec3(1,1,0);
    } else if (height < 4098 ) {
        return vec3(0,1,0);
    }  else if (height < 4099 ) {
        return vec3(0.1,0.7,0.1);
    }  else if (height < 4102 ) {
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

    vec3 lightDir = normalize(-u_Light.direction.xyz);

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.shininess);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 specular = u_Light.specular.xyz * (spec * material.specular);
    vec3 ambient = u_Light.ambient.xyz * material.ambient;
    vec3 diffuse = u_Light.diffuse.xyz * (diff * material.diffuse);

    vec3 result = (ambient + diffuse + specular) * getColorFromHeight(distance(fragPos, vec3(0,0,0)));
    fragColor = vec4(result, 1.0);

}
