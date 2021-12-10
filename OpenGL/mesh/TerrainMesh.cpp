//
//  TerrainMesh.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 09/12/21.
//

#include "TerrainMesh.h"

TerrainMesh::TerrainMesh(){
    this->vertices = std::vector<TerrainVertex>();
    
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(vertices.data(), (unsigned int)vertices.size() * sizeof(TerrainVertex));
    indexBuffer = new IndexBuffer(indices.data(), (unsigned int)indices.size());

    VertexBufferLayout layout;
    layout.AddFloat(3);
    layout.AddFloat(3);
    layout.AddFloat(3);

    vertexArray->AddBuffer(*vertexBuffer, layout);    
}

TerrainMesh::~TerrainMesh(){
    delete vertexArray;
    delete indexBuffer;
    delete vertexBuffer;
}

TerrainMesh::TerrainMesh(const TerrainMesh& mesh){
    copyData(mesh);
}

TerrainMesh::TerrainMesh(TerrainMesh&& mesh){
    copyData(mesh);
}

void TerrainMesh::copyData(const TerrainMesh &mesh){
    this->vertices = mesh.vertices;
    this->indices = mesh.indices;
    
    delete vertexArray;
    delete indexBuffer;
    delete vertexBuffer;
    
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(vertices.data(), (unsigned int)vertices.size() * sizeof(TerrainVertex));
    indexBuffer = new IndexBuffer(indices.data(), (unsigned int)indices.size());

    VertexBufferLayout layout;
    layout.AddFloat(3);
    layout.AddFloat(3);
    layout.AddFloat(3);

    vertexArray->AddBuffer(*vertexBuffer, layout);
    
    this->minVertex = mesh.minVertex;
    this->maxVertex = mesh.maxVertex;
}

TerrainMesh& TerrainMesh::operator=(const TerrainMesh &mesh){
    copyData(mesh);
    return *this;
}

void TerrainMesh::UpdateBuffers(){
    vertexBuffer->Update(vertices.data(), (unsigned int)vertices.size() * sizeof(TerrainVertex));
    indexBuffer->Update(indices.data(), (unsigned int)indices.size());
}

void TerrainMesh::Clear(){
    vertices = std::vector<TerrainVertex>();
    indices = std::vector<unsigned int>();
}

void TerrainMesh::AddVertex(TerrainVertex v){
    vertices.push_back(v);
    calculateMinMax(v.position);
}

void TerrainMesh::RecalculateNormals(){
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

