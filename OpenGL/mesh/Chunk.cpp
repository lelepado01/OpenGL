//
//  Chunk.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 10/11/21.
//

#include "Chunk.h"

Chunk::Chunk(int offsetX, int offSetZ){
    globalOffsetX = offsetX;
    globalOffsetY = offSetZ;
    vertices = std::vector<Vertex>();
    indices = std::vector<unsigned int>();
        
    for (float z = 0; z <= ChunkSettings::VerticesPerSide; z++) {
        for (float x = 0; x <= ChunkSettings::VerticesPerSide; x++) {
            
            float globalX = x * ChunkSettings::DistanceBetweenVertices + offsetX * ChunkSettings::Size;
            float globalZ = z * ChunkSettings::DistanceBetweenVertices + offSetZ * ChunkSettings::Size;
            
            Vertex v = {};
            v.position = glm::vec3(globalX, 0, globalZ);
            
            vertices.push_back(v);
        }
    }

    int vertexIndex = 0;
    for (int z = 0; z <= ChunkSettings::VerticesPerSide; z++) {
        for (int x = 0; x <= ChunkSettings::VerticesPerSide; x++) {
            if (x < ChunkSettings::VerticesPerSide && z < ChunkSettings::VerticesPerSide){
                indices.push_back(vertexIndex);
                indices.push_back(vertexIndex + ChunkSettings::VerticesPerSide + 1);
                indices.push_back(vertexIndex + 1);
                
                indices.push_back(vertexIndex + 1);
                indices.push_back(vertexIndex + ChunkSettings::VerticesPerSide + 1);
                indices.push_back(vertexIndex + ChunkSettings::VerticesPerSide + 2);

                vertexIndex++;
            }
        }
        vertexIndex++;
    }

}


Chunk::Chunk(int offsetX, int offSetZ, int LOD){
    globalOffsetX = offsetX;
    globalOffsetY = offSetZ; 
    vertices = std::vector<Vertex>();
    indices = std::vector<unsigned int>();
        
    for (float z = 0; z <= ChunkSettings::VerticesPerSide * LOD; z++) {
        for (float x = 0; x <= ChunkSettings::VerticesPerSide * LOD; x++) {
            
            float globalX = x * ChunkSettings::DistanceBetweenVertices / LOD + offsetX * ChunkSettings::Size;
            float globalZ = z * ChunkSettings::DistanceBetweenVertices / LOD + offSetZ * ChunkSettings::Size;
            
            Vertex v = {};
            v.position = glm::vec3(globalX,
                                   MeshHeight::GetHeight(globalX ,globalZ),
                                   globalZ);
            
            vertices.push_back(v);
        }
    }

    int vertexIndex = 0;
    for (int z = 0; z <= ChunkSettings::VerticesPerSide * LOD; z++) {
        for (int x = 0; x <= ChunkSettings::VerticesPerSide * LOD; x++) {
            if (x < ChunkSettings::VerticesPerSide * LOD && z < ChunkSettings::VerticesPerSide * LOD){
                indices.push_back(vertexIndex);
                indices.push_back(vertexIndex + ChunkSettings::VerticesPerSide * LOD + 1);
                indices.push_back(vertexIndex + 1);
                
                indices.push_back(vertexIndex + 1);
                indices.push_back(vertexIndex + ChunkSettings::VerticesPerSide * LOD + 1);
                indices.push_back(vertexIndex + ChunkSettings::VerticesPerSide * LOD + 2);

                vertexIndex++;
            }
        }
        vertexIndex++;
    }

    calculateNormals();
}

void Chunk::calculateNormals(){
    
    for (int normalTriangleIndex = 0; normalTriangleIndex < indices.size(); normalTriangleIndex+=3) {
        int vertexIndexA = indices.at(normalTriangleIndex);
        int vertexIndexB = indices.at(normalTriangleIndex + 1);
        int vertexIndexC = indices.at(normalTriangleIndex + 2);

        glm::vec3 normal = computeVertexNormal(vertices.at(vertexIndexA).position,
                                               vertices.at(vertexIndexB).position,
                                               vertices.at(vertexIndexC).position);
        
        vertices[vertexIndexA].normal += normal;
        vertices[vertexIndexB].normal += normal;
        vertices[vertexIndexC].normal += normal;
    }
    
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].normal = glm::normalize(vertices[i].normal);
    }
}

Chunk::~Chunk(){}; 

glm::vec3 Chunk::computeVertexNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c){
    return glm::cross(b-a, c-a);
}

float Chunk::GetMaxHeight(int offsetX, int offSetZ){
    float maxHeight = 0;
    
    for (float z = 0; z <= ChunkSettings::VerticesPerSide; z++) {
        for (float x = 0; x <= ChunkSettings::VerticesPerSide; x++) {
            
            float globalX = x * ChunkSettings::DistanceBetweenVertices + offsetX * ChunkSettings::Size;
            float globalZ = z * ChunkSettings::DistanceBetweenVertices + offSetZ * ChunkSettings::Size;
            
            float height = MeshHeight::GetHeight(globalX ,globalZ);
            
            if (height > maxHeight) maxHeight = height;
        }
    }
    
    return maxHeight; 
}
