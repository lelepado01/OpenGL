//
//  TerrainPatch.h
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#ifndef TerrainPatch_h
#define TerrainPatch_h

#include <stdio.h>

#include "../shaders/ActiveShaders.h"
#include "../settings/QuadtreeSettings.h"
#include "../engine/Time.h"
#include "../mesh/TerrainMesh.h"
#include "utils/MeshHeightHandler.h"
#include "utils/TerrainUtils.h"
#include "utils/GeomorphHandler.h"


class TerrainPatch {
private:
    int globalPositionX, globalPositionY, globalPositionZ, width;
    int levelOfDetail;

    float distanceBetweenVertices;
    int correctVerticesPerSide; 
    
    TerrainFaceDirection direction;
    GeomorphHandler geomorphHandler;

    TerrainMesh meshOfPatch;
    
    glm::mat3x3 axisRotationMatrix; 
                
private:
    void copyData(const TerrainPatch& terrainPatch);
    
    void createMesh();
    void calculateVertices();
    void calculateIndices();

    glm::vec3 computeVertexPosition(float x, float z) const;
    glm::vec3 computeVertexPosition(float x, float z, int lod) const;
    
    bool geomorphingIsEnabled() const;
    bool pointIsOnBorder(const int x, const int y) const; 
    
public:
    TerrainPatch(int x, int z, int width, TerrainFaceDirection dir, int LOD);
    TerrainPatch(const TerrainPatch& terrainPatch);
    TerrainPatch(TerrainPatch&& terrainPatch);
    ~TerrainPatch();
    TerrainPatch& operator=(const TerrainPatch& terrainPatch);
    
    void Update(); 
    void Render();
    
    long GetVertexNumber() const { return meshOfPatch.GetVertexNumber(); };
    const glm::vec3& GetMinPoint() const { return meshOfPatch.GetMinPoint(); };
    const glm::vec3& GetMaxPoint() const { return meshOfPatch.GetMaxPoint(); };

    static glm::vec3 PointCubeToSphere(const glm::vec3& point);

};

#endif /* TerrainPatch_h */
