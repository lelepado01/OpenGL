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
}

void Camera::recalculateCameraDirection(){
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    recalculateVisibilityAngle();
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
    
    return deltaAngle <= visibilityAngle;
}

void Camera::recalculateVisibilityAngle(){
    visibilityAngle = (1 - (direction.y + 1) / 2) * minVisibilityAngle + minVisibilityAngle;
}

void Camera::recalculateSpeedFromHeight(){
    float maxDist = 2000;
    float dist = glm::distance(position, glm::vec3(0,0,0));
    if (dist > maxDist) dist = maxDist;
    speedFromHeightModifier = dist / maxDist;
}
