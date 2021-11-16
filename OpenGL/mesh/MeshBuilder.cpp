//
//  MeshBuilder.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 09/11/21.
//

#include "MeshBuilder.h"

MeshBuilder::MeshBuilder(Camera camera){
    
    meshHeight = MeshHeight(); 
    
    distanceLOD[30] = 2;
    distanceLOD[20] = 4;
    
    UpdateMesh(camera);
}


MeshBuilder::~MeshBuilder(){
    
}

void MeshBuilder::UpdateMesh(Camera camera){
    glm::vec2 cameraChunkPosition = getCameraChunkPosition(camera.GetPosition());
    if ( cameraChunkPosition == lastFrameCameraChunkPosition && !camera.HasRotated()) {
        return;
    }
    lastFrameCameraChunkPosition = cameraChunkPosition;
    
    // chunks = std::vector<Chunk>();
    
    removeChunksOutOfView(camera);
    
    for (int x = -chunkNumber; x < chunkNumber; x++) {
        for (int z = -chunkNumber; z < chunkNumber; z++) {
            
            int xglobal = x + cameraChunkPosition.x;
            int zglobal = z + cameraChunkPosition.y;
            
            if (chunkIsAlreadyBuilt(xglobal, zglobal)) continue;
            
            int chunkCenterX = Chunk::GetChunkCenterFromIndex(xglobal);
            int chunkCenterZ = Chunk::GetChunkCenterFromIndex(zglobal);
            
            if (camera.PointIsVisibleFromCamera(chunkCenterX, chunkCenterZ) && chunkIsVisibleFromCamera(camera, xglobal, zglobal, meshHeight)){
                Chunk c(xglobal, zglobal, meshHeight, getChunkLOD(camera.GetPosition(), xglobal, zglobal));
                chunks.push_back(c);
            } else if (cameraIsCloseToChunk(camera.GetPosition(), chunkCenterX, chunkCenterZ)){
                Chunk c(xglobal, zglobal, meshHeight, 1);
                chunks.push_back(c);
            }
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
    glm::vec3 chunkPosition = glm::vec3(Chunk::GetChunkCenterFromIndex(offX), 0.0f, Chunk::GetChunkCenterFromIndex(offX));

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

glm::vec2 MeshBuilder::getCameraChunkPosition(glm::vec3 cameraPosition){
    return glm::vec2(Chunk::GetChunkIndexFromPosition(cameraPosition.x), Chunk::GetChunkIndexFromPosition(cameraPosition.z));
}

bool MeshBuilder::cameraIsCloseToChunk(glm::vec3 cameraPosition, int chunkX, int chunkY){
    glm::vec2 camera2d = glm::vec2(cameraPosition.x, cameraPosition.z);
    glm::vec2 chunk2d = glm::vec2(chunkX, chunkY);
    
    return abs(glm::distance(camera2d, chunk2d)) < 60;
}

bool MeshBuilder::chunkIsVisibleFromCamera(Camera camera, int offsetX, int offSetZ, MeshHeight meshHeight){
    
    float maxHeight = Chunk::GetMaxHeight(offsetX, offSetZ, meshHeight);
    
    return maxHeight > -100;
    
}

bool MeshBuilder::chunkIsAlreadyBuilt(int x, int y){
    for (int i = 0; i < chunks.size(); i++) {
        if (chunks.at(i).HasPosition(x, y)) return true;
    }
    
    return false;
}

bool MeshBuilder::chunkIsVisible(Camera camera, glm::vec2 globalPosition){
    int chunkCenterX = Chunk::GetChunkCenterFromIndex(globalPosition.x);
    int chunkCenterZ = Chunk::GetChunkCenterFromIndex(globalPosition.y);

    return  camera.PointIsVisibleFromCamera(chunkCenterX, chunkCenterZ) &&
            chunkIsVisibleFromCamera(camera, globalPosition.x, globalPosition.y, meshHeight) &&
            cameraIsCloseToChunk(camera.GetPosition(), chunkCenterX, chunkCenterZ);
}

void MeshBuilder::removeChunksOutOfView(Camera camera){
    
    for (int i = 0; i < chunks.size(); i++) {
        glm::vec2 globalPosition = chunks.at(i).GetPosition(); 
    
        if (!chunkIsVisible(camera, globalPosition)){
            chunks.erase(chunks.begin() + i);
            i--;
        }
    }
}
