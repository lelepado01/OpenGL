//
//  ModelMesh.h
//  OpenGL
//
//  Created by Gabriele Padovani on 09/12/21.
//

#ifndef ModelMesh_h
#define ModelMesh_h

#include <stdio.h>
#include <optional>

//#include "../buffers/VertexBuffer.h"
//#include "../buffers/IndexBuffer.h"
//#include "../buffers/VertexArray.h"
//#include "../buffers/VertexBufferLayout.h"
//#include "../shaders/Shader.h"
#include "../engine/OpenGLEngine.h"
#include "Vertex.h"
#include "Mesh.h"


class ModelMesh : public Mesh {
private:
    std::vector<ModelVertex> vertices = std::vector<ModelVertex>();
    
    std::optional<glm::vec3> center = {};
    
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
    void AddVertex(ModelVertex v);
    void AddBufferLayout(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
    void Render(const Shader &shader, const unsigned int amount);
    
    glm::vec3 GetCenter();
}; 

#endif /* ModelMesh_h */
