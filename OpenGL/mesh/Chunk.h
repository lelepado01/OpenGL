//
//  Chunk.h
//  OpenGL
//
//  Created by Gabriele Padovani on 10/11/21.
//

#ifndef Chunk_h
#define Chunk_h

#include <stdio.h>
#include <vector>

#include "../vendor/fast_noise/FastNoiseLite.h"

#include "Vertex.h"

class Chunk {
private:
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();

public:
    static const int ChunkSize = 10;

    
public:
    Chunk(int offsetX, int offSetZ, int LOD);
    ~Chunk();
    
    std::vector<Vertex>* GetVertices() { return &vertices; };
    std::vector<unsigned int>* GetIndices() { return &indices; };

};

#endif /* Chunk_h */
