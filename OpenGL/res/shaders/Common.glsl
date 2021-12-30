
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
