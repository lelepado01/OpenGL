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
#include "../settings/ChunkSettings.h"

#include <glm/gtx/string_cast.hpp>

class Chunk {
private:
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    
    int globalOffsetX;
    int globalOffsetY; 

private:
    glm::vec3 computeVertexNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    bool indexInChunk(int i) { return i < vertices.size(); } ;
    void calculateNormals(); 

public:
    Chunk(int offsetX, int offSetZ, MeshHeight meshHeight, int LOD);
    Chunk(int offsetX, int offSetZ, int LOD);
    ~Chunk();
    
    std::vector<Vertex>* GetVertices() { return &vertices; };
    std::vector<unsigned int>* GetIndices() { return &indices; };
    bool HasPosition(int x, int y) { return x == globalOffsetX && y == globalOffsetY; };
    glm::vec2 GetPosition() { return glm::vec2(globalOffsetX, globalOffsetY); }; 

    static float GetChunkCenterFromIndex(int i){ return i * ChunkSettings::Size + ChunkSettings::Size/2; };
    static int GetChunkIndexFromPosition(float pos){ return floor(pos / ChunkSettings::Size); };
    static float GetMaxHeight(int offsetX, int offSetZ, MeshHeight meshHeight);
};

#endif /* Chunk_h */
