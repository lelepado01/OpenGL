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
    VertexArray* vertexArray;
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    
    int verticesNumber; 
    
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
    ~Mesh();
    
    void UpdateMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
    
    VertexArray& GetVertexArray() { return *vertexArray; };
    IndexBuffer& GetIndexBuffer() { return *indexBuffer; };
    
    int GetVerticesNumber(){ return verticesNumber; }; 
};

#endif /* Mesh_h */
