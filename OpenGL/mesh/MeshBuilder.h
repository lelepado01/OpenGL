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

#include "Vertex.h"
#include "Chunk.h"

#include "glm/gtx/norm.hpp"
#include <glm/gtx/string_cast.hpp>

class MeshBuilder {
private:
    std::vector<Vertex> vertices = std::vector<Vertex>();
//    std::vector<Vertex> gridVertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
        
    const int sphereRadius = 30;
    const int chunkNumber = 3; 
    
    std::vector<Chunk> chunks = std::vector<Chunk>(); 
    
public:
    MeshBuilder(glm::vec3 cameraPosition);
    ~MeshBuilder();
    
    void UpdateMesh(glm::vec3 cameraPosition);
    
    std::vector<Vertex>* GetVertices();
    std::vector<unsigned int>* GetIndices();

private:
    void createGridMesh(glm::vec3 cameraPosition);
    void gridMeshToSphere(glm::vec3 cameraPosition);

    float getGlobalOffset(glm::vec3 cameraPosition);
    
    int getChunkLOD(glm::vec3 cameraPosition, int offX, int offZ);

};

#endif /* MeshBuilder_h */
