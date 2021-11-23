//
//  TerrainPatch.h
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#ifndef TerrainPatch_h
#define TerrainPatch_h

#include <stdio.h>
#include "../../buffers/VertexBuffer.h"
#include "../../buffers/IndexBuffer.h"
#include "../../buffers/VertexArray.h"
#include "../../buffers/VertexBufferLayout.h"
#include "../../engine/OpenGLEngine.h"
#include "../../shaders/ActiveShaders.h"
#include "../../settings/QuadtreeSettings.h"
#include "../Vertex.h"
#include "../MeshHeight.h"
#include "TerrainFaceUtils.h"

class TerrainPatch {
private:
    int globalPositionX, globalPositionY, globalPositionZ, width;
    float distanceBetweenVertices;
    
    TerrainFaceDirection direction;  
        
    VertexArray* vertexArray = nullptr;
    VertexBuffer* vertexBuffer = nullptr;
    IndexBuffer* indexBuffer = nullptr;
    
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    
private:
    void createMesh();
    void calculateNormals();
    void copyData(const TerrainPatch& terrainPatch);
    
    glm::vec3 computeVertexNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    glm::vec3 pointCubeToSphere(glm::vec3 point);
    
public:
    TerrainPatch(int x, int z, int width, TerrainFaceDirection dir);
    TerrainPatch(const TerrainPatch& terrainPatch);
    TerrainPatch(TerrainPatch&& terrainPatch);
    ~TerrainPatch();
    TerrainPatch& operator=(const TerrainPatch& terrainPatch);
    
    void Update(); 
    void Render();
    
    int GetVertexNumber() { return (int)vertices.size(); };
};

#endif /* TerrainPatch_h */
