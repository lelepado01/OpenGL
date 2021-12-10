//
//  Mesh.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 06/12/21.
//

#include "Mesh.h"

Mesh::Mesh(){
    this->vertices = std::vector<Vertex>();
    this->indices = std::vector<unsigned int>();
    
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(vertices.data(), (unsigned int)vertices.size() * vertices[0].GetSize());
    indexBuffer = new IndexBuffer(indices.data(), (unsigned int)indices.size());

    VertexBufferLayout layout;
    layout.AddFloat(3);
    layout.AddFloat(3);
    layout.AddFloat(3);

    vertexArray->AddBuffer(*vertexBuffer, layout);
    
    this->minVertex = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    this->maxVertex = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

Mesh::~Mesh(){
    delete vertexArray;
    delete indexBuffer;
    delete vertexBuffer;
}

Mesh::Mesh(const Mesh& mesh){
    copyData(mesh);
}

Mesh::Mesh(Mesh&& mesh){
    copyData(mesh);
}

void Mesh::copyData(const Mesh &mesh){
    this->vertices = mesh.vertices;
    this->indices = mesh.indices;
    
    delete vertexArray;
    delete indexBuffer;
    delete vertexBuffer;
    
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(vertices.data(), (unsigned int)vertices.size() * vertices[0].GetSize());
    indexBuffer = new IndexBuffer(indices.data(), (unsigned int)indices.size());

    VertexBufferLayout layout;
    layout.AddFloat(3);
    layout.AddFloat(3);
    layout.AddFloat(3);

    vertexArray->AddBuffer(*vertexBuffer, layout);
    
    this->minVertex = mesh.minVertex;
    this->maxVertex = mesh.maxVertex;
}

Mesh& Mesh::operator=(const Mesh &mesh){
    copyData(mesh);
    return *this;
}

void Mesh::UpdateBuffers(){
    vertexBuffer->Update(vertices.data(), (unsigned int)vertices.size() * vertices[0].GetSize());
    indexBuffer->Update(indices.data(), (unsigned int)indices.size()); 
}

void Mesh::Clear(){
    vertices = std::vector<Vertex>();
    indices = std::vector<unsigned int>();
}

void Mesh::Render(const Shader &shader){
    OpenGLEngine::Draw(*vertexArray, *indexBuffer, shader);
}

void Mesh::AddVertex(Vertex v){
    vertices.push_back(v);
    calculateMinMax(v.position);
}

void Mesh::AddIndex(unsigned int index){
    indices.push_back(index); 
}

void Mesh::AddTriangleIndices(unsigned int ind1, unsigned int ind2, unsigned int ind3){
    indices.push_back(ind1);
    indices.push_back(ind2);
    indices.push_back(ind3);
}

void Mesh::RecalculateNormals(){
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

glm::vec3 Mesh::computeVertexNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) const {
    return glm::cross(b-a, c-a);
}

void Mesh::calculateMinMax(const glm::vec3& point){
    minVertex.x = fmin(minVertex.x, point.x);
    minVertex.y = fmin(minVertex.y, point.y);
    minVertex.z = fmin(minVertex.z, point.z);

    maxVertex.x = fmax(maxVertex.x, point.x);
    maxVertex.y = fmax(maxVertex.y, point.y);
    maxVertex.z = fmax(maxVertex.z, point.z);
}
