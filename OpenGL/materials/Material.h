//
//  Material.h
//  OpenGL
//
//  Created by Gabriele Padovani on 06/11/21.
//

#ifndef Material_h
#define Material_h


struct Material {
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Light {
    glm::vec4 direction;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
};

#endif /* Material_h */
