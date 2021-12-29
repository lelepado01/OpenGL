//
//  TreeModelMesh.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 28/12/21.
//

#include "TreeModelMesh.h"


TreeModelMesh::TreeModelMesh(){
    this->vertices = std::vector<ModelVertex>();
    
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer(vertices.data(), (unsigned int)vertices.size() * sizeof(ModelVertex));
    indexBuffer = new IndexBuffer(indices.data(), (unsigned int)indices.size());

    VertexBufferLayout layout;
    layout.AddFloat(3);
    layout.AddFloat(3);
    layout.AddFloat(3);
    layout.AddFloat(3);
    
    layout.AddFloat(3, true);
    layout.AddFloat(1, true);

    AddBufferLayout(*vertexBuffer, layout);
}

TreeModelMesh::~TreeModelMesh(){
    delete vertexArray;
    delete indexBuffer;
    delete vertexBuffer;
}

TreeModelMesh::TreeModelMesh(const TreeModelMesh& mesh){
    copyData(mesh);
}

TreeModelMesh::TreeModelMesh(TreeModelMesh&& mesh){
    copyData(mesh);
}

void TreeModelMesh::copyData(const TreeModelMesh &mesh){
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

TreeModelMesh& TreeModelMesh::operator=(const TreeModelMesh &mesh){
    copyData(mesh);
    return *this;
}


