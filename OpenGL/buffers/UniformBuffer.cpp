//
//  UniformBuffer.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 17/12/21.
//

#include "UniformBuffer.h"
#include "../engine/Debug.h"


UniformBuffer::UniformBuffer() {
    blockOffset = std::unordered_map<std::string, unsigned int>();
    
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

void UniformBuffer::SetUniformBlockLight(const std::string &name, Light light){
    Bind();
    if (!blockOffset.contains(name)){
        blockOffset[name] = globalOffset;
        globalOffset += sizeof(Light);
    }
    GLCall( glBufferSubData(GL_UNIFORM_BUFFER, blockOffset[name], sizeof(Light), (void*)&light.direction.x) );
}

void UniformBuffer::SetUniformBlock3f(const std::string &name, glm::vec3 value) {
    Bind();
    if (!blockOffset.contains(name)){
        blockOffset[name] = globalOffset;
        globalOffset += sizeof(glm::vec3);
    }
    GLCall( glBufferSubData(GL_UNIFORM_BUFFER, blockOffset[name], sizeof(glm::vec3), glm::value_ptr(value)) );
}

void UniformBuffer::SetUniformBlock1f(const std::string &name, float f) {
    Bind();
    if (!blockOffset.contains(name)){
        blockOffset[name] = globalOffset;
        globalOffset += sizeof(float);
    }
    GLCall( glBufferSubData(GL_UNIFORM_BUFFER, blockOffset[name], sizeof(float), (void*)&f) );
}

void UniformBuffer::SetUniformBlockMat4x4f(const std::string &name, glm::mat4x4& value) {
    Bind();
    if (!blockOffset.contains(name)){
        blockOffset[name] = globalOffset;
        globalOffset += sizeof(glm::mat4x4);
    }
    GLCall( glBufferSubData(GL_UNIFORM_BUFFER, blockOffset[name], sizeof(glm::mat4), glm::value_ptr(value)) );
}

void UniformBuffer::SetUniformBlock3fv(const std::string& name, const float *data, unsigned int count) {
    Bind();
    if (!blockOffset.contains(name)){
        blockOffset[name] = globalOffset;
        globalOffset += count * 3 * sizeof(float);
    }
    GLCall( glBufferSubData(GL_UNIFORM_BUFFER, blockOffset[name], count * 3 * sizeof(float), data) );
}

void UniformBuffer::SetUniformBlock1fv(const std::string& name, const float *data, unsigned int count) {
    Bind();
    if (!blockOffset.contains(name)){
        blockOffset[name] = globalOffset;
        globalOffset += count * sizeof(float);
    }
    GLCall( glBufferSubData(GL_UNIFORM_BUFFER, blockOffset[name], count * sizeof(float), data) );
}

void UniformBuffer::BindUniformBlock(const Shader& shader, const std::string &name, unsigned int bindingPoint, unsigned int size){
    Bind();
    m_Size = size;

    unsigned int blockIndex = glGetUniformBlockIndex(shader.GetID(), name.c_str());
    GLCall( glUniformBlockBinding(shader.GetID(), blockIndex, bindingPoint) );
    
    GLCall( glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW) );
    GLCall( glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_RendererID, 0, m_Size) );
}

void UniformBuffer::Update(const void *data, unsigned int size){
    Bind();
    m_Size = size;
    GLCall( glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW) );
}
