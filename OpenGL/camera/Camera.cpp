//
//  Camera.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 05/11/21.
//

#include "Camera.h"


Camera::Camera(int x, int y, int z){
    position = glm::vec3(x, y, z);
    front = glm::vec3(0.0f, 0.0f, 0.0f);
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
    
    hasMoved = oldPosition != position;
    
    if (directionHasChanged || hasMoved){
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

bool Camera::PointIsVisibleFromCamera(int pointX, int pointY){
    glm::vec2 point2d = glm::normalize(glm::vec2(pointX - position.x, pointY - position.z));
    float pointAngle = atan2(point2d.y, point2d.x);
    pointAngle *= 180 / M_PI;
    pointAngle = (360 + (int)round(pointAngle)) % 360;
    
    glm::vec2 dir2d = glm::normalize(glm::vec2(direction.x, direction.z));
    float cameraAngle = atan2(dir2d.y, dir2d.x);
    cameraAngle *= 180 / M_PI;
    cameraAngle = (360 + (int)round(cameraAngle)) % 360;
    
    float deltaAngle = abs(cameraAngle - pointAngle);
    if (deltaAngle > 180) {
        deltaAngle = 360 - deltaAngle;
    }
    
    return deltaAngle <= 50;
}
