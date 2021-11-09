#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

#include "../materials/Material.h"

class Shader {
private:
    unsigned int m_RendererID;
    std::string m_VertexPath;
    std::string m_FragmentPath;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string& vertexpath, const std::string& fragmentPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, float f0, float f1, float f2);
    void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
    void SetUniformMaterial(const std::string& name, Material material);

private:
    int getUniformLocation(const std::string& name);
    std::string parseShader(const std::string& filepath);
    unsigned int compileShader(unsigned int type, const std::string& source);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

};
