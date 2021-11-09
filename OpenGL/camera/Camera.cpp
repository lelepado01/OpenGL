//
//  Camera.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 05/11/21.
//

#include "Camera.h"


Camera::Camera(int x, int y, int z){
    position = glm::vec3(x, y, z);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f,  0.0f);
    proj = glm::perspective(45.0f, (float)OpenGLEngine::WINDOW_WIDTH / OpenGLEngine::WINDOW_HEIGHT, 0.1f, 1000.0f);
    
    recalculateCameraView();
    recalculateCameraDirection();
}

Camera::~Camera(){
    
}


void Camera::UpdateDirection(){
    directionHasChanged = OpenGLEngine::GetMouseOffsetX() != 0 || OpenGLEngine::GetMouseOffsetY() != 0;
    
    yaw   += OpenGLEngine::GetMouseOffsetX();
    pitch += OpenGLEngine::GetMouseOffsetY();
    
    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;
    
    recalculateCameraDirection();
    front = glm::normalize(direction);
}


void Camera::UpdatePosition(){
    
    glm::vec3 oldPosition = position;
    
    if (OpenGLEngine::KeyIsPressed(GLFW_KEY_W)){
        position += movementSpeed * front;
    }
    if (OpenGLEngine::KeyIsPressed(GLFW_KEY_S)){
        position -= movementSpeed * front;
    }
    if (OpenGLEngine::KeyIsPressed(GLFW_KEY_A)){
        position -= movementSpeed * glm::normalize(glm::cross(front, up));
    }
    if (OpenGLEngine::KeyIsPressed(GLFW_KEY_D)){
        position += movementSpeed * glm::normalize(glm::cross(front, up));
    }
    if (OpenGLEngine::KeyIsPressed(GLFW_KEY_SPACE)){
        position += movementSpeed * glm::normalize(up);
    }
    if (OpenGLEngine::KeyIsPressed(GLFW_KEY_LEFT_SHIFT)){
        position += movementSpeed * glm::normalize(-up);
    }
    
    if (directionHasChanged || oldPosition != position){
        recalculateCameraView();
    }
}

void Camera::recalculateCameraDirection(){
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
}

void Camera::recalculateCameraView(){
    view = glm::lookAt(position, position + front, up);
}
