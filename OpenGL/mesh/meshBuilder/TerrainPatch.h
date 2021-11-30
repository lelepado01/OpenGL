//
//  TerrainPatch.h
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#ifndef TerrainPatch_h
#define TerrainPatch_h

#include <stdio.h>
#include <chrono>

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
#include "../../engine/Time.h"

class TerrainPatch {
private:
    int globalPositionX, globalPositionY, globalPositionZ, width;
    float distanceBetweenVertices;
    int LOD; 
    
    TerrainFaceDirection direction;  
        
    VertexArray* vertexArray = nullptr;
    VertexBuffer* vertexBuffer = nullptr;
    IndexBuffer* indexBuffer = nullptr;
    
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    
    bool wasBuiltInTheLastSecond;
    float incrementalTimeHeightMultiplier;
    long timeOfBuildCall;
    const int transitionTimeInMilliseconds = 10000; 
    
private:
    void copyData(const TerrainPatch& terrainPatch);
    
    void createMesh();
    
    void calculateVertices(); 
    void calculateIndices(); 
    void calculateNormals();
    
    glm::vec3 computeVertexPosition(float x, float z, glm::mat3x3& axisRotationMatrix); 
    glm::vec3 computeVertexNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    
public:
    TerrainPatch(int x, int z, int width, TerrainFaceDirection dir, int LOD);
    TerrainPatch(const TerrainPatch& terrainPatch);
    TerrainPatch(TerrainPatch&& terrainPatch);
    ~TerrainPatch();
    TerrainPatch& operator=(const TerrainPatch& terrainPatch);
    
    void Update(); 
    void Render();
    
    int GetVertexNumber() { return (int)vertices.size(); };
    glm::vec3& GetMinPoint() { return vertices[0].position; };
    glm::vec3& GetMaxPoint() { return vertices[vertices.size()].position; };

    static glm::vec3 PointCubeToSphere(glm::vec3 point);

};

#endif /* TerrainPatch_h */
