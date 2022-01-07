//
//  QuadtreeNode.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#include "TerrainPatch.h"

TerrainPatch::TerrainPatch(int x, int z, int width, TerrainFaceDirection dir, int LOD){
    this->globalPositionX = x;
    this->globalPositionZ = z;
    this->globalPositionY = QuadtreeSettings::InitialWidth / 2;
    this->width = width;
    this->levelOfDetail = LOD;
    this->distanceBetweenVertices = (float)width / QuadtreeSettings::VerticesPerPatchSide;
    
    this->direction = dir;
        
    this->axisRotationMatrix = TerrainFace::GetAxisRotationMatrix(direction);
    
    this->meshOfPatch = TerrainMesh();
    createMesh();
}

TerrainPatch::~TerrainPatch(){}

TerrainPatch::TerrainPatch(const TerrainPatch& terrainPatch) {
    copyData(terrainPatch);
}

TerrainPatch::TerrainPatch(TerrainPatch&& terrainPatch) {
    copyData(terrainPatch);
}

TerrainPatch& TerrainPatch::operator=(const TerrainPatch& terrainPatch) {
    copyData(terrainPatch);
    return *this; 
}

void TerrainPatch::copyData(const TerrainPatch& terrainPatch){
    if (this != &terrainPatch) {
        this->globalPositionX = terrainPatch.globalPositionX;
        this->globalPositionY = terrainPatch.globalPositionY;
        this->globalPositionZ = terrainPatch.globalPositionZ;
        this->width = terrainPatch.width;
        this->levelOfDetail = terrainPatch.levelOfDetail;
        this->distanceBetweenVertices = terrainPatch.distanceBetweenVertices;
        
        this->direction = terrainPatch.direction;
        
        this->axisRotationMatrix = terrainPatch.axisRotationMatrix;
                
        this->meshOfPatch = terrainPatch.meshOfPatch;
        
        this->geomorphHandler.Reset();
    }
}

void TerrainPatch::Update(){
    if (geomorphHandler.TerrainIsBeingAnimated()){
        geomorphHandler.Update();
    }
}

void TerrainPatch::Render(){
    ActiveShaders::TerrainShader->SetUniform1f("u_TerrainAnimationPercentage", geomorphHandler.GetAnimationPercentage());
    meshOfPatch.Render(*ActiveShaders::TerrainShader);
}


void TerrainPatch::createMesh(){
    meshOfPatch.Clear();
    
    calculateVertices();
    calculateIndices();
    
    meshOfPatch.RecalculateNormals();
    meshOfPatch.UpdateBuffers();
}

glm::vec3 TerrainPatch::computeVertexPosition(float x, float z) const {
    float globalX = x * distanceBetweenVertices + globalPositionX;
    float globalZ = z * distanceBetweenVertices + globalPositionZ;

    glm::vec3 vertex = glm::vec3(globalX, globalPositionY, globalZ);
    vertex = axisRotationMatrix * vertex;
    vertex = PointCubeToSphere(vertex);
    vertex += MeshHeightHandler::GetHeight(vertex.x ,vertex.y, vertex.z, levelOfDetail) * glm::normalize(vertex);
    
    return vertex;
}

glm::vec3 TerrainPatch::computeOldVertexPosition(int x, int z) const {
    if (z % 2 == 0 && x % 2 != 0){
        glm::vec3 v1 = computeVertexPosition(x, z + 1);
        glm::vec3 v2 = computeVertexPosition(x, z - 1);
        return (v1 + v2) / 2.0f;
    } else if(z % 2 != 0 && x % 2 == 0) {
        glm::vec3 v1 = computeVertexPosition(x + 1, z);
        glm::vec3 v2 = computeVertexPosition(x - 1, z);
        return (v1 + v2) / 2.0f;
    } else {
        glm::vec3 v1 = computeVertexPosition(x + 1, z + 1);
        glm::vec3 v2 = computeVertexPosition(x - 1, z - 1);
        return (v1 + v2) / 2.0f;
    }
}

bool TerrainPatch::geomorphingIsEnabled() const {
    return levelOfDetail > QuadtreeSettings::MaxSubdivisions-3;
}

void TerrainPatch::calculateVertices(){
    for (float z = 0; z <= QuadtreeSettings::VerticesPerPatchSide; z++) {
        for (float x = 0; x <= QuadtreeSettings::VerticesPerPatchSide; x++) {
            
            TerrainVertex v = {};
            v.position = computeVertexPosition(x, z);
            if (geomorphingIsEnabled() && ((int)x % 2 != 0 || (int)z % 2 != 0)) {
                v.deltaPosition = v.position - computeOldVertexPosition(x, z);
            }  else {
                v.deltaPosition = glm::vec3();
            }
            
            meshOfPatch.AddVertex(v);
        }
    }
}


void TerrainPatch::calculateIndices(){
    int vertexIndex = 0;
    for (int z = 0; z <= QuadtreeSettings::VerticesPerPatchSide; z++) {
        for (int x = 0; x <= QuadtreeSettings::VerticesPerPatchSide; x++) {
            if (x < QuadtreeSettings::VerticesPerPatchSide && z < QuadtreeSettings::VerticesPerPatchSide){
                if (TerrainFace::IsBackFace(direction)){
                    
                    meshOfPatch.AddTriangleIndices(vertexIndex+1, vertexIndex + QuadtreeSettings::VerticesPerPatchSide + 1, vertexIndex);
                    meshOfPatch.AddTriangleIndices(vertexIndex + QuadtreeSettings::VerticesPerPatchSide + 2, vertexIndex + QuadtreeSettings::VerticesPerPatchSide + 1, vertexIndex + 1);
                
                } else {
                    
                    meshOfPatch.AddTriangleIndices(vertexIndex, vertexIndex + QuadtreeSettings::VerticesPerPatchSide + 1, vertexIndex+1);
                    meshOfPatch.AddTriangleIndices(vertexIndex + 1, vertexIndex + QuadtreeSettings::VerticesPerPatchSide + 1, vertexIndex + QuadtreeSettings::VerticesPerPatchSide + 2);
                    
                }
                vertexIndex++;
            }
        }
        vertexIndex++;
    }
}

glm::vec3 TerrainPatch::PointCubeToSphere(const glm::vec3& point) {
        
    glm::vec3 normalizedPoint = point / ((float)QuadtreeSettings::InitialWidth / 2);
    
    float x2 = normalizedPoint.x * normalizedPoint.x;
    float y2 = normalizedPoint.y * normalizedPoint.y;
    float z2 = normalizedPoint.z * normalizedPoint.z;
        
    return glm::vec3(normalizedPoint.x * sqrt(1.0 - (y2 + z2) * 0.5 + y2 * z2 * 0.33333333333333333333),
                     normalizedPoint.y * sqrt(1.0 - (z2 + x2) * 0.5 + z2 * x2 * 0.33333333333333333333),
                     normalizedPoint.z * sqrt(1.0 - (x2 + y2) * 0.5 + x2 * y2 * 0.33333333333333333333)) * (float)QuadtreeSettings::InitialWidth;
}
