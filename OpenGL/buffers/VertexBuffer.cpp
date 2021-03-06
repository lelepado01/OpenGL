#include "VertexBuffer.h"
#include "Debug.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    m_Size = size;
    
    GLCall( glGenBuffers(1, &m_RendererID) );
    GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );
    GLCall( glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW) );
}

VertexBuffer::~VertexBuffer() {
    GLCall( glDeleteBuffers(1, &m_RendererID) );
}

void VertexBuffer::Bind() const {
    GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );
}

void VertexBuffer::Unbind() const {
    GLCall( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}

void VertexBuffer::Update(const void *data, unsigned int size){
    Bind();
    m_Size = size; 
    GLCall( glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW) );
}

void VertexBuffer::SetAttribDivisor(const unsigned int attribIndex, const unsigned int instanceIndex) const {
    GLCall( glVertexAttribDivisor(attribIndex, instanceIndex) );
}
