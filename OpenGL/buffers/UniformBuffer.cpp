//
//  UniformBuffer.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 17/12/21.
//

#include "UniformBuffer.h"
#include "../engine/Debug.h"


UniformBuffer::UniformBuffer() {
    GLCall( glGenBuffers(1, &m_RendererID) );
    GLCall( glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID) );
}

UniformBuffer::~UniformBuffer() {
    GLCall( glDeleteBuffers(1, &m_RendererID) );
}

void UniformBuffer::Bind() const {
    GLCall( glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID) );
}

void UniformBuffer::Unbind() const {
    GLCall( glBindBuffer(GL_UNIFORM_BUFFER, 0) );
}

void UniformBuffer::SetUniformBlock3f(const std::string &name, glm::vec3 value) const {
    Bind();
    GLCall( glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(value)) );
}

void UniformBuffer::SetUniformBlock1f(const std::string &name, float f) const {
    Bind();
    GLCall( glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), (void*)&f) );
}

void UniformBuffer::SetUniformBlockMat4x4f(const std::string &name, glm::mat4x4& value) const {
    Bind();
    GLCall( glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(value)) );
}

void UniformBuffer::SetUniformBlock3fv(const std::string& name, const float *data, unsigned int count) const {
    Bind();
    GLCall( glBufferData(GL_UNIFORM_BUFFER, count * 3 * sizeof(float), data, GL_DYNAMIC_DRAW) );
}

void UniformBuffer::SetUniformBlock1fv(const std::string& name, const float *data, unsigned int count) const {
    Bind();
    GLCall( glBufferData(GL_UNIFORM_BUFFER, count * sizeof(float), data, GL_DYNAMIC_DRAW) );
}

void UniformBuffer::SetUniformBlock4fv(const std::string& name, const float *data, unsigned int count) const {
    Bind();
    GLCall( glBufferData(GL_UNIFORM_BUFFER, count * 4 * sizeof(float), data, GL_DYNAMIC_DRAW) );
}

void UniformBuffer::BindUniformBlock(const Shader& shader, const std::string &name, unsigned int bindingPoint, unsigned int size){
    Bind();
    m_Size = size;

    unsigned int blockIndex = glGetUniformBlockIndex(shader.GetID(), name.c_str());
    GLCall( glUniformBlockBinding(shader.GetID(), blockIndex, bindingPoint) );
    
    GLCall( glBufferData(GL_UNIFORM_BUFFER, m_Size, NULL, GL_STATIC_DRAW) );
    GLCall( glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_RendererID, 0, m_Size) );
}

void UniformBuffer::Update(const void *data, unsigned int size){
    Bind();
    m_Size = size;
    GLCall( glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW) );
}
