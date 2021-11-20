#include "OpenGLEngine.h"

GLFWwindow* OpenGLEngine::window = nullptr;

bool OpenGLEngine::debugMode = false;
bool OpenGLEngine::isFirstFrame = true;
bool OpenGLEngine::mousePositionUpdatedThisFrame = false;

float OpenGLEngine::deltaTime = 0.0f;
float OpenGLEngine::lastFrame = 0.0f;

float OpenGLEngine::lastFrameMousePositionX = 0;
float OpenGLEngine::lastFrameMousePositionY = 0;
float OpenGLEngine::mouseOffsetX = 0;
float OpenGLEngine::mouseOffsetY = 0;
float OpenGLEngine::mouseSensitivity = 0.1f;

void OpenGLEngine::Init(){
    initWindow();
    
    GLCall( glEnable(GL_BLEND) );
    GLCall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
    GLCall( glEnable(GL_DEPTH_TEST) );
    GLCall( glDepthFunc(GL_LESS) );
    GLCall( glEnable(GL_CULL_FACE) );
    
    GLCall( glfwSetCursorPosCallback(window, mouseHandler) );
}

void OpenGLEngine::initWindow() {

    if( !glfwInit() ) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        exit(1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        exit(1);

    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        exit(1);
    }

    std::cout << "Using GL Version: " << glGetString(GL_VERSION) << std::endl;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

void OpenGLEngine::mouseHandler(GLFWwindow* window, double xpos, double ypos){
    mousePositionUpdatedThisFrame = true;
    
    if (isFirstFrame) {
        lastFrameMousePositionX = xpos;
        lastFrameMousePositionY = ypos;
        isFirstFrame = false;
    }
    
    mouseOffsetX = xpos - lastFrameMousePositionX;
    mouseOffsetY = lastFrameMousePositionY - ypos;
        
    lastFrameMousePositionX = xpos;
    lastFrameMousePositionY = ypos;
    
    mouseOffsetX *= mouseSensitivity;
    mouseOffsetY *= mouseSensitivity;
}


void OpenGLEngine::Clear() {
    GLCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
}

void OpenGLEngine::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) {
    shader.Bind();
    va.Bind();
    ib.Bind();
    
    if (debugMode){
        GLCall( glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ));
    } else {
        GLCall( glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ));
    }
    
    GLCall( glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr) );
}

void OpenGLEngine::SwapBuffers(){
    GLCall( glfwSwapBuffers(window) );
}

bool OpenGLEngine::IsRunning(){
    return glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0;
}

bool OpenGLEngine::KeyIsPressed(int key){
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void OpenGLEngine::PollEvents(){
    GLCall( glfwPollEvents() );
    
    if (!mousePositionUpdatedThisFrame){
        mouseOffsetX = 0;
        mouseOffsetY = 0; 
    }
    
    mousePositionUpdatedThisFrame = false;
    
}

void OpenGLEngine::Quit(){
    GLCall( glfwTerminate() );
}

void OpenGLEngine::UpdateTime(){
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

float OpenGLEngine::DeltaTime(){
    return deltaTime;
}


/*
 ImGui Section
 */
void OpenGLEngine::ImguiInit(){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();
}

void OpenGLEngine::ImguiQuit(){
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
}

void OpenGLEngine::ImguiNewFrame(){
    ImGui_ImplGlfwGL3_NewFrame();
}

void OpenGLEngine::ImguiDraw(){
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

