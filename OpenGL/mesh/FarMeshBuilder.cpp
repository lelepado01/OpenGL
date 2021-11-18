//
//  FarMeshBuilder.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 18/11/21.
//

#include "FarMeshBuilder.h"


FarMeshBuilder::FarMeshBuilder(Camera camera){
    UpdateMesh(camera);
}


FarMeshBuilder::~FarMeshBuilder(){}

void FarMeshBuilder::UpdateMesh(Camera camera){
    glm::vec2 cameraChunkPosition = getCameraChunkPosition(camera.GetPosition());
    if ( cameraChunkPosition == lastFrameCameraChunkPosition && !camera.HasRotated()) {
        return;
    }
    lastFrameCameraChunkPosition = cameraChunkPosition;
    
    removeChunksOutOfView(camera);
    
    
    
    
}

//float FarMeshBuilder::getGlobalOffset(glm::vec3 cameraPosition){
//    float R2 = 10000;
//    glm::vec3 origin = glm::vec3(0,0,0);
//    float d2 = glm::distance(cameraPosition, origin) * glm::distance(cameraPosition, origin);
//
//    float HplusS = sqrt(d2 - MeshSettings::SphereRadius*MeshSettings::SphereRadius) + sqrt(R2 - MeshSettings::SphereRadius*MeshSettings::SphereRadius);
//
//    return (R2 + d2 - HplusS*HplusS) / (2*MeshSettings::SphereRadius * HplusS);
//}

//void FarMeshBuilder::gridMeshToSphere(glm::vec3 cameraPosition){
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
//}

std::vector<Vertex>* FarMeshBuilder::GetVertices() {
    vertices = std::vector<Vertex>();

    for (int i=0; i < chunks.size(); i++) {
        
        std::vector<Vertex>* vert = chunks[i].GetVertices();
    
        for (int j = 0; j < vert->size(); j++){
            vertices.push_back(vert[0][j]);
        }
    }
    
    return &vertices;
}

std::vector<unsigned int>* FarMeshBuilder::GetIndices(){
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

glm::vec2 FarMeshBuilder::getCameraChunkPosition(glm::vec3 cameraPosition){
    return glm::vec2(Chunk::GetChunkIndexFromPosition(cameraPosition.x), Chunk::GetChunkIndexFromPosition(cameraPosition.z));
}

bool FarMeshBuilder::cameraIsCloseToChunk(glm::vec3 cameraPosition, int chunkX, int chunkY){
    glm::vec2 camera2d = glm::vec2(cameraPosition.x, cameraPosition.z);
    glm::vec2 chunk2d = glm::vec2(chunkX, chunkY);
    
    return abs(glm::distance(camera2d, chunk2d)) < 60;
}

bool FarMeshBuilder::chunkIsVisibleFromCamera(Camera camera, int offsetX, int offSetZ){
    
    float maxHeight = Chunk::GetMaxHeight(offsetX, offSetZ);
    
    return maxHeight > -100;
    
}

bool FarMeshBuilder::positionIsInListOfChunks(int x, int y){
    for (int i = 0; i < chunks.size(); i++) {
        if (chunks.at(i).HasPosition(x, y)) return true;
    }
    
    return false;
}

bool FarMeshBuilder::chunkIsVisible(Camera camera, glm::vec2 globalPosition){
    int chunkCenterX = Chunk::GetChunkCenterFromIndex(globalPosition.x);
    int chunkCenterZ = Chunk::GetChunkCenterFromIndex(globalPosition.y);

    return  camera.PointIsVisibleFromCamera(chunkCenterX, chunkCenterZ) &&
            chunkIsVisibleFromCamera(camera, globalPosition.x, globalPosition.y) &&
            cameraIsCloseToChunk(camera.GetPosition(), chunkCenterX, chunkCenterZ);
}

void FarMeshBuilder::removeChunksOutOfView(Camera camera){
    
    int chunkIndex = 0;
    for (Chunk chunk : chunks) {
        
        glm::vec2 globalPosition = chunk.GetPosition();
    
        if (!chunkIsVisible(camera, globalPosition)){
            chunks.erase(chunks.begin() + chunkIndex);
            continue;
        }
        
        chunkIndex++;
    }
}

