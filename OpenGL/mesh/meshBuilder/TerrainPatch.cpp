//
//  QuadtreeNode.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#include "TerrainPatch.h"

TerrainPatch::TerrainPatch(int x, int z, int width, TerrainFaceDirection dir, int LOD, TerrainPatchAnimation transition){
    this->globalPositionX = x;
    this->globalPositionZ = z;
    this->globalPositionY = QuadtreeSettings::InitialWidth / 2;
    this->width = width;
    this->levelOfDetail = LOD;
    this->distanceBetweenVertices = (float)width / QuadtreeSettings::VerticesPerPatchSide;
    this->correctVerticesPerSide = QuadtreeSettings::VerticesPerPatchSide + 2;
    
    this->direction = dir;
    this->transition = transition;
        
    this->axisRotationMatrix = TerrainFace::GetAxisRotationMatrix(direction);
    
    this->meshOfPatch = Mesh();
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
        this->correctVerticesPerSide = terrainPatch.correctVerticesPerSide;
        
        this->direction = terrainPatch.direction;
        this->transition = terrainPatch.transition;
        
        this->axisRotationMatrix = terrainPatch.axisRotationMatrix;
                
        this->meshOfPatch = terrainPatch.meshOfPatch;
    }
}

void TerrainPatch::Update(int lod){
    if (terrainPopHandler.TerrainIsBeingAnimated()){
        terrainPopHandler.Update();
    }
    
    if (lod != levelOfDetail){
        if (lod > levelOfDetail) transition = TerrainPatchAnimation::Upscale;
        else transition = TerrainPatchAnimation::Downscale;

        levelOfDetail = lod;
        
        createMesh();
    }
}

void TerrainPatch::Render(){
    ActiveShaders::TerrainShader->SetUniform1f("u_TerrainAnimationPercentage", terrainPopHandler.GetAnimationPercentage());
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

glm::vec3 TerrainPatch::computeVertexPosition(float x, float z, int lod) const {
    float globalX = x * distanceBetweenVertices + globalPositionX;
    float globalZ = z * distanceBetweenVertices + globalPositionZ;

    glm::vec3 vertex = glm::vec3(globalX, globalPositionY, globalZ);
    vertex = axisRotationMatrix * vertex;
    vertex = PointCubeToSphere(vertex);
    vertex += MeshHeightHandler::GetHeight(vertex.x ,vertex.y, vertex.z, lod) * glm::normalize(vertex);
    
    return vertex;
}

void TerrainPatch::calculateVertices(){
    for (float z = 0; z <= correctVerticesPerSide; z++) {
        for (float x = 0; x <= correctVerticesPerSide; x++) {
            
            float offsettedX = x - 1;
            float offsettedZ = z - 1;
            
            Vertex v = {};
            v.position = computeVertexPosition(offsettedX, offsettedZ);
            v.oldPosition = computeVertexPosition(offsettedX, offsettedZ, levelOfDetail-1);
            
            meshOfPatch.AddVertex(v);
        }
    }
}


void TerrainPatch::calculateIndices(){
    int vertexIndex = 0;
    for (int z = 0; z <= correctVerticesPerSide; z++) {
        for (int x = 0; x <= correctVerticesPerSide; x++) {
            if (x < correctVerticesPerSide && z < correctVerticesPerSide){
                if (TerrainFace::IsBackFace(direction)){
                    
                    meshOfPatch.AddTriangleIndices(vertexIndex+1, vertexIndex + correctVerticesPerSide + 1, vertexIndex);
                    meshOfPatch.AddTriangleIndices(vertexIndex + correctVerticesPerSide + 2, vertexIndex + correctVerticesPerSide + 1, vertexIndex + 1);
                
                } else {
                    
                    meshOfPatch.AddTriangleIndices(vertexIndex, vertexIndex + correctVerticesPerSide + 1, vertexIndex+1);
                    meshOfPatch.AddTriangleIndices(vertexIndex + 1, vertexIndex + correctVerticesPerSide + 1, vertexIndex + correctVerticesPerSide + 2);
                    
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
