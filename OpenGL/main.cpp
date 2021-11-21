
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "engine/OpenGLEngine.h"
#include "engine/Time.h"
#include "camera/Camera.h"
#include "materials/Material.h"
#include "shaders/Shader.h"
#include "shaders/ActiveShaders.h"
#include "mesh/Mesh.h"
#include "mesh/meshBuilder/CloseMeshBuilder.h"
#include "mesh/meshBuilder/TerrainQuadtree.h"
#include "settings/MeshSettings.h"
#include "settings/ChunkSettings.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"


int main( void ) {
    OpenGLEngine::Init();

    Camera camera(0, 0, 0);
    
    MeshHeight::Init(); 
//    CloseMeshBuilder closeMeshBuilder = CloseMeshBuilder(camera);
//    Mesh closeMesh(closeMeshBuilder.GetVertices(), closeMeshBuilder.GetIndices());

    TerrainQuadtree terrain(0, 0, 512, 1);
    
    ActiveShaders::Init(); 
    
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
        ActiveShaders::TerrainShader->SetUniformMat4f("u_MVP", mvp);
        ActiveShaders::TerrainShader->SetUniformMaterial("u_Material", material);
        ActiveShaders::TerrainShader->SetUniform3f("u_cameraPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        ActiveShaders::TerrainShader->SetUniformLight("u_Light", light);
                    
//        if (camera.HasMoved() || camera.HasRotated()){
//            closeMeshBuilder.UpdateMesh(camera);
//            closeMesh.UpdateMesh(closeMeshBuilder.GetVertices(), closeMeshBuilder.GetIndices());
//        }
//
//        OpenGLEngine::Draw(closeMesh.GetVertexArray(), closeMesh.GetIndexBuffer(), *ActiveShaders::TerrainShader);
        
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
//        ImGui::Text("%.1d Vertices Displayed)", closeMesh.GetVerticesNumber());
        ImGui::Checkbox("Debug Mode", OpenGLEngine::DebugMode());
                
        OpenGLEngine::ImguiDraw();

        OpenGLEngine::SwapBuffers();
        OpenGLEngine::PollEvents();
    }


    OpenGLEngine::ImguiQuit();

    OpenGLEngine::Quit(); 
    
    return 0;
}

