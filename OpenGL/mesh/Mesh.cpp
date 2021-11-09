//
//  Mesh.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 06/11/21.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->indices = indices;
    
    setupMesh();
}

Mesh::~Mesh(){
    
}

void Mesh::setupMesh(){
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(this->vertices.data(), this->vertices.size() * sizeof(Vertex));
    indexBuffer = new IndexBuffer(this->indices.data(), this->indices.size());
    
    VertexBufferLayout layout;
    layout.AddFloat(3);
    layout.AddFloat(3);

    vertexArray->AddBuffer(*vertexBuffer, layout);
}
