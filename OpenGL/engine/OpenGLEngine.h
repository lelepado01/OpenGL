#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Debug.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

class OpenGLEngine {
private:
    static GLFWwindow* window;
    
    static bool debugMode;
    static bool isFirstFrame;
    static bool mousePositionUpdatedThisFrame; 
    
    static float deltaTime;
    static float lastFrame;
    
    static float lastFrameMousePositionX;
    static float lastFrameMousePositionY;
    static float mouseOffsetX;
    static float mouseOffsetY;
    static float mouseSensitivity;
    
public:
    static const int WINDOW_WIDTH = 1560;
    static const int WINDOW_HEIGHT = 960;

    static void Init();
    static void Clear();
    static void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader);
    static void DrawInstanced(const VertexArray &va, const IndexBuffer &ib, const Shader &shader, const unsigned int amount); 
    static void SwapBuffers();
    static void PollEvents();
    static void UpdateTime(); 
    static void Quit(); 
    
    static bool IsRunning();
    static bool KeyIsPressed(int key);
    static bool* DebugMode() { return &debugMode; };

    static float DeltaTime();
    
    static void ImguiNewFrame();
    static void ImguiInit();
    static void ImguiQuit();
    static void ImguiDraw();
    
    static float GetMouseOffsetX() { return mouseOffsetX; };
    static float GetMouseOffsetY() { return mouseOffsetY; };

private:
    static void initWindow();
    static void mouseHandler(GLFWwindow* window, double xpos, double ypos);
    
};
