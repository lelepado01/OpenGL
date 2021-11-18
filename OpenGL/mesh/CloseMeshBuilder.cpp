//
//  MeshBuilder.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 09/11/21.
//

#include "CloseMeshBuilder.h"

CloseMeshBuilder::CloseMeshBuilder(Camera camera){
    distanceLOD[30] = 2;
    distanceLOD[20] = 4;
    
    UpdateMesh(camera);
}


CloseMeshBuilder::~CloseMeshBuilder(){}

void CloseMeshBuilder::UpdateMesh(Camera camera){
    glm::vec2 cameraChunkPosition = getCameraChunkPosition(camera.GetPosition());
    if ( cameraChunkPosition == lastFrameCameraChunkPosition && !camera.HasRotated()) {
        return;
    }
    lastFrameCameraChunkPosition = cameraChunkPosition;
    
    removeChunksOutOfView(camera);
    
    // TODO: do I have to iterate over all chunks? 
    for (int x = -MeshSettings::ChunkNumber; x < MeshSettings::ChunkNumber; x++) {
        for (int z = -MeshSettings::ChunkNumber; z < MeshSettings::ChunkNumber; z++) {

            int xglobal = x + cameraChunkPosition.x;
            int zglobal = z + cameraChunkPosition.y;

            if (positionIsInListOfChunks(xglobal, zglobal)) continue;

            int chunkCenterX = Chunk::GetChunkCenterFromIndex(xglobal);
            int chunkCenterZ = Chunk::GetChunkCenterFromIndex(zglobal);

            if (camera.PointIsVisibleFromCamera(chunkCenterX, chunkCenterZ) && chunkIsVisibleFromCamera(camera, xglobal, zglobal)){
                Chunk c(xglobal, zglobal, getChunkLOD(camera.GetPosition(), xglobal, zglobal));
                chunks.push_back(c);
            } else if (cameraIsCloseToChunk(camera.GetPosition(), chunkCenterX, chunkCenterZ)){
                Chunk c(xglobal, zglobal, 1);
                chunks.push_back(c);
            }
        }
    }
    
    
}

//float CloseMeshBuilder::getGlobalOffset(glm::vec3 cameraPosition){
//    float R2 = 10000;
//    glm::vec3 origin = glm::vec3(0,0,0);
//    float d2 = glm::distance(cameraPosition, origin) * glm::distance(cameraPosition, origin);
//
//    float HplusS = sqrt(d2 - MeshSettings::SphereRadius*MeshSettings::SphereRadius) + sqrt(R2 - MeshSettings::SphereRadius*MeshSettings::SphereRadius);
//
//    return (R2 + d2 - HplusS*HplusS) / (2*MeshSettings::SphereRadius * HplusS);
//}

//void CloseMeshBuilder::gridMeshToSphere(glm::vec3 cameraPosition){
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

std::vector<Vertex>* CloseMeshBuilder::GetVertices() {
    vertices = std::vector<Vertex>();

    for (int i=0; i < chunks.size(); i++) {
        
        std::vector<Vertex>* vert = chunks[i].GetVertices();
    
        for (int j = 0; j < vert->size(); j++){
            vertices.push_back(vert[0][j]);
        }
    }
    
    return &vertices;
}

std::vector<unsigned int>* CloseMeshBuilder::GetIndices(){
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

std::vector<Vertex>* CloseMeshBuilder::GetLowLODVertices(){
    return sampleLowLODChunk.GetVertices();
}

std::vector<unsigned int>* CloseMeshBuilder::GetLowLODIndices(){
    return sampleLowLODChunk.GetIndices();
}

int CloseMeshBuilder::getChunkLOD(glm::vec3 cameraPosition, int offX, int offZ){
    glm::vec3 chunkPosition = glm::vec3(Chunk::GetChunkCenterFromIndex(offX), 0.0f, Chunk::GetChunkCenterFromIndex(offX));

    return getLODFromDistance(glm::distance(chunkPosition, cameraPosition));
}

int CloseMeshBuilder::getLODFromDistance(int distance){
    for (auto detailLevel : distanceLOD) {
        if ( distance < detailLevel.first){
            return detailLevel.second;
        }
    }
    
    return 1;
}

glm::vec2 CloseMeshBuilder::getCameraChunkPosition(glm::vec3 cameraPosition){
    return glm::vec2(Chunk::GetChunkIndexFromPosition(cameraPosition.x), Chunk::GetChunkIndexFromPosition(cameraPosition.z));
}

bool CloseMeshBuilder::cameraIsCloseToChunk(glm::vec3 cameraPosition, int chunkX, int chunkY){
    glm::vec2 camera2d = glm::vec2(cameraPosition.x, cameraPosition.z);
    glm::vec2 chunk2d = glm::vec2(chunkX, chunkY);
    
    return abs(glm::distance(camera2d, chunk2d)) < 60;
}

bool CloseMeshBuilder::chunkIsVisibleFromCamera(Camera camera, int offsetX, int offSetZ){
    
    float maxHeight = Chunk::GetMaxHeight(offsetX, offSetZ);
    
    return maxHeight > -100;
    
}

bool CloseMeshBuilder::positionIsInListOfChunks(int x, int y){
    for (int i = 0; i < chunks.size(); i++) {
        if (chunks.at(i).HasPosition(x, y)) return true;
    }
    
    return false;
}

bool CloseMeshBuilder::chunkIsVisible(Camera camera, glm::vec2 globalPosition){
    int chunkCenterX = Chunk::GetChunkCenterFromIndex(globalPosition.x);
    int chunkCenterZ = Chunk::GetChunkCenterFromIndex(globalPosition.y);

    return  camera.PointIsVisibleFromCamera(chunkCenterX, chunkCenterZ) &&
            chunkIsVisibleFromCamera(camera, globalPosition.x, globalPosition.y) &&
            cameraIsCloseToChunk(camera.GetPosition(), chunkCenterX, chunkCenterZ);
}

void CloseMeshBuilder::removeChunksOutOfView(Camera camera){
    
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
