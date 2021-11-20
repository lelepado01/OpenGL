//
//  MeshBuilder.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 09/11/21.
//

#include "CloseMeshBuilder.h"

CloseMeshBuilder::CloseMeshBuilder(Camera camera){
    distanceLOD[4 * ChunkSettings::ChunkSize] = 2;
    distanceLOD[2 * ChunkSettings::ChunkSize] = 4;
    
    UpdateMesh(camera);
}

CloseMeshBuilder::~CloseMeshBuilder(){}

void CloseMeshBuilder::UpdateMesh(Camera camera){
    if ( !camera.HasMoved() && !camera.HasRotated()) {
        return;
    }
    
    removeChunksOutOfView(camera);
    
    glm::vec2 cameraChunkPosition = glm::vec2(Chunk::GetChunkIndexFromPosition(camera.GetPosition().x),
                                              Chunk::GetChunkIndexFromPosition(camera.GetPosition().z));
    
    // TODO: do I have to iterate over all chunks?
    // IF has changed chunks ->
    //      check if out of view
    //      check if has to change LOD
    //      append border chunks at min LOD
    for (int x = -MeshSettings::ChunkNumber; x < MeshSettings::ChunkNumber; x++) {
        for (int z = -MeshSettings::ChunkNumber; z < MeshSettings::ChunkNumber; z++) {

            int xglobal = x + cameraChunkPosition.x;
            int zglobal = z + cameraChunkPosition.y;
            int chunkLOD = getChunkLOD(camera.GetPosition(), xglobal, zglobal);
            
            if (chunkIsAlreadyLoaded(xglobal, zglobal, chunkLOD)) continue;

            int chunkCenterX = Chunk::GetChunkCenterFromIndex(xglobal);
            int chunkCenterZ = Chunk::GetChunkCenterFromIndex(zglobal);

            if ((camera.PointIsVisibleFromCamera(chunkCenterX, chunkCenterZ) && !chunkIsOcclusionCulled(camera, xglobal, zglobal)) || cameraIsCloseToChunk(camera.GetPosition(), chunkCenterX, chunkCenterZ)){
                Chunk c(xglobal, zglobal, chunkLOD);
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

const std::vector<Vertex>& CloseMeshBuilder::GetVertices() {
    vertices = std::vector<Vertex>();

    for (int i=0; i < chunks.size(); i++) {
        
        std::vector<Vertex>* vert = chunks[i].GetVertices();
    
        for (int j = 0; j < vert->size(); j++){
            vertices.push_back(vert[0][j]);
        }
    }
    
    return vertices;
}

const std::vector<unsigned int>& CloseMeshBuilder::GetIndices(){
    indices = std::vector<unsigned int>();

    int chunkOffset = 0;
    for (int i=0; i < chunks.size(); i++) {
        
        std::vector<unsigned int>* ind = chunks[i].GetIndices();
        for (int j = 0; j < ind->size(); j++){
            indices.push_back(ind[0][j] + chunkOffset);
        }
        
        chunkOffset += chunks[i].GetVertices()->size();
    }
    
    return indices;
}

int CloseMeshBuilder::getChunkLOD(glm::vec3 cameraPosition, int offX, int offZ){
    glm::vec3 chunkPosition = glm::vec3(Chunk::GetChunkCenterFromIndex(offX), 0.0f, Chunk::GetChunkCenterFromIndex(offZ));

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

bool CloseMeshBuilder::cameraIsCloseToChunk(glm::vec3 cameraPosition, int chunkX, int chunkY){
    glm::vec2 camera2d = glm::vec2(cameraPosition.x, cameraPosition.z);
    glm::vec2 chunk2d = glm::vec2(chunkX, chunkY);
    
    return glm::distance(camera2d, chunk2d) < 2 * ChunkSettings::ChunkSize;
}

bool CloseMeshBuilder::chunkIsOcclusionCulled(Camera camera, int offsetX, int offsetZ){
    
    // TODO: frustrum culling not working
    // TODO: not correct (completely)
    
    glm::vec3 chunkPoint = glm::vec3(offsetX * ChunkSettings::ChunkSize, Chunk::GetMaxHeight(offsetX, offsetZ), offsetZ * ChunkSettings::ChunkSize);
    glm::vec3 cameraPosition = camera.GetPosition();
    cameraPosition.y -= 7;
    
    float stepNumber = 100;
    glm::vec3 stepDirection = chunkPoint - cameraPosition;
    glm::vec3 stepAmount = stepDirection / stepNumber;
    
    float cameraToChunkDistance = glm::distance(chunkPoint, cameraPosition);
    float cameraToIntermediatePointDistance = 0;
    
    for (glm::vec3 intermediatePoint = camera.GetPosition(); cameraToIntermediatePointDistance < cameraToChunkDistance; intermediatePoint += stepAmount) {
        
        cameraToIntermediatePointDistance = glm::distance(intermediatePoint, cameraPosition);
        
        if (MeshHeight::GetHeight(intermediatePoint.x, intermediatePoint.z) > intermediatePoint.y){
            return true;
        }
    }

    return false;
    
}

bool CloseMeshBuilder::chunkIsAlreadyLoaded(int x, int y, int lod){
    for (int i = 0; i < chunks.size(); i++) {
        if (chunks.at(i).HasPosition(x, y) && chunks.at(i).GetLOD() == lod) return true;
    }
    
    return false;
}

bool CloseMeshBuilder::chunkIsVisible(Camera camera, glm::vec2 globalPosition){
    int chunkCenterX = Chunk::GetChunkCenterFromIndex(globalPosition.x);
    int chunkCenterZ = Chunk::GetChunkCenterFromIndex(globalPosition.y);

    return camera.PointIsVisibleFromCamera(chunkCenterX, chunkCenterZ) &&
           !chunkIsOcclusionCulled(camera, globalPosition.x, globalPosition.y) &&
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
