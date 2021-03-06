#pragma once

class VertexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Size;

public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
    
    void Update(const void* data, unsigned int size);
    
    void SetAttribDivisor(const unsigned int attribIndex, const unsigned int instanceIndex) const;
};
