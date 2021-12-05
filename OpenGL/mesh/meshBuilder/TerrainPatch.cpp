//
//  QuadtreeNode.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#include "TerrainPatch.h"

TerrainPatch::TerrainPatch(int x, int z, int width, TerrainFaceDirection dir, int LOD, TerrainPatchTransition transition){
    this->globalPositionX = x;
    this->globalPositionZ = z;
    this->globalPositionY = QuadtreeSettings::InitialWidth / 2;
    this->width = width;
    this->levelOfDetail = LOD;
    this->distanceBetweenVertices = (float)width / QuadtreeSettings::VerticesPerPatchSide;
    this->correctVerticesPerSide = QuadtreeSettings::VerticesPerPatchSide + 2;
    
    this->direction = dir;
    this->transition = transition;
    
    this->wasBuiltInTheLastSecond = true;
    this->timeOfBuildCall = Time::GetMillisecondsFromEpoch();
    this->incrementalTimeHeightMultiplier = 1;
    
    this->axisRotationMatrix = TerrainFace::GetAxisRotationMatrix(direction);

    this->minVertex = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    this->maxVertex = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    
    this->vertices = std::vector<Vertex>();
    this->indices = std::vector<unsigned int>();
    
    createMesh();
    
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(vertices.data(), (int)vertices.size() * sizeof(Vertex));
    indexBuffer = new IndexBuffer(indices.data(), (int)indices.size());
    
    VertexBufferLayout layout;
    layout.AddFloat(3);
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
        this->levelOfDetail = terrainPatch.levelOfDetail;
        this->distanceBetweenVertices = terrainPatch.distanceBetweenVertices;
        this->correctVerticesPerSide = terrainPatch.correctVerticesPerSide;
        
        this->direction = terrainPatch.direction;
        this->transition = terrainPatch.transition;
        
        this->vertices = terrainPatch.vertices;
        this->indices = terrainPatch.indices;
        
        this->axisRotationMatrix = terrainPatch.axisRotationMatrix;
        
        this->wasBuiltInTheLastSecond = terrainPatch.wasBuiltInTheLastSecond;
        this->timeOfBuildCall = terrainPatch.timeOfBuildCall;
        this->incrementalTimeHeightMultiplier = terrainPatch.incrementalTimeHeightMultiplier; 
        
        this->vertexArray = new VertexArray();
        this->vertexBuffer = new VertexBuffer(vertices.data(), (int)vertices.size() * sizeof(Vertex));
        this->indexBuffer = new IndexBuffer(indices.data(), (int)indices.size());
        
        this->minVertex = terrainPatch.minVertex;
        this->maxVertex = terrainPatch.maxVertex; 
        
        VertexBufferLayout layout;
        layout.AddFloat(3);
        layout.AddFloat(3);
        layout.AddFloat(3);

        this->vertexArray->AddBuffer(*vertexBuffer, layout);
    }
}

void TerrainPatch::Update(int lod){
    if (wasBuiltInTheLastSecond){

        long timeNow = Time::GetMillisecondsFromEpoch();
        if (timeNow - timeOfBuildCall >= transitionTimeInMilliseconds){
            wasBuiltInTheLastSecond = false;
            return;
        }
        
        incrementalTimeHeightMultiplier = ((float)(timeNow - timeOfBuildCall)) / transitionTimeInMilliseconds;
    }
    
    if (lod != levelOfDetail){
        if (lod > levelOfDetail) transition = TerrainPatchTransition::Upscale;
        else transition = TerrainPatchTransition::Downscale;

        levelOfDetail = lod;
        
        
        createMesh();
        updateBuffers();
    }
}

void TerrainPatch::Render(){
    
    if (wasBuiltInTheLastSecond){
        ActiveShaders::TerrainShader->SetUniform1f("u_IncrementalHeightMultiplier", incrementalTimeHeightMultiplier);
    } else {
        ActiveShaders::TerrainShader->SetUniform1f("u_IncrementalHeightMultiplier", -1.0f);
    }
    
    OpenGLEngine::Draw(*vertexArray, *indexBuffer, *ActiveShaders::TerrainShader);
}


void TerrainPatch::createMesh(){
    vertices.clear();
    indices.clear();
    
    calculateVertices();
    calculateIndices();
    calculateNormals();
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


glm::vec3 TerrainPatch::computeVertexNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) const {
    return glm::cross(b-a, c-a);
}


void TerrainPatch::calculateVertices(){
    for (float z = 0; z <= correctVerticesPerSide; z++) {
        for (float x = 0; x <= correctVerticesPerSide; x++) {
            
            float offsettedX = x -1;
            float offsettedZ = z -1;
            
            Vertex v = {};
            v.position = computeVertexPosition(offsettedX, offsettedZ);
            if ((int)x % 2 == 0 || (int)z % 2 == 0){
                v.oldPosition = (computeVertexPosition(offsettedX-1, offsettedZ) + computeVertexPosition(offsettedX+1, offsettedZ)) / 2.0f;
            } else {
                v.oldPosition = v.position;
            }
            vertices.push_back(v);

            calculateMinMax(v.position);
        }
    }
}


void TerrainPatch::updateBuffers(){
    vertexBuffer->Update(vertices.data(), (unsigned int)vertices.size() * sizeof(Vertex));
    indexBuffer->Update(indices.data(), (unsigned int)indices.size());
}

void TerrainPatch::calculateMinMax(const glm::vec3& point){
    minVertex.x = fmin(minVertex.x, point.x);
    minVertex.y = fmin(minVertex.y, point.y);
    minVertex.z = fmin(minVertex.z, point.z);

    maxVertex.x = fmax(maxVertex.x, point.x);
    maxVertex.y = fmax(maxVertex.y, point.y);
    maxVertex.z = fmax(maxVertex.z, point.z);
}


void TerrainPatch::calculateIndices(){
    int vertexIndex = 0;
    for (int z = 0; z <= correctVerticesPerSide; z++) {
        for (int x = 0; x <= correctVerticesPerSide; x++) {
            if (x < correctVerticesPerSide && z < correctVerticesPerSide){
                if (TerrainFace::IsBackFace(direction)){
                    indices.push_back(vertexIndex + 1);
                    indices.push_back(vertexIndex + correctVerticesPerSide + 1);
                    indices.push_back(vertexIndex);
                    
                    indices.push_back(vertexIndex + correctVerticesPerSide + 2);
                    indices.push_back(vertexIndex + correctVerticesPerSide + 1);
                    indices.push_back(vertexIndex + 1);
                } else {
                    indices.push_back(vertexIndex);
                    indices.push_back(vertexIndex + correctVerticesPerSide + 1);
                    indices.push_back(vertexIndex + 1);
                    
                    indices.push_back(vertexIndex + 1);
                    indices.push_back(vertexIndex + correctVerticesPerSide + 1);
                    indices.push_back(vertexIndex + correctVerticesPerSide + 2);
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


glm::vec3 TerrainPatch::PointCubeToSphere(const glm::vec3& point) {
        
    glm::vec3 normalizedPoint = point / ((float)QuadtreeSettings::InitialWidth / 2);
    
    float x2 = normalizedPoint.x * normalizedPoint.x;
    float y2 = normalizedPoint.y * normalizedPoint.y;
    float z2 = normalizedPoint.z * normalizedPoint.z;
        
    return glm::vec3(normalizedPoint.x * sqrt(1.0 - (y2 + z2) * 0.5 + y2 * z2 * 0.33333333333333333333),
                     normalizedPoint.y * sqrt(1.0 - (z2 + x2) * 0.5 + z2 * x2 * 0.33333333333333333333),
                     normalizedPoint.z * sqrt(1.0 - (x2 + y2) * 0.5 + x2 * y2 * 0.33333333333333333333)) * (float)QuadtreeSettings::InitialWidth;
}
