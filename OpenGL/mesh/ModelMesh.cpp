//
//  ModelMesh.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 09/12/21.
//

#include "ModelMesh.h"

ModelMesh::ModelMesh(){
    this->vertices = std::vector<ModelVertex>();
    
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(vertices.data(), (unsigned int)vertices.size() * sizeof(ModelVertex));
    indexBuffer = new IndexBuffer(indices.data(), (unsigned int)indices.size());

    VertexBufferLayout layout;
    layout.AddFloat(3);
    layout.AddFloat(3);
    layout.AddFloat(3);
    layout.AddFloat(3); 

    vertexArray->AddBuffer(*vertexBuffer, layout);
}

ModelMesh::~ModelMesh(){
    delete vertexArray;
    delete indexBuffer;
    delete vertexBuffer;
}

ModelMesh::ModelMesh(const ModelMesh& mesh){
    copyData(mesh);
}

ModelMesh::ModelMesh(ModelMesh&& mesh){
    copyData(mesh);
}

void ModelMesh::copyData(const ModelMesh &mesh){
    this->vertices = mesh.vertices;
    this->indices = mesh.indices;
    
    delete vertexArray;
    delete indexBuffer;
    delete vertexBuffer;
    
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(vertices.data(), (unsigned int)vertices.size() * sizeof(ModelVertex));
    indexBuffer = new IndexBuffer(indices.data(), (unsigned int)indices.size());

    VertexBufferLayout layout;
    layout.AddFloat(3);
    layout.AddFloat(3);
    layout.AddFloat(3);
    layout.AddFloat(3);

    vertexArray->AddBuffer(*vertexBuffer, layout);
}

ModelMesh& ModelMesh::operator=(const ModelMesh &mesh){
    copyData(mesh);
    return *this;
}

void ModelMesh::UpdateBuffers(){
    vertexBuffer->Update(vertices.data(), (unsigned int)vertices.size() * sizeof(ModelVertex));
    indexBuffer->Update(indices.data(), (unsigned int)indices.size());
}

void ModelMesh::Clear(){
    vertices = std::vector<ModelVertex>();
    indices = std::vector<unsigned int>();
}

void ModelMesh::AddVertex(ModelVertex v){
    vertices.push_back(v);
}

void ModelMesh::Render(const Shader &shader, const unsigned int amount){
    OpenGLEngine::DrawInstanced(*vertexArray, *indexBuffer, shader, amount);
}
