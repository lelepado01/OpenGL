
struct Material {
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


struct Light {
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

const float M_PI = 3.14159265359; 

float magnitude(vec3 v){
    return sqrt( pow(v.x,2) + pow(v.y,2) + pow(v.z,2));
}
