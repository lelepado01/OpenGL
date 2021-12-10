//
//  Vertex.h
//  OpenGL
//
//  Created by Gabriele Padovani on 06/11/21.
//

#ifndef Vertex_h
#define Vertex_h

#include "glm/glm.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 oldPosition;
};

struct ModelVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 oldPosition;
    glm::vec3 color;
};

struct TerrainVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 oldPosition;
};

#endif /* Vertex_h */
