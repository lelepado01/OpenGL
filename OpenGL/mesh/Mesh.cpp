//
//  Mesh.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 06/11/21.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(vertices.data(), (int)vertices.size() * sizeof(Vertex));
    indexBuffer = new IndexBuffer(indices.data(), (int)indices.size());
    
    VertexBufferLayout layout;
    layout.AddFloat(3);
    layout.AddFloat(3);

    vertexArray->AddBuffer(*vertexBuffer, layout);
}

Mesh::~Mesh(){
    
}

void Mesh::UpdateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices){
    vertexBuffer->Update(vertices.data(), (unsigned int)vertices.size() * sizeof(Vertex));
    indexBuffer->Update(indices.data(), (unsigned int)indices.size());
}
