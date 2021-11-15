
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shaders/Shader.h"
#include "engine/OpenGLEngine.h"
#include "camera/Camera.h"
#include "materials/Material.h"
#include "mesh/Mesh.h"
#include "mesh/MeshBuilder.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"


int main( void ) {
    OpenGLEngine::Init();

    Camera camera(0, 0, 0);
        
    MeshBuilder meshBuilder = MeshBuilder(camera);
    Mesh mesh(*meshBuilder.GetVertices(), *meshBuilder.GetIndices());

    Shader shader("/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/Vertex.shader",
                  "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/Fragment.shader");
    shader.Bind();
    
    Material material = {};
    material.color = glm::vec3(1,1,1);
    material.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    material.diffuse = glm::vec3(0.8, 0.8f, 0.8f);
    material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    material.shininess = 0.2f;

    Light light = {};
    light.direction = glm::vec3(0.8f, -1.0f, 0.3f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    light.ambient = glm::vec3(0.8f, 0.8f, 0.8f);

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

        shader.SetUniform3f("u_Light.direction", light.direction.x, light.direction.y, light.direction.z);
        shader.SetUniform3f("u_Light.specular", light.specular.x, light.specular.y, light.specular.z);
        shader.SetUniform3f("u_Light.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
        shader.SetUniform3f("u_Light.ambient", light.ambient.x, light.ambient.y, light.ambient.z);

        if (camera.HasMoved() || camera.HasRotated()){
            meshBuilder.UpdateMesh(camera);
            mesh.UpdateMesh(*meshBuilder.GetVertices(), *meshBuilder.GetIndices());
        }
        
        OpenGLEngine::Draw(mesh.GetVertexArray(), mesh.GetIndexBuffer(), &shader);
        
        ImGui::SliderFloat3("Cube Color", &material.color.x, 0, 1);

        ImGui::SliderFloat("Large Freq", &meshBuilder.meshHeight.LargeFrequency, 0, 1);
        ImGui::SliderFloat("Small Freq", &meshBuilder.meshHeight.SmallFrequency, 0, 1);
        ImGui::SliderFloat("Small Scale", &meshBuilder.meshHeight.SmallScale, 0, 1);
        ImGui::SliderFloat("Large Scale", &meshBuilder.meshHeight.LargeScale, 0, 1);
        ImGui::SliderFloat("Large Mult", &meshBuilder.meshHeight.LargeMultiplier, 1, 5);
        
        ImGui::Separator();
        
        ImGui::Text("%.1f FPS)", ImGui::GetIO().Framerate);
        ImGui::Text("%.1d Vertices Displayed)", mesh.GetVerticesNumber());
        ImGui::Checkbox("Debug Mode", OpenGLEngine::DebugMode()); 
                
        OpenGLEngine::ImguiDraw();

        OpenGLEngine::SwapBuffers();
        OpenGLEngine::PollEvents();
    }


    OpenGLEngine::ImguiQuit();

    OpenGLEngine::Quit(); 
    
    return 0;
}

