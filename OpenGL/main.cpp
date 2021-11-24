
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "engine/OpenGLEngine.h"
#include "engine/Time.h"
#include "camera/Camera.h"
#include "materials/Material.h"
#include "shaders/Shader.h"
#include "shaders/ActiveShaders.h"
#include "mesh/meshBuilder/PlanetaryMesh.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"


int main( void ) {
    OpenGLEngine::Init();
    
    ActiveShaders::Init();
    MeshHeight::Init();

    Camera camera(0, 500, 0);
    
    PlanetaryMesh terrain;
    
    Material material = {};
    material.color = glm::vec3(1,1,1);
    material.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    material.diffuse = glm::vec3(0.8, 0.8f, 0.8f);
    material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    material.shininess = 0.2f;

    Light light = {};
    light.direction = glm::vec3(0.8f, -1.0f, 0.8f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    light.ambient = glm::vec3(0.8f, 0.8f, 0.8f);

    OpenGLEngine::ImguiInit();

    float time = 0;
    while( OpenGLEngine::IsRunning() ){
        OpenGLEngine::Clear();
        OpenGLEngine::UpdateTime();
        Time::Count(); 

        OpenGLEngine::ImguiNewFrame();
        
        camera.UpdateDirection();
        camera.UpdatePosition();
        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = (camera.GetProjection()) * (camera.GetView()) * model;
        
        ActiveShaders::TerrainShader->Bind();
        time += 0.0001;
        if (time > 60) time = 0; 
        ActiveShaders::TerrainShader->SetUniform1f("u_Time", time);
        ActiveShaders::TerrainShader->SetUniformMat4f("u_MVP", mvp);
        ActiveShaders::TerrainShader->SetUniformMaterial("u_Material", material);
        ActiveShaders::TerrainShader->SetUniform3f("u_cameraPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        ActiveShaders::TerrainShader->SetUniformLight("u_Light", light);
                    
        terrain.Update(camera);
        terrain.Render(camera);
        
        ImGui::SliderFloat3("Light Angle", &light.direction.x, -0.5, 0.5);

        ImGui::SliderFloat("Large Freq", &MeshHeight::LargeFrequency, 0, 1);
        ImGui::SliderFloat("Small Freq", &MeshHeight::SmallFrequency, 0, 1);
        ImGui::SliderFloat("Small Scale", &MeshHeight::SmallScale, 0, 1);
        ImGui::SliderFloat("Large Scale", &MeshHeight::LargeScale, 0, 1);
        ImGui::SliderFloat("Large Mult", &MeshHeight::LargeMultiplier, 1, 5);
        
        ImGui::Separator();
        
        ImGui::Text("%.1f FPS)", ImGui::GetIO().Framerate);
        ImGui::Text("%.1d Vertices Displayed)", terrain.GetVertexNumber());
        ImGui::Checkbox("Debug Mode", OpenGLEngine::DebugMode());
                
        OpenGLEngine::ImguiDraw();

        OpenGLEngine::SwapBuffers();
        OpenGLEngine::PollEvents();
    }


    OpenGLEngine::ImguiQuit();

    OpenGLEngine::Quit(); 
    
    return 0;
}

