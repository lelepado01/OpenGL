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
    
    unsigned int GetSize() { return sizeof(Vertex); };
};

struct ModelVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 oldPosition;
    glm::vec3 color;
    
    unsigned int GetSize() { return sizeof(ModelVertex); };
};

struct TerrainVertex : Vertex {
    unsigned int GetSize() { return sizeof(TerrainVertex); };
};

#endif /* Vertex_h */
