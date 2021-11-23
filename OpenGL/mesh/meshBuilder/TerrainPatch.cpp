//
//  QuadtreeNode.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#include "TerrainPatch.h"

TerrainPatch::TerrainPatch(int x, int z, int width, TerrainFaceDirection dir){
    this->globalPositionX = x;
    this->globalPositionZ = z;
    this->globalPositionY = QuadtreeSettings::InitialWidth / 2;
    this->width = width;
    this->distanceBetweenVertices = (float)width / QuadtreeSettings::VerticesPerPatchSide;
    
    this->direction = dir;
    
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
    if (this != &terrainPatch) {
        this->globalPositionX = terrainPatch.globalPositionX;
        this->globalPositionY = terrainPatch.globalPositionY;
        this->globalPositionZ = terrainPatch.globalPositionZ;
        this->width = terrainPatch.width;
        this->distanceBetweenVertices = terrainPatch.distanceBetweenVertices;
        
        this->direction = terrainPatch.direction;
        
        this->vertices = terrainPatch.vertices;
        this->indices = terrainPatch.indices;
        
        this->vertexArray = new VertexArray();
        this->vertexBuffer = new VertexBuffer(vertices.data(), (int)vertices.size() * sizeof(Vertex));
        this->indexBuffer = new IndexBuffer(indices.data(), (int)indices.size());
        
        VertexBufferLayout layout;
        layout.AddFloat(3);
        layout.AddFloat(3);

        this->vertexArray->AddBuffer(*vertexBuffer, layout);

    }
}

TerrainPatch& TerrainPatch::operator=(const TerrainPatch& terrainPatch) {
    if (this != &terrainPatch) {
        this->globalPositionX = terrainPatch.globalPositionX;
        this->globalPositionY = terrainPatch.globalPositionY;
        this->globalPositionZ = terrainPatch.globalPositionZ;
        this->width = terrainPatch.width;
        this->distanceBetweenVertices = terrainPatch.distanceBetweenVertices;
        
        this->direction = terrainPatch.direction;
        
        this->vertices = terrainPatch.vertices;
        this->indices = terrainPatch.indices;
        
        this->vertexArray = new VertexArray();
        this->vertexBuffer = new VertexBuffer(vertices.data(), (int)vertices.size() * sizeof(Vertex));
        this->indexBuffer = new IndexBuffer(indices.data(), (int)indices.size());
        
        VertexBufferLayout layout;
        layout.AddFloat(3);
        layout.AddFloat(3);

        this->vertexArray->AddBuffer(*vertexBuffer, layout);

    }
    
    return *this; 
}


TerrainPatch::TerrainPatch(TerrainPatch&& terrainPatch) {
    if (this != &terrainPatch) {
        this->globalPositionX = terrainPatch.globalPositionX;
        this->globalPositionY = terrainPatch.globalPositionY;
        this->globalPositionZ = terrainPatch.globalPositionZ;
        this->width = terrainPatch.width;
        this->distanceBetweenVertices = terrainPatch.distanceBetweenVertices;
        
        this->direction = terrainPatch.direction;

        this->vertices = terrainPatch.vertices;
        this->indices = terrainPatch.indices;
        
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
    vertexBuffer->Update(vertices.data(), (unsigned int)vertices.size() * sizeof(Vertex));
    indexBuffer->Update(indices.data(), (unsigned int)indices.size());
}

void TerrainPatch::Render(){
    OpenGLEngine::Draw(*vertexArray, *indexBuffer, *ActiveShaders::TerrainShader);
}


void TerrainPatch::createMesh(){
    
    glm::mat3x3 axisRotationMatrix = GetAxisRotationMatrix();
    
    for (float z = 0; z <= QuadtreeSettings::VerticesPerPatchSide; z++) {
        for (float x = 0; x <= QuadtreeSettings::VerticesPerPatchSide; x++) {
            
            float globalX = x * distanceBetweenVertices + globalPositionX;
            float globalZ = z * distanceBetweenVertices + globalPositionZ;
            
            Vertex v = {};
            v.position = glm::vec3(globalX, MeshHeight::GetHeight(globalX ,globalZ) + globalPositionY, globalZ);
            v.position = axisRotationMatrix * v.position;
            v.position = pointCubeToSphere(v.position);
            
            vertices.push_back(v);
        }
    }

    int vertexIndex = 0;
    for (int z = 0; z <= QuadtreeSettings::VerticesPerPatchSide; z++) {
        for (int x = 0; x <= QuadtreeSettings::VerticesPerPatchSide; x++) {
            if (x < QuadtreeSettings::VerticesPerPatchSide && z < QuadtreeSettings::VerticesPerPatchSide){
                if (IsBackFace()){
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

    calculateNormals();

}

glm::vec3 TerrainPatch::computeVertexNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c){
    return glm::cross(b-a, c-a);
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


glm::vec3 TerrainPatch::pointCubeToSphere(glm::vec3 point) {
    
    return glm::normalize(point) * 100.0f;
    
    const double x2 = point.x * point.x;
    const double y2 = point.y * point.y;
    const double z2 = point.z * point.z;
        
    return glm::vec3(point.x * sqrt(1.0 - (y2 + z2) * 0.5 + y2 * z2 * 0.33333333333333333333),
                     point.y * sqrt(1.0 - (z2 + x2) * 0.5 + z2 * x2 * 0.33333333333333333333),
                     point.z * sqrt(1.0 - (x2 + y2) * 0.5 + x2 * y2 * 0.33333333333333333333));
}


bool TerrainPatch::IsBackFace(){
    return direction == TerrainFaceDirection::Back
        || direction == TerrainFaceDirection::Bottom
        || direction == TerrainFaceDirection::Right;
}

glm::mat3x3 TerrainPatch::GetAxisRotationMatrix(){
    if (direction == TerrainFaceDirection::Bottom){
        return glm::mat3x3(1,0,0,0,-1,0,0,0,1);
    } else if (direction == TerrainFaceDirection::Front){
        return glm::mat3x3(1,0,0,0,0,-1,0,1,0);
    } else if (direction == TerrainFaceDirection::Back){
        return glm::mat3x3(1,0,0,0,0,1,0,1,0);
    } else if (direction == TerrainFaceDirection::Left){
        return glm::mat3x3(0,1,0,-1,0,0,0,0,1);
    } else if (direction == TerrainFaceDirection::Right){
        return glm::mat3x3(0,1,0,+1,0,0,0,0,1);
    }
    
    return glm::identity<glm::mat3x3>();
}
