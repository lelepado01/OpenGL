#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    m_Count = count;
    m_Size = count * sizeof(unsigned int);
    
    GLCall( glGenBuffers(1, &m_RendererID) );
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
    GLCall( glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW) );
}

IndexBuffer::~IndexBuffer() {
    GLCall( glDeleteBuffers(1, &m_RendererID) );
}

void IndexBuffer::Bind() const {
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
}

void IndexBuffer::Unbind() const {
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}

void IndexBuffer::Update(const unsigned int *indices, unsigned int count){
    int size = count * sizeof(unsigned int);
    Bind();
    m_Count = count;
    m_Size = size; 
    GLCall( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, indices) );
}
