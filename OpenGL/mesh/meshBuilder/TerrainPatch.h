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
#include "../Vertex.h"
#include "../MeshHeight.h"
#include "../../engine/OpenGLEngine.h"
#include "../../shaders/ActiveShaders.h"
#include "../../settings/QuadtreeSettings.h"

#include "glm/glm.hpp"

class TerrainPatch {
private:
    int globalPositionX, globalPositionY, width;
    float distanceBetweenVertices;
    
    VertexArray* vertexArray;
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    
private:
    void createMesh();
    void calculateNormals();
    glm::vec3 computeVertexNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    
public:
    TerrainPatch(int x, int y, int width);
    ~TerrainPatch();
    
    void Update(); 
    void Render();
    
    int GetVertexNumber() { return (int)vertices.size(); };
};

#endif /* TerrainPatch_h */
