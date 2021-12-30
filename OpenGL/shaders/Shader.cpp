#include "OpenGLEngine.h"
#include "Shader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexpath, const std::string& fragmentPath) : m_RendererID(0) {
    
    m_VertexPath = vertexpath;
    m_FragmentPath = fragmentPath;
        
    std::string vertexSource = parseShader(vertexpath);
    std::string fragmentSource = parseShader(fragmentPath);
    
    m_RendererID = createShader(vertexSource, fragmentSource);

    GLCall( glUseProgram(m_RendererID) );
}

Shader::Shader(const std::string& vertexpath, const std::string& geometryPath, const std::string& fragmentPath) : m_RendererID(0) {
    
    m_VertexPath = vertexpath;
    m_GeometryPath = geometryPath;
    m_FragmentPath = fragmentPath;
    
    std::string vertexSource = parseShader(vertexpath);
    std::string geometrySource = parseShader(geometryPath);
    std::string fragmentSource = parseShader(fragmentPath);

    m_RendererID = createShader(vertexSource, geometrySource, fragmentSource);

    GLCall( glUseProgram(m_RendererID) );
}

Shader::~Shader() {
    GLCall( glDeleteProgram(m_RendererID) );
}

Shader& Shader::operator=(Shader &shader){
    this->m_VertexPath = shader.m_VertexPath;
    this->m_FragmentPath = shader.m_FragmentPath;
    
    this->m_RendererID = shader.m_RendererID;
    
    GLCall( glUseProgram(m_RendererID) );
    
    return *this;
}

void Shader::Bind() const {
    GLCall( glUseProgram(m_RendererID) );
}

void Shader::Unbind() const {
    GLCall( glUseProgram(0) );
}

int Shader::getUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()){
        return m_UniformLocationCache[name];
    }

    GLCall( int location = glGetUniformLocation(m_RendererID, name.c_str()) );
    if (location == -1){
        std::cout << "No active uniform variable with name " << name << " found" << std::endl;
    }
    
    m_UniformLocationCache[name] = location;

    return location;
}

void Shader::SetUniform1i(const std::string& name, int value) {
    GLCall( glUniform1i(getUniformLocation(name), value) );
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall( glUniform1f(getUniformLocation(name), value) );
}

void Shader::SetUniform3f(const std::string &name, float f0, float f1, float f2){
    GLCall( glUniform3f(getUniformLocation(name), f0, f1, f2) );
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3) {
    GLCall( glUniform4f(getUniformLocation(name), f0, f1, f2, f3) );
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLCall( glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]) );
}

void Shader::SetUniformMaterial(const std::string &name, Material material){
    GLCall( glUniform3f(getUniformLocation(name + ".color"), material.color.x, material.color.y, material.color.z) );
    GLCall( glUniform3f(getUniformLocation(name + ".ambient"), material.ambient.x, material.ambient.y, material.ambient.z) );
    GLCall( glUniform3f(getUniformLocation(name + ".diffuse"), material.diffuse.x, material.diffuse.y, material.diffuse.z) );
    GLCall( glUniform3f(getUniformLocation(name + ".specular"), material.specular.x, material.specular.y, material.specular.z) );
    GLCall( glUniform1f(getUniformLocation(name + ".shininess"), material.shininess) );
}

void Shader::SetUniformLight(const std::string &name, Light light){
    SetUniform3f(name + ".direction", light.direction.x, light.direction.y, light.direction.z);
    SetUniform3f(name + ".specular", light.specular.x, light.specular.y, light.specular.z);
    SetUniform3f(name + ".diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
    SetUniform3f(name + ".ambient", light.ambient.x, light.ambient.y, light.ambient.z);
}


void Shader::SetUniform3fv(const std::string &name, const float *data, unsigned int count){
    GLCall( glUniform3fv(getUniformLocation(name), count, data) );
}

void Shader::SetUniform1fv(const std::string &name, const float *data, unsigned int count){
    GLCall( glUniform1fv(getUniformLocation(name), count, data) );
}

std::string Shader::parseShader(const std::string& filepath) {

    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss;

    while (getline(stream, line)) {
        
        if (line.find("#include") != std::string::npos){
            std::string path = line.substr(line.find("\"")+1, line.size());
            path = path.substr(0, path.size()-1); 
            std::cout << path << "\n";
            std::string includeSource = parseShader(path);
            
            ss << includeSource << '\n';
        }  else {
            ss << line << '\n';
        }
    }

    return ss.str();
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    
    GLCall( unsigned int id = glCreateShader(type) );
    const char* src = source.c_str();
    GLCall( glShaderSource(id, 1, &src, nullptr) );
    GLCall( glCompileShader(id) );

    // Error handling
    int result;
    GLCall( glGetShaderiv(id, GL_COMPILE_STATUS, &result) );
    //std::cout << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compile status: " << result << std::endl;
    
    if ( result == GL_FALSE ) {
        int length;
        GLCall( glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length) );
        char* message = (char*) alloca(length * sizeof(char));
        GLCall( glGetShaderInfoLog(id, length, &length, message) );
        std::cout 
            << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader"
            << std::endl;
        std::cout << message << std::endl;
        GLCall( glDeleteShader(id) );
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    // create a shader program
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall( glAttachShader(program, vs) );
    GLCall( glAttachShader(program, fs) );

    GLCall( glLinkProgram(program) );

    GLint program_linked;

    GLCall( glGetProgramiv(program, GL_LINK_STATUS, &program_linked) );
    std::cout << "Program link status: " << program_linked << std::endl;
    if (program_linked != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        GLCall( glGetProgramInfoLog(program, 1024, &log_length, message) );
        std::cout << "Failed to link program" << std::endl;
        std::cout << message << std::endl;
    }

    GLCall( glValidateProgram(program) );

    GLCall( glDeleteShader(vs) );
    GLCall( glDeleteShader(fs) );

    return program;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader) {
    // create a shader program
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int gs = compileShader(GL_GEOMETRY_SHADER, geometryShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall( glAttachShader(program, vs) );
    GLCall( glAttachShader(program, gs) );
    GLCall( glAttachShader(program, fs) );

    GLCall( glLinkProgram(program) );

    GLint program_linked;

    GLCall( glGetProgramiv(program, GL_LINK_STATUS, &program_linked) );
    std::cout << "Program link status: " << program_linked << std::endl;
    if (program_linked != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        GLCall( glGetProgramInfoLog(program, 1024, &log_length, message) );
        std::cout << "Failed to link program" << std::endl;
        std::cout << message << std::endl;
    }

    GLCall( glValidateProgram(program) );

    GLCall( glDeleteShader(vs) );
    GLCall( glDeleteShader(fs) );
    
    return program;
}
