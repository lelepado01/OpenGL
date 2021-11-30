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
#include "../../engine/Time.h"

class TerrainPatch {
private:
    int globalPositionX, globalPositionY, globalPositionZ, width;
    float distanceBetweenVertices;
    int levelOfDetail; 
    
    TerrainFaceDirection direction;  
        
    VertexArray* vertexArray = nullptr;
    VertexBuffer* vertexBuffer = nullptr;
    IndexBuffer* indexBuffer = nullptr;
    
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    
    glm::vec3 minVertex;
    glm::vec3 maxVertex;
    
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
    void calculateMinMax(const glm::vec3& point); 
    
    glm::vec3 computeVertexPosition(float x, float z, const glm::mat3x3& axisRotationMatrix);
    glm::vec3 computeVertexNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    
public:
    TerrainPatch(int x, int z, int width, TerrainFaceDirection dir, int LOD);
    TerrainPatch(const TerrainPatch& terrainPatch);
    TerrainPatch(TerrainPatch&& terrainPatch);
    ~TerrainPatch();
    TerrainPatch& operator=(const TerrainPatch& terrainPatch);
    
    void Update(); 
    void Render();
    
    int GetVertexNumber() const { return (int)vertices.size(); };
    
    const glm::vec3& GetMinPoint() const { return minVertex; };
    const glm::vec3& GetMaxPoint() const { return maxVertex; };

    static glm::vec3 PointCubeToSphere(const glm::vec3& point);

};

#endif /* TerrainPatch_h */
