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
#include "../camera/Camera.h"

#include "glm/gtx/norm.hpp"
#include <glm/gtx/string_cast.hpp>

class MeshBuilder {
private:
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
        
    std::unordered_map<int, int> distanceLOD = std::unordered_map<int, int>();
    
    const int sphereRadius = 30;
    const int chunkNumber = 20; 
    
    std::vector<Chunk> chunks = std::vector<Chunk>(); 
    
public:
    MeshBuilder(Camera camera);
    ~MeshBuilder();
    
    void UpdateMesh(Camera camera);
    
    std::vector<Vertex>* GetVertices();
    std::vector<unsigned int>* GetIndices();

private:
    void createGridMesh(glm::vec3 cameraPosition);
    void gridMeshToSphere(glm::vec3 cameraPosition);

    float getGlobalOffset(glm::vec3 cameraPosition);
    
    int getChunkLOD(glm::vec3 cameraPosition, int offX, int offZ);
    int getLODFromDistance(int distance);
    
    glm::vec3 getCameraChunkPosition(glm::vec3 cameraPosition);
    glm::vec3 getCameraLookDirection(glm::vec3 cameraDirection);

};

#endif /* MeshBuilder_h */
