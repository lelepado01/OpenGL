
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "OpenGLEngine.h"
#include "Texture.h"
#include "camera/Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"

int main( void ) {
    OpenGLEngine::Init();

    float positions[] = {
        1.f, 1.f, 1.f,
        -1.f, 1.f, 1.f,
        -1.f, 1.f, -1.f,
        1.f, 1.f, -1.f,
        1.f, -1.f, 1.f,
        -1.f, -1.f, 1.f,
        -1.f, -1.f, -1.f,
        1.f, -1.f, -1.f
    };

    unsigned int indices[] = {
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
    
    VertexArray va;
    VertexBuffer vb(positions, 3 * 8 * sizeof(float));
    IndexBuffer ib(indices, 3 * 12);
        
    VertexBufferLayout layout;
    layout.AddFloat(3);

    va.AddBuffer(vb, layout);

    Shader shader("/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/Vertex.shader",
                  "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/Fragment.shader");
    shader.Bind();

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
        OpenGLEngine::Draw(va, ib, shader);
        
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

