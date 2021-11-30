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
    this->LOD = LOD; 
    this->distanceBetweenVertices = (float)width / QuadtreeSettings::VerticesPerPatchSide;
    
    this->direction = dir;
    
    this->wasBuiltInTheLastSecond = true;
    this->timeOfBuildCall = Time::GetMillisecondsFromEpoch();
    this->incrementalTimeHeightMultiplier = 1;

    
    createMesh();
    
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(vertices.data(), (int)vertices.size() * sizeof(Vertex));
    indexBuffer = new IndexBuffer(indices.data(), (int)indices.size());
    
    VertexBufferLayout layout;
    layout.AddFloat(3);
    layout.AddFloat(3);

    vertexArray->AddBuffer(*vertexBuffer, layout);
}

TerrainPatch::~TerrainPatch(){
    delete vertexArray;
    delete indexBuffer;
    delete vertexBuffer;
}

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
        this->LOD = terrainPatch.LOD;
        this->distanceBetweenVertices = terrainPatch.distanceBetweenVertices;
        
        this->direction = terrainPatch.direction;
        
        this->vertices = terrainPatch.vertices;
        this->indices = terrainPatch.indices;
        
        this->wasBuiltInTheLastSecond = terrainPatch.wasBuiltInTheLastSecond;
        this->timeOfBuildCall = terrainPatch.timeOfBuildCall;
        this->incrementalTimeHeightMultiplier = terrainPatch.incrementalTimeHeightMultiplier; 
        
        this->vertexArray = new VertexArray();
        this->vertexBuffer = new VertexBuffer(vertices.data(), (int)vertices.size() * sizeof(Vertex));
        this->indexBuffer = new IndexBuffer(indices.data(), (int)indices.size());
        
        VertexBufferLayout layout;
        layout.AddFloat(3);
        layout.AddFloat(3);

        this->vertexArray->AddBuffer(*vertexBuffer, layout);

    }
}

void TerrainPatch::Update(){
    if (wasBuiltInTheLastSecond){

        long timeNow = Time::GetMillisecondsFromEpoch();
        if (timeNow - timeOfBuildCall >= transitionTimeInMilliseconds){
            wasBuiltInTheLastSecond = false;
            return;
        }
        
        incrementalTimeHeightMultiplier = ((float)(timeNow - timeOfBuildCall)) / transitionTimeInMilliseconds;
        
//        vertices = std::vector<Vertex>();
//        calculateVertices();
//
//        vertexBuffer->Update(vertices.data(), (unsigned int)vertices.size() * sizeof(Vertex));
//        indexBuffer->Update(indices.data(), (unsigned int)indices.size());
    }
}

void TerrainPatch::Render(){
    OpenGLEngine::Draw(*vertexArray, *indexBuffer, *ActiveShaders::TerrainShader);
}


void TerrainPatch::createMesh(){
    calculateVertices();
    calculateIndices();
    calculateNormals();
}

glm::vec3 TerrainPatch::computeVertexPosition(float x, float z, glm::mat3x3& axisRotationMatrix){
    float globalX = x * distanceBetweenVertices + globalPositionX;
    float globalZ = z * distanceBetweenVertices + globalPositionZ;

    glm::vec3 vertex = glm::vec3(globalX, globalPositionY, globalZ);
    vertex = axisRotationMatrix * vertex;
    vertex = PointCubeToSphere(vertex);
    vertex += MeshHeight::GetHeight(vertex.x ,vertex.y, vertex.z, LOD) * glm::normalize(vertex);
    
    return vertex;
}

glm::vec3 TerrainPatch::computeVertexNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c){
    return glm::cross(b-a, c-a);
}

void TerrainPatch::calculateVertices(){

    glm::mat3x3 axisRotationMatrix = TerrainFace::GetAxisRotationMatrix(direction);

    for (float z = 0; z <= QuadtreeSettings::VerticesPerPatchSide; z++) {
        for (float x = 0; x <= QuadtreeSettings::VerticesPerPatchSide; x++) {

            Vertex v = {};
            
//            if (LOD > 6 && wasBuiltInTheLastSecond && ((int)x % 2 == 1 || (int)z % 2 == 1)){
//
//                glm::vec3 pos1 = computeVertexPosition(x-1, z, axisRotationMatrix);
//                glm::vec3 pos2 = computeVertexPosition(x+1, z, axisRotationMatrix);
//
//                glm::vec3 previousLODVertexPosition = (pos1 + pos2) / 2.0f;
//
//                glm::vec3 finalPosition = computeVertexPosition(x, z, axisRotationMatrix);
//
//                glm::vec3 startToFinishDifference = previousLODVertexPosition - finalPosition;
//                glm::vec3 startToFinishDifferenceForTimeStep = startToFinishDifference * incrementalTimeHeightMultiplier;
//
//                v.position = previousLODVertexPosition + startToFinishDifferenceForTimeStep;
//
//            } else {
//
                v.position = computeVertexPosition(x, z, axisRotationMatrix);
                
//            }
            
            vertices.push_back(v);

        }
    }
}

void TerrainPatch::calculateIndices(){
    int vertexIndex = 0;
    for (int z = 0; z <= QuadtreeSettings::VerticesPerPatchSide; z++) {
        for (int x = 0; x <= QuadtreeSettings::VerticesPerPatchSide; x++) {
            if (x < QuadtreeSettings::VerticesPerPatchSide && z < QuadtreeSettings::VerticesPerPatchSide){
                if (TerrainFace::IsBackFace(direction)){
                    indices.push_back(vertexIndex + 1);
                    indices.push_back(vertexIndex + QuadtreeSettings::VerticesPerPatchSide + 1);
                    indices.push_back(vertexIndex);
                    
                    indices.push_back(vertexIndex + QuadtreeSettings::VerticesPerPatchSide + 2);
                    indices.push_back(vertexIndex + QuadtreeSettings::VerticesPerPatchSide + 1);
                    indices.push_back(vertexIndex + 1);
                } else {
                    indices.push_back(vertexIndex);
                    indices.push_back(vertexIndex + QuadtreeSettings::VerticesPerPatchSide + 1);
                    indices.push_back(vertexIndex + 1);
                    
                    indices.push_back(vertexIndex + 1);
                    indices.push_back(vertexIndex + QuadtreeSettings::VerticesPerPatchSide + 1);
                    indices.push_back(vertexIndex + QuadtreeSettings::VerticesPerPatchSide + 2);
                }
                vertexIndex++;
            }
        }
        vertexIndex++;
    }
}

void TerrainPatch::calculateNormals(){
    for (int normalTriangleIndex = 0; normalTriangleIndex < indices.size(); normalTriangleIndex+=3) {
        int vertexIndexA = indices.at(normalTriangleIndex);
        int vertexIndexB = indices.at(normalTriangleIndex + 1);
        int vertexIndexC = indices.at(normalTriangleIndex + 2);

        glm::vec3 normal = computeVertexNormal(vertices.at(vertexIndexA).position,
                                               vertices.at(vertexIndexB).position,
                                               vertices.at(vertexIndexC).position);
        
        vertices[vertexIndexA].normal += normal;
        vertices[vertexIndexB].normal += normal;
        vertices[vertexIndexC].normal += normal;
    }
    
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].normal = glm::normalize(vertices[i].normal);
    }
}


glm::vec3 TerrainPatch::PointCubeToSphere(glm::vec3 point) {
    
    point /= QuadtreeSettings::InitialWidth / 2;
    
    const double x2 = point.x * point.x;
    const double y2 = point.y * point.y;
    const double z2 = point.z * point.z;
        
    return glm::vec3(point.x * sqrt(1.0 - (y2 + z2) * 0.5 + y2 * z2 * 0.33333333333333333333),
                     point.y * sqrt(1.0 - (z2 + x2) * 0.5 + z2 * x2 * 0.33333333333333333333),
                     point.z * sqrt(1.0 - (x2 + y2) * 0.5 + x2 * y2 * 0.33333333333333333333)) * (float)QuadtreeSettings::InitialWidth;
}
