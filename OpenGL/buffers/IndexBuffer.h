#pragma once

#include "Debug.h"

class IndexBuffer {
private:
    unsigned int m_RendererID;
    
    unsigned int m_Count;
    unsigned int m_Size;

public:
    IndexBuffer(const unsigned int* indices, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    void Update(const unsigned int* indices, unsigned int count);
    
    inline unsigned int GetCount() const { return m_Count; };
};
