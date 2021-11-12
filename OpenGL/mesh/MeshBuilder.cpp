//
//  MeshBuilder.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 09/11/21.
//

#include "MeshBuilder.h"

MeshBuilder::MeshBuilder(Camera camera){
    
    distanceLOD[100] = 2;
    distanceLOD[40] = 4;
    
    createGridMesh(camera.GetPosition());
    UpdateMesh(camera);
}


MeshBuilder::~MeshBuilder(){
    
}

void MeshBuilder::UpdateMesh(Camera camera){
    chunks = std::vector<Chunk>();
    
    glm::vec3 cameraChunkPosition = getCameraChunkPosition(camera.GetPosition());

    for (int x = -chunkNumber; x < chunkNumber; x++) {
        for (int z = -chunkNumber; z < chunkNumber; z++) {
            
            int xglobal = x + cameraChunkPosition.x;
            int zglobal = z + cameraChunkPosition.z;
            int chunkCenterX = xglobal * Chunk::Size + Chunk::Size / 2;
            int chunkCenterZ = zglobal * Chunk::Size + Chunk::Size / 2;
            
            if (camera.PointIsVisibleFromCamera(chunkCenterX, chunkCenterZ)){
                Chunk c(xglobal, zglobal, getChunkLOD(camera.GetPosition(), xglobal, zglobal));
                chunks.push_back(c);
            }
        }
    }
}


void MeshBuilder::createGridMesh(glm::vec3 cameraPosition){
    chunks = std::vector<Chunk>();

    int start = floor(float(chunkNumber) / 2) - chunkNumber;
    int end = floor(float(chunkNumber) / 2) +1;
    for (int x = start; x < end; x++) {
        for (int y = start; y < end; y++) {
            Chunk c(x, y, getChunkLOD(cameraPosition, x, y));
            chunks.push_back(c);
        }
    }
}

float MeshBuilder::getGlobalOffset(glm::vec3 cameraPosition){
    float R2 = 10000;
    glm::vec3 origin = glm::vec3(0,0,0);
    float d2 = glm::distance(cameraPosition, origin) * glm::distance(cameraPosition, origin);

    float HplusS = sqrt(d2 - sphereRadius*sphereRadius) + sqrt(R2 - sphereRadius*sphereRadius);

    return (R2 + d2 - HplusS*HplusS) / (2*sphereRadius * HplusS);
}

void MeshBuilder::gridMeshToSphere(glm::vec3 cameraPosition){
//    vertices = std::vector<Vertex>();
//    vertices = gridVertices;
//
//    float zs = getGlobalOffset(cameraPosition);
//
//    glm::vec3 W = glm::normalize(cameraPosition);
//    glm::vec3 b = glm::vec3(0,0,1);
//    glm::vec3 v = glm::normalize(W * b);
//    glm::vec3 u = W * v;
//
//    glm::mat3x3 R = glm::transpose(glm::mat3x3(u, v, W));
//
//    for (int i = 0; i < vertices.size(); i++) {
//        vertices[i].position.y = (1 - glm::pow(vertices[i].position.x, 2)) * (1 - glm::pow(vertices[i].position.z, 2));
//
//        vertices[i].position = glm::normalize(vertices[i].position);
//        vertices[i].position *= sphereRadius;
//
//        vertices[i].position.y += zs;
//
//        vertices[i].position = R * vertices[i].position;
//    }

}

std::vector<Vertex>* MeshBuilder::GetVertices() {
    vertices = std::vector<Vertex>();

    for (int i=0; i < chunks.size(); i++) {
        
        std::vector<Vertex>* vert = chunks[i].GetVertices();
    
        for (int j = 0; j < vert->size(); j++){
            vertices.push_back(vert[0][j]);
        }
    }
    
    return &vertices;
}

std::vector<unsigned int>* MeshBuilder::GetIndices(){
    indices = std::vector<unsigned int>();

    int chunkOffset = 0;
    for (int i=0; i < chunks.size(); i++) {
        
        std::vector<unsigned int>* ind = chunks[i].GetIndices();
        for (int j = 0; j < ind->size(); j++){
            indices.push_back(ind[0][j] + chunkOffset);
        }
        
        chunkOffset += chunks[i].GetVertices()->size();
    }
    
    return &indices;
}


int MeshBuilder::getChunkLOD(glm::vec3 cameraPosition, int offX, int offZ){
    glm::vec3 chunkPosition = glm::vec3(offX * Chunk::Size + Chunk::Size/2,
                                        0.0f,
                                        offZ * Chunk::Size + Chunk::Size/2);

    return getLODFromDistance(glm::distance(chunkPosition, cameraPosition));
}

int MeshBuilder::getLODFromDistance(int distance){
    for (auto detailLevel : distanceLOD) {
        if ( distance < detailLevel.first){
            return detailLevel.second;
        }
    }
    
    return 1;
}

glm::vec3 MeshBuilder::getCameraChunkPosition(glm::vec3 cameraPosition){
    return glm::vec3(floor(cameraPosition.x / Chunk::Size),
                     0,
                     floor(cameraPosition.z / Chunk::Size));
}

glm::vec3 MeshBuilder::getCameraLookDirection(glm::vec3 cameraDirection){
    glm::vec3 dir = glm::vec3();
    if (cameraDirection.x >= 0) dir.x = 1;
    if (cameraDirection.x < 0) dir.x = -1;
    if (cameraDirection.z >= 0) dir.z = 1;
    if (cameraDirection.z < 0) dir.z = -1;
    return dir; 
}
