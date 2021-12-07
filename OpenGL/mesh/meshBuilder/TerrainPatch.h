//
//  TerrainPatch.h
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#ifndef TerrainPatch_h
#define TerrainPatch_h

#include <stdio.h>

#include "../../shaders/ActiveShaders.h"
#include "../../settings/QuadtreeSettings.h"
#include "../../engine/Time.h"
#include "../MeshHeightHandler.h"
#include "../Mesh.h"
#include "TerrainUtils.h"


class TerrainPatch {
private:
    int globalPositionX, globalPositionY, globalPositionZ, width;
    float distanceBetweenVertices;
    int levelOfDetail;
    int correctVerticesPerSide; 
    
    TerrainFaceDirection direction;
    TerrainPatchTransition transition;
    Mesh meshOfPatch;
    
    glm::mat3x3 axisRotationMatrix; 
            
    bool wasBuiltInTheLastSecond;
    float incrementalTimeHeightMultiplier;
    long timeOfBuildCall;
    const int transitionTimeInMilliseconds = 1000; 
    
private:
    void copyData(const TerrainPatch& terrainPatch);
    
    void createMesh();
    void calculateVertices();
    void calculateIndices();

    glm::vec3 computeVertexPosition(float x, float z) const;
    glm::vec3 computeVertexPosition(float x, float z, int lod) const;
    
public:
    TerrainPatch(int x, int z, int width, TerrainFaceDirection dir, int LOD, TerrainPatchTransition transition);
    TerrainPatch(const TerrainPatch& terrainPatch);
    TerrainPatch(TerrainPatch&& terrainPatch);
    ~TerrainPatch();
    TerrainPatch& operator=(const TerrainPatch& terrainPatch);
    
    void Update(int lod); 
    void Render();
    
    long GetVertexNumber() const { return meshOfPatch.GetVertexNumber(); };
    const glm::vec3& GetMinPoint() const { return meshOfPatch.GetMinPoint(); };
    const glm::vec3& GetMaxPoint() const { return meshOfPatch.GetMaxPoint(); };

    static glm::vec3 PointCubeToSphere(const glm::vec3& point);

};

#endif /* TerrainPatch_h */
