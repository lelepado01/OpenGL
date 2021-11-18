//
//  CloseMeshBuilder.h
//  OpenGL
//
//  Created by Gabriele Padovani on 09/11/21.
//

#ifndef CloseMeshBuilder_h
#define CloseMeshBuilder_h

#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "../Vertex.h"
#include "../Chunk.h"
#include "../MeshHeight.h"
#include "../../settings/MeshSettings.h"
#include "../../camera/Camera.h"

#include "glm/gtx/norm.hpp"
#include <glm/gtx/string_cast.hpp>

class CloseMeshBuilder {
private:
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    
    std::vector<Chunk> chunks = std::vector<Chunk>();
    
    std::unordered_map<int, int> distanceLOD = std::unordered_map<int, int>();
        
public:
    CloseMeshBuilder(Camera camera);
    ~CloseMeshBuilder();
    
    void UpdateMesh(Camera camera);
    
    std::vector<Vertex>* GetVertices();
    std::vector<unsigned int>* GetIndices();

private:
//    void gridMeshToSphere(glm::vec3 cameraPosition);
//    float getGlobalOffset(glm::vec3 cameraPosition);
    
    int getChunkLOD(glm::vec3 cameraPosition, int offX, int offZ);
    int getLODFromDistance(int distance);
        
    bool cameraIsCloseToChunk(glm::vec3 cameraPosition, int chunkX, int chunkY);
    bool chunkIsOcclusionCulled(Camera camera, int offsetX, int offsetZ);
    bool chunkIsVisible(Camera camera, glm::vec2 globalPosition);
    bool positionIsInListOfChunks(int x, int y);
    
    void removeChunksOutOfView(Camera camera);
};

#endif /* CloseMeshBuilder_h */
