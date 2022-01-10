
#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>

#include "engine/OpenGLEngine.h"
#include "engine/Time.h"
#include "camera/Camera.h"
#include "materials/Material.h"
#include "shaders/ActiveShaders.h"
#include "terrain/PlanetaryMesh.h"
#include "terrain/utils/LODHandler.h"
#include "forest/ForestHandler.h"
#include "grass/GrassHandler.h"
#include "buffers/UniformBuffer.h"
#include "icosphere/IcosphereTerrain.h"

int main( void ) {
    
    OpenGLEngine::Init();
    ActiveShaders::Init();
    MeshHeightHandler::Init();
    LODHandler::Init();
    
//    ForestHandler forest;
//    GrassHandler::Init();
    
    Camera camera(0, 100, 0);
    
//    PlanetaryMesh terrain;
    IcosphereTerrain terrain;
//    Planet terrain;
    
    Material material = {};
    material.color = glm::vec3(1,1,1);
    material.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
    material.diffuse = glm::vec3(0.8, 0.8f, 0.8f);
    material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    material.shininess = 0.2f;

    Light light = {};
    light.direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
    light.specular = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    light.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 0.0f);
    light.ambient = glm::vec4(0.8f, 0.8f, 0.8f, 0.0f);
    
    UniformBuffer uniformBuffer;
    unsigned int bufferSize = sizeof(glm::mat4x4) * 3 + sizeof(Light) + sizeof(float);
    uniformBuffer.BindUniformBlock(*ActiveShaders::TerrainShader, "SharedResources", 0, bufferSize);
    uniformBuffer.BindUniformBlock(*ActiveShaders::TreeModelShader, "SharedResources", 0, bufferSize);
    uniformBuffer.BindUniformBlock(*ActiveShaders::SimpleShader, "SharedResources", 0, bufferSize);

    OpenGLEngine::ImguiInit();

    while( OpenGLEngine::IsRunning() ){
        OpenGLEngine::Clear();
        OpenGLEngine::UpdateTime();
        Time::Count(); 

        OpenGLEngine::ImguiNewFrame();
        
        camera.UpdateDirection();
        camera.UpdatePosition();
    
        glm::mat4 mvp = camera.GetMVP();
        glm::mat4 view = camera.GetView();
        glm::mat4 projection = camera.GetProjection();
        
        uniformBuffer.SetUniformBlockMat4x4f("u_MVP", mvp);
        uniformBuffer.SetUniformBlockMat4x4f("u_View", view);
        uniformBuffer.SetUniformBlockMat4x4f("u_Projection", projection);
        uniformBuffer.SetUniformBlockLight("u_Light", light);
        uniformBuffer.SetUniformBlock1f("u_Time", Time::GetFrameCount() / 100.0f);

        ActiveShaders::TerrainShader->Bind();
        ActiveShaders::TerrainShader->SetUniformMaterial("u_Material", material);
        ActiveShaders::TerrainShader->SetUniform3f("u_cameraPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        
//        terrain.Update();
//        terrain.Draw();
        
        terrain.Update(camera);
        terrain.Render(camera);

//        forest.Update(camera);
//        forest.Render();
        
//        GrassHandler::Update(camera);
//        GrassHandler::Render();
        
        ImGui::Text("%.1f FPS)", ImGui::GetIO().Framerate);
//        ImGui::Text("%.1ld Vertices Displayed)", terrain.GetVertexNumber(camera));
        ImGui::Checkbox("Debug Mode", OpenGLEngine::DebugMode());

        OpenGLEngine::ImguiDraw();

        OpenGLEngine::SwapBuffers();
        OpenGLEngine::PollEvents();
    }


    OpenGLEngine::ImguiQuit();

    OpenGLEngine::Quit(); 
    
    return 0;
}

