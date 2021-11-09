//
//  Mesh.h
//  OpenGL
//
//  Created by Gabriele Padovani on 06/11/21.
//

#ifndef Mesh_h
#define Mesh_h

#include <stdio.h>
#include <vector>

#include "Vertex.h"
#include "../buffers/VertexArray.h"
#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexBufferLayout.h"
#include "../buffers/VertexBuffer.h"


class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    VertexArray* vertexArray;
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ~Mesh();
    
    VertexArray* GetVertexArray() { return vertexArray; };
    IndexBuffer* GetIndexBuffer() { return indexBuffer; };
    
private:
    void setupMesh();
};

#endif /* Mesh_h */
