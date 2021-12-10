//
//  ModelMesh.h
//  OpenGL
//
//  Created by Gabriele Padovani on 09/12/21.
//

#ifndef ModelMesh_h
#define ModelMesh_h

#include <stdio.h>
#include "../buffers/VertexBuffer.h"
#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexArray.h"
#include "../buffers/VertexBufferLayout.h"
#include "../shaders/Shader.h"
#include "../engine/OpenGLEngine.h"
#include "Vertex.h"


class ModelMesh {
private:
    VertexArray* vertexArray = nullptr;
    VertexBuffer* vertexBuffer = nullptr;
    IndexBuffer* indexBuffer = nullptr;

    std::vector<ModelVertex> vertices = std::vector<ModelVertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    
private:
    void copyData(const ModelMesh& mesh);

public:
    ModelMesh();
    ~ModelMesh();
    ModelMesh(const ModelMesh& mesh);
    ModelMesh(ModelMesh&& mesh);
    ModelMesh& operator=(const ModelMesh& mesh);
    
    void UpdateBuffers();
    void Clear();
    void Render(const Shader& shader);
    
    void AddVertex(ModelVertex v);
    void AddIndex(unsigned int index);
    
    long GetVertexNumber() const { return vertices.size(); };
    long GetNumberOfIndices() const { return indices.size(); };
}; 

#endif /* ModelMesh_h */
