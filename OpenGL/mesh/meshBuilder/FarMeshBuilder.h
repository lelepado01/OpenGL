//
//  FarMeshBuilder.hpp
//  OpenGL
//
//  Created by Gabriele Padovani on 18/11/21.
//

#ifndef FarMeshBuilder_h
#define FarMeshBuilder_h

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

class FarMeshBuilder {
private:
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    
    std::vector<Chunk> chunks = std::vector<Chunk>();
        
public:
    FarMeshBuilder(Camera camera);
    ~FarMeshBuilder();
    
    void UpdateMesh(Camera camera);
    
    std::vector<Vertex>* GetVertices();
    std::vector<unsigned int>* GetIndices();

private:
//    void gridMeshToSphere(glm::vec3 cameraPosition);
//    float getGlobalOffset(glm::vec3 cameraPosition);
    
    void constructInitialMesh(Camera camera);
            
    bool cameraIsCloseToChunk(glm::vec3 cameraPosition, int chunkX, int chunkY);
    bool chunkIsVisibleFromCamera(Camera camera, int offsetX, int offSetZ);
    bool chunkIsVisible(Camera camera, glm::vec2 globalPosition);
    bool positionIsInListOfChunks(int x, int y);
    
    void removeChunksOutOfView(Camera camera);
};


#endif /* FarMeshBuilder_h */
