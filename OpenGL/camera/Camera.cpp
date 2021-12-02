//
//  Camera.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 05/11/21.
//

#include "Camera.h"


Camera::Camera(int x, int y, int z){
    position = glm::vec3(x, y, z);
    up = glm::normalize(position); //glm::vec3(0.0f, 1.0f,  0.0f);
    front = glm::vec3(1.0f, 0.0f, 0.0f);
    proj = glm::perspective(45.0f, (float)OpenGLEngine::WINDOW_WIDTH / OpenGLEngine::WINDOW_HEIGHT, 0.1f, 10000.0f);
    
    recalculateCameraView();
    recalculateCameraDirection();
    
    cameraFrustum = Frustum(proj * view);
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
    
    recalculateSpeedFromHeight();
    
    glm::vec3 oldPosition = position;
    
    if (OpenGLEngine::KeyIsPressed(GLFW_KEY_W)){
        position += movementSpeed * speedFromHeightModifier * front * Time::DeltaTime();
    }
    if (OpenGLEngine::KeyIsPressed(GLFW_KEY_S)){
        position -= movementSpeed * speedFromHeightModifier * front * Time::DeltaTime();
    }
    if (OpenGLEngine::KeyIsPressed(GLFW_KEY_A)){
        position -= movementSpeed * speedFromHeightModifier * glm::normalize(glm::cross(front, up)) * Time::DeltaTime();
    }
    if (OpenGLEngine::KeyIsPressed(GLFW_KEY_D)){
        position += movementSpeed * speedFromHeightModifier * glm::normalize(glm::cross(front, up)) * Time::DeltaTime();
    }
    if (OpenGLEngine::KeyIsPressed(GLFW_KEY_SPACE)){
        position += movementSpeed * speedFromHeightModifier * glm::normalize(up) * Time::DeltaTime();
    }
    if (OpenGLEngine::KeyIsPressed(GLFW_KEY_LEFT_SHIFT)){
        position += movementSpeed * speedFromHeightModifier * glm::normalize(-up) * Time::DeltaTime();
    }
        
    hasMoved = oldPosition != position;
    oldPosition = position;
        
    if (directionHasChanged || hasMoved){
        recalculateCameraView();
    }
    
    cameraFrustum = Frustum(proj * view);
}

void Camera::recalculateCameraDirection(){
    up = glm::normalize(position);
    
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
}

void Camera::recalculateCameraView(){
    view = glm::lookAt(position, position + front, up);
}

bool Camera::PointIsVisibleFromCamera(const glm::vec3& minPoint, const glm::vec3& maxPoint) const {
    return cameraFrustum.IsBoxVisible(minPoint, maxPoint); 
}

void Camera::recalculateSpeedFromHeight(){
    float maxDist = QuadtreeSettings::InitialWidth * 2;
    float dist = glm::distance(position, glm::vec3(0,0,0)) - QuadtreeSettings::InitialWidth;
    if (dist > maxDist) dist = maxDist;
    if (dist < 80) dist = 80;
    speedFromHeightModifier = dist * 25 / maxDist;
}
