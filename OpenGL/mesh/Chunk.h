//
//  Chunk.h
//  OpenGL
//
//  Created by Gabriele Padovani on 10/11/21.
//

#ifndef Chunk_h
#define Chunk_h

#include <stdio.h>
#include <iostream>
#include <vector>

#include "Vertex.h"
#include "MeshHeight.h"

#include <glm/gtx/string_cast.hpp>

class Chunk {
private:
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
        
public:
    static const int VerticesPerSide = 4;
    static const int DistanceBetweenVertices = 5;
    static const int Size = VerticesPerSide * DistanceBetweenVertices;

private:
    glm::vec3 computeVertexNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    bool indexInChunk(int i) { return i < vertices.size(); } ;
    void calculateNormals(); 

public:
    Chunk(int offsetX, int offSetZ, MeshHeight meshHeight, int LOD);
    ~Chunk();
    
    std::vector<Vertex>* GetVertices() { return &vertices; };
    std::vector<unsigned int>* GetIndices() { return &indices; };

};

#endif /* Chunk_h */
