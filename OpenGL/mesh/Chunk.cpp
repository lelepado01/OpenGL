//
//  Chunk.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 10/11/21.
//

#include "Chunk.h"

Chunk::Chunk(int offsetX, int offSetZ, int LOD){
    vertices = std::vector<Vertex>();
    indices = std::vector<unsigned int>();
    
    FastNoiseLite noise = FastNoiseLite();
    noise.SetFrequency(0.6f);
    
    for (float z = 0; z <= ChunkSize * LOD; z++) {
        for (float x = 0; x <= ChunkSize * LOD; x++) {
            Vertex v = {};
//            v.position = glm::vec3(x / (chunkSize/2) - 1, 0.0f, z / (chunkSize/2) - 1);
            
            float globalX = x / LOD + offsetX * ChunkSize;
            float globalZ = z / LOD + offSetZ * ChunkSize;
            
            v.position = glm::vec3(globalX, noise.GetNoise((globalX) * 0.2f, (globalZ) * 0.2f), globalZ);
            v.normal = glm::vec3(1.f, 1.f, 1.f);
            
            vertices.push_back(v);
        }
    }

    int vertexIndex = 0;
    for (int z = 0; z < ChunkSize * LOD; z++) {
        for (int x = 0; x < ChunkSize * LOD; x++) {
            indices.push_back(vertexIndex);
            indices.push_back(vertexIndex + ChunkSize*LOD + 1);
            indices.push_back(vertexIndex + 1);
            indices.push_back(vertexIndex + 1);
            indices.push_back(vertexIndex + ChunkSize*LOD + 1);
            indices.push_back(vertexIndex + ChunkSize*LOD + 2);
            
            vertexIndex++;
        }
        vertexIndex++;
    }
}

Chunk::~Chunk(){}; 