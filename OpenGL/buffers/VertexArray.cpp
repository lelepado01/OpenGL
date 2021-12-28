#include "VertexArray.h"
#include "Debug.h"

VertexArray::VertexArray() {
    GLCall( glGenVertexArrays(1, &m_RendererID) );
}

VertexArray::~VertexArray() {
    GLCall( glDeleteVertexArrays(1, &m_RendererID) );
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    Bind();
    vb.Bind();
    const std::vector<VertexBufferElement> elements = layout.GetElements();
//    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size() ; i++) {
        const VertexBufferElement element = elements[i];
        
        unsigned int globalIndex = (unsigned int)layoutElements.size();
        layoutElements.push_back(element);
        
        GLCall( glEnableVertexAttribArray(globalIndex) );
        GLCall( glVertexAttribPointer(globalIndex, element.count, element.type, element.normalized, layout.GetStride(), (void*)(uintptr_t)globalOffset) );
        
        globalOffset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const {
    GLCall( glBindVertexArray(m_RendererID) );
}

void VertexArray::Unbind() const {
    GLCall( glBindVertexArray(0) );
};
