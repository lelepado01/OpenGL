//
//  Chunk.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 10/11/21.
//

#include "Chunk.h"

Chunk::Chunk(int offsetX, int offSetZ, int LOD){
    globalOffsetX = offsetX;
    globalOffsetY = offSetZ;
    vertices = std::vector<Vertex>();
    indices = std::vector<unsigned int>();
        
    for (float z = 0; z <= VerticesPerSide * LOD; z++) {
        for (float x = 0; x <= VerticesPerSide * LOD; x++) {
            
            float globalX = x * DistanceBetweenVertices / LOD + offsetX * Chunk::Size;
            float globalZ = z * DistanceBetweenVertices / LOD + offSetZ * Chunk::Size;
            
            Vertex v = {};
            v.position = glm::vec3(globalX, 0, globalZ);
            
            vertices.push_back(v);
        }
    }

    int vertexIndex = 0;
    for (int z = 0; z <= VerticesPerSide * LOD; z++) {
        for (int x = 0; x <= VerticesPerSide * LOD; x++) {
            if (x < VerticesPerSide * LOD && z < VerticesPerSide * LOD){
                indices.push_back(vertexIndex);
                indices.push_back(vertexIndex + VerticesPerSide * LOD + 1);
                indices.push_back(vertexIndex + 1);
                
                indices.push_back(vertexIndex + 1);
                indices.push_back(vertexIndex + VerticesPerSide * LOD + 1);
                indices.push_back(vertexIndex + VerticesPerSide * LOD + 2);

                vertexIndex++;
            }
        }
        vertexIndex++;
    }

}


Chunk::Chunk(int offsetX, int offSetZ, MeshHeight meshHeight, int LOD){
    globalOffsetX = offsetX;
    globalOffsetY = offSetZ; 
    vertices = std::vector<Vertex>();
    indices = std::vector<unsigned int>();
        
    for (float z = 0; z <= VerticesPerSide * LOD; z++) {
        for (float x = 0; x <= VerticesPerSide * LOD; x++) {
            
            float globalX = x * DistanceBetweenVertices / LOD + offsetX * Chunk::Size;
            float globalZ = z * DistanceBetweenVertices / LOD + offSetZ * Chunk::Size;
            
            Vertex v = {};
            v.position = glm::vec3(globalX,
                                   meshHeight.GetHeight(globalX ,globalZ),
                                   globalZ);
            
            vertices.push_back(v);
        }
    }

    int vertexIndex = 0;
    for (int z = 0; z <= VerticesPerSide * LOD; z++) {
        for (int x = 0; x <= VerticesPerSide * LOD; x++) {
            if (x < VerticesPerSide * LOD && z < VerticesPerSide * LOD){
                indices.push_back(vertexIndex);
                indices.push_back(vertexIndex + VerticesPerSide * LOD + 1);
                indices.push_back(vertexIndex + 1);
                
                indices.push_back(vertexIndex + 1);
                indices.push_back(vertexIndex + VerticesPerSide * LOD + 1);
                indices.push_back(vertexIndex + VerticesPerSide * LOD + 2);

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

float Chunk::GetMaxHeight(int offsetX, int offSetZ, MeshHeight meshHeight){
    float maxHeight = 0;
    
    for (float z = 0; z <= VerticesPerSide; z++) {
        for (float x = 0; x <= VerticesPerSide; x++) {
            
            float globalX = x * DistanceBetweenVertices + offsetX * Chunk::Size;
            float globalZ = z * DistanceBetweenVertices + offSetZ * Chunk::Size;
            
            float height = meshHeight.GetHeight(globalX ,globalZ);
            
            if (height > maxHeight) maxHeight = height;
        }
    }
    
    return maxHeight; 
}
