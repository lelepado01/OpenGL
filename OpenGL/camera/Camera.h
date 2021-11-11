//
//  Camera.hpp
//  OpenGL
//
//  Created by Gabriele Padovani on 05/11/21.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>

#include "OpenGLEngine.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
private:
    const float movementSpeed = 0.5f;
    
    float pitch = 0;
    float yaw = 0;
    float roll = 0;
    
    bool directionHasChanged = false;
    bool hasMoved = false;
    
    glm::vec3 lastFramePosition;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 direction;
    
    glm::mat4 proj;
    glm::mat4 view;
    
public:
    Camera(int x, int y, int z);
    ~Camera();
    
    void UpdatePosition();
    void UpdateDirection();
    
    glm::mat4* GetProjection() { return &proj; };
    glm::mat4* GetView() { return &view; };
    glm::vec3 GetPosition() { return position; };
    
    bool HasMoved() { return hasMoved; };
    
private:
    void recalculateCameraDirection();
    void recalculateCameraView(); 
};

#endif /* Camera_hpp */
