//
//  Mesh.h
//  OpenGL
//
//  Created by Gabriele Padovani on 06/12/21.
//

#ifndef Mesh_h
#define Mesh_h

#include <stdio.h>
#include <vector>

#include "../buffers/VertexBuffer.h"
#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexArray.h"
#include "../buffers/VertexBufferLayout.h"
#include "../shaders/Shader.h"
#include "../engine/OpenGLEngine.h"
#include "Vertex.h"

class Mesh {
private:
    VertexArray* vertexArray = nullptr;
    VertexBuffer* vertexBuffer = nullptr;
    IndexBuffer* indexBuffer = nullptr;
    
    std::vector<Vertex> vertices = std::vector<Vertex>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    
    glm::vec3 minVertex;
    glm::vec3 maxVertex;

    
private:
    void copyData(const Mesh& mesh);
    void calculateMinMax(const glm::vec3& point);

    glm::vec3 computeVertexNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) const;

public:
    Mesh();
    ~Mesh();
    Mesh(const Mesh& mesh);
    Mesh(Mesh&& mesh);
    Mesh& operator=(const Mesh& mesh);
    
    void UpdateBuffers();
    void Clear(); 
    void Render(const Shader& shader);
    void RecalculateNormals(); 
    
    void AddVertex(Vertex v);
    void AddTriangleIndices(unsigned int ind1, unsigned int ind2, unsigned int ind3);
    
    long GetVertexNumber() const { return vertices.size(); };
    const glm::vec3& GetMinPoint() const { return minVertex; };
    const glm::vec3& GetMaxPoint() const { return maxVertex; };

};

#endif /* Mesh_h */
