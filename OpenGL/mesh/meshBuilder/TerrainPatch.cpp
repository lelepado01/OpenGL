//
//  QuadtreeNode.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#include "TerrainPatch.h"

TerrainPatch::TerrainPatch(int x, int y, int width){
    this->globalPositionX = x;
    this->globalPositionY = y;
    this->width = width;
    this->distanceBetweenVertices = (float)width / verticesPerSide;
    
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
//    delete vertexArray;
//    delete indexBuffer;
//    delete vertexBuffer;
}


void TerrainPatch::Update(){
    vertexBuffer->Update(vertices.data(), (unsigned int)vertices.size() * sizeof(Vertex));
    indexBuffer->Update(indices.data(), (unsigned int)indices.size());
}

void TerrainPatch::Render(){
    OpenGLEngine::Draw(*vertexArray, *indexBuffer, *ActiveShaders::TerrainShader);
}


void TerrainPatch::createMesh(){
    for (float z = 0; z <= verticesPerSide; z++) {
        for (float x = 0; x <= verticesPerSide; x++) {
            
            float globalX = x * distanceBetweenVertices + globalPositionX;
            float globalZ = z * distanceBetweenVertices + globalPositionY;
            
            Vertex v = {};
            v.position = glm::vec3(globalX, MeshHeight::GetHeight(globalX ,globalZ), globalZ);
            
            vertices.push_back(v);
        }
    }

    int vertexIndex = 0;
    for (int z = 0; z <= verticesPerSide; z++) {
        for (int x = 0; x <= verticesPerSide; x++) {
            if (x < verticesPerSide && z < verticesPerSide){
                indices.push_back(vertexIndex);
                indices.push_back(vertexIndex + verticesPerSide + 1);
                indices.push_back(vertexIndex + 1);
                
                indices.push_back(vertexIndex + 1);
                indices.push_back(vertexIndex + verticesPerSide + 1);
                indices.push_back(vertexIndex + verticesPerSide + 2);

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
