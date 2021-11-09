
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"
#include "OpenGLEngine.h"
#include "camera/Camera.h"
#include "materials/Material.h"
#include "mesh/Vertex.h"
#include "mesh/Mesh.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"


int main( void ) {
    OpenGLEngine::Init();

    int positionsLength = 8;
    std::vector<Vertex> positions = std::vector<Vertex>(positionsLength);
    positions[0] = Vertex{};
    positions[0].position = glm::vec3(1.f, 1.f, 1.f);
    positions[0].normal = glm::vec3(1.f, 1.f, 1.f);
    positions[1] = Vertex{};
    positions[1].position = glm::vec3(-1.f, 1.f, 1.f);
    positions[1].normal = glm::vec3(-1.f, 1.f, 1.f);
    positions[2] = Vertex{};
    positions[2].position = glm::vec3(-1.f, 1.f, -1.f);
    positions[2].normal = glm::vec3(-1.f, 1.f, -1.f);
    positions[3] = Vertex{};
    positions[3].position = glm::vec3(1.f, 1.f, -1.f);
    positions[3].normal = glm::vec3(1.f, 1.f, -1.f);
    positions[4] = Vertex{};
    positions[4].position = glm::vec3(1.f, -1.f, 1.f);
    positions[4].normal = glm::vec3(1.f, -1.f, 1.f);
    positions[5] = Vertex{};
    positions[5].position = glm::vec3(-1.f, -1.f, 1.f);
    positions[5].normal = glm::vec3(-1.f, -1.f, 1.f);
    positions[6] = Vertex{};
    positions[6].position = glm::vec3(-1.f, -1.f, -1.f);
    positions[6].normal = glm::vec3(-1.f, -1.f, -1.f);
    positions[7] = Vertex{};
    positions[7].position = glm::vec3(1.f, -1.f, -1.f);
    positions[7].normal = glm::vec3(1.f, -1.f, -1.f);

    std::vector<unsigned int> indices = std::vector<unsigned int> {
        0, 1, 3,
        3, 1, 2,
        2, 6, 7,
        7, 3, 2,
        7, 6, 5,
        5, 4, 7,
        5, 1, 4,
        4, 1, 0,
        4, 3, 7,
        3, 4, 0,
        5, 6, 2,
        5, 1, 2
    };

    Camera camera(0, 0, 10);
        
    Mesh mesh(positions, indices);

    Shader shader("/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/Vertex.shader",
                  "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/Fragment.shader");
    shader.Bind();
    
    Material material = {};
    material.color = glm::vec3(1,0,0);
    material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
    material.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
    material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    material.shininess = 32.0f;

    Light light = {};
    light.position = glm::vec3(10,-10,-10);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    light.ambient = glm::vec3(0.4f, 0.4f, 0.4f);

    OpenGLEngine::ImguiInit();

    while( OpenGLEngine::IsRunning() ){
        OpenGLEngine::Clear();
        OpenGLEngine::UpdateTime(); 

        OpenGLEngine::ImguiNewFrame();
        
        camera.UpdateDirection();
        camera.UpdatePosition();
        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = (*camera.GetProjection()) * (*camera.GetView()) * model;
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniformMaterial("u_Material", material);
        
        shader.SetUniform3f("u_cameraPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        shader.SetUniform3f("u_Light.position", light.position.x, light.position.y, light.position.z);
        shader.SetUniform3f("u_Light.specular", light.specular.x, light.specular.y, light.specular.z);
        shader.SetUniform3f("u_Light.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
        shader.SetUniform3f("u_Light.ambient", light.ambient.x, light.ambient.y, light.ambient.z);

        OpenGLEngine::Draw(mesh.GetVertexArray(), mesh.GetIndexBuffer(), &shader);
        
        ImGui::SliderFloat3("Cube Color", &material.color.x, 0, 1);
        ImGui::SliderFloat3("Light Position", &light.position.x, -10, 10);
        ImGui::Text("%.1f FPS)", ImGui::GetIO().Framerate);
        ImGui::Checkbox("Debug Mode", OpenGLEngine::DebugMode()); 
                
        OpenGLEngine::ImguiDraw();

        OpenGLEngine::SwapBuffers();
        OpenGLEngine::PollEvents();
    }


    OpenGLEngine::ImguiQuit();

    OpenGLEngine::Quit(); 
    
    return 0;
}

