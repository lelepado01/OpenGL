
#define TINYOBJLOADER_IMPLEMENTATION

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
#include "terrain/PlanetaryMesh.h"
#include "terrain/utils/LODHandler.h"
#include "forest/ForestHandler.h"
#include "grass/GrassHandler.h"
#include "buffers/UniformBuffer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"

int main( void ) {
    OpenGLEngine::Init();
    ActiveShaders::Init();
    MeshHeightHandler::Init();
    LODHandler::Init();
    
    ForestHandler::Init();
//    GrassHandler::Init();
    
    Camera camera(0, 5000, 0);
    
    PlanetaryMesh terrain;
    
    Material material = {};
    material.color = glm::vec3(1,1,1);
    material.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    material.diffuse = glm::vec3(0.8, 0.8f, 0.8f);
    material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    material.shininess = 0.2f;

    Light light = {};
    light.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    light.ambient = glm::vec3(0.8f, 0.8f, 0.8f);
    
    UniformBuffer uniformBuffer(sizeof(glm::mat4x4));
    uniformBuffer.BindUniformBlock(*ActiveShaders::TerrainShader, "Matrices", 0);
    uniformBuffer.BindUniformBlock(*ActiveShaders::TreeModelShader, "Matrices", 0);
//    uniformBuffer.BindUniformBlock(*ActiveShaders::GrassModelShader, "Matrices", 0);

    OpenGLEngine::ImguiInit();

    while( OpenGLEngine::IsRunning() ){
        OpenGLEngine::Clear();
        OpenGLEngine::UpdateTime();
        Time::Count(); 

        OpenGLEngine::ImguiNewFrame();
        
        camera.UpdateDirection();
        camera.UpdatePosition();
    
        glm::mat4 mvp = camera.GetMVP();
        uniformBuffer.SetUniformBlockMat4x4f("u_MVP", mvp);

        ActiveShaders::TerrainShader->Bind();
        ActiveShaders::TerrainShader->SetUniform1f("u_Time", Time::GetFrameCount() / 100.0f);
//        ActiveShaders::TerrainShader->SetUniformMat4f("u_MVP", mvp);
        ActiveShaders::TerrainShader->SetUniformMaterial("u_Material", material);
        ActiveShaders::TerrainShader->SetUniform3f("u_cameraPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        ActiveShaders::TerrainShader->SetUniformLight("u_Light", light);
                    
        terrain.Update(camera);
        terrain.Render(camera);
        
//        ActiveShaders::TreeModelShader->Bind();
//        ActiveShaders::TreeModelShader->SetUniformMat4f("u_MVP", mvp);

        ForestHandler::Update(camera);
        ForestHandler::Render();

//        ActiveShaders::GrassModelShader->Bind();
//        ActiveShaders::GrassModelShader->SetUniformMat4f("u_MVP", mvp);
//        GrassHandler::Update(camera);
//        GrassHandler::Render();
        
        ImGui::Text("%.1f FPS)", ImGui::GetIO().Framerate);
        ImGui::Text("%.1ld Vertices Displayed)", terrain.GetVertexNumber(camera));
        ImGui::Checkbox("Debug Mode", OpenGLEngine::DebugMode());

        OpenGLEngine::ImguiDraw();

        OpenGLEngine::SwapBuffers();
        OpenGLEngine::PollEvents();
    }


    OpenGLEngine::ImguiQuit();

    OpenGLEngine::Quit(); 
    
    return 0;
}

