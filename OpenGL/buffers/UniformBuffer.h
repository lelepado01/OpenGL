//
//  UniformBuffer.hpp
//  OpenGL
//
//  Created by Gabriele Padovani on 17/12/21.
//

#ifndef UniformBuffer_h
#define UniformBuffer_h

#include <stdio.h>
#include <string>

#include "../shaders/Shader.h"

class UniformBuffer {
private:
    unsigned int m_RendererID;
    unsigned int m_Size;

public:
    UniformBuffer(unsigned int size);
    ~UniformBuffer();

    void Bind() const;
    void Unbind() const;
    
    void BindUniformBlock(const Shader& shader, const std::string &name, unsigned int bindingPoint);
    
    void SetUniformBlockMat4x4f(const std::string& name, glm::mat4x4& value) const;
    void SetUniformBlock3f(const std::string& name, glm::vec3 value) const;
    void SetUniformBlock1f(const std::string& name, float f) const;
    
    void Update(const void* data, unsigned int size);
};


#endif /* UniformBuffer_h */
