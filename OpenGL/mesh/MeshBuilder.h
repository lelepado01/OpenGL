//
//  MeshBuilder.h
//  OpenGL
//
//  Created by Gabriele Padovani on 09/11/21.
//

#ifndef MeshBuilder_h
#define MeshBuilder_h

#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "Vertex.h"
#include "Chunk.h"
#include "MeshHeight.h"
#include "../camera/Camera.h"

#include "glm/gtx/norm.hpp"
#include <glm/gtx/string_cast.hpp>

class MeshBuilder {
private:
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    
    std::vector<Chunk> chunks = std::vector<Chunk>();
    Chunk sampleLowLODChunk = Chunk(0,0,1);;

    std::unordered_map<int, int> distanceLOD = std::unordered_map<int, int>();
    
    const int sphereRadius = 30;
    const int chunkNumber = 10; 
    
    glm::vec2 lastFrameCameraChunkPosition; 
        
public:
    MeshBuilder(Camera camera);
    ~MeshBuilder();

    MeshHeight meshHeight;
    
    void UpdateMesh(Camera camera);
    
    std::vector<Vertex>* GetVertices();
    std::vector<unsigned int>* GetIndices();
    
    std::vector<Vertex>* GetLowLODVertices();
    std::vector<unsigned int>* GetLowLODIndices();

private:
    void gridMeshToSphere(glm::vec3 cameraPosition);
    void constructInitialMesh(Camera camera); 
    
    float getChunkCenterFromIndex(int i); 
    float getGlobalOffset(glm::vec3 cameraPosition);
    
    int getChunkLOD(glm::vec3 cameraPosition, int offX, int offZ);
    int getLODFromDistance(int distance);
    
    glm::vec2 getCameraChunkPosition(glm::vec3 cameraPosition);
    glm::vec3 getCameraLookDirection(glm::vec3 cameraDirection);
    
    bool cameraIsCloseToChunk(glm::vec3 cameraPosition, int chunkX, int chunkY);
    bool chunkIsVisibleFromCamera(Camera camera, int offsetX, int offSetZ, MeshHeight meshHeight);
    bool chunkIsVisible(Camera camera, glm::vec2 globalPosition);
    bool positionIsInListOfChunks(int x, int y);
    
    void removeChunksOutOfView(Camera camera);
};

#endif /* MeshBuilder_h */
