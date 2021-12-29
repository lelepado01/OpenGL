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
#include <unordered_map>

#include "../shaders/Shader.h"

class UniformBuffer {
private:
    unsigned int m_RendererID;
    unsigned int m_Size;
    
    unsigned int globalOffset = 0; 
    std::unordered_map<std::string, unsigned int> blockOffset;

public:
    UniformBuffer();
    ~UniformBuffer();

    void Bind() const;
    void Unbind() const;
    
    void BindUniformBlock(const Shader& shader, const std::string &name, unsigned int bindingPoint, unsigned int size);
    
    void SetUniformBlockMat4x4f(const std::string& name, glm::mat4x4& value);
    void SetUniformBlock3f(const std::string& name, glm::vec3 value);
    void SetUniformBlock3fv(const std::string& name, const float *data, unsigned int count);
    void SetUniformBlock1fv(const std::string& name, const float *data, unsigned int count);
    void SetUniformBlock1f(const std::string& name, float f);
    
    void Update(const void* data, unsigned int size);
};


#endif /* UniformBuffer_h */
