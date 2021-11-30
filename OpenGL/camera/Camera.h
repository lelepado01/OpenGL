//
//  Camera.hpp
//  OpenGL
//
//  Created by Gabriele Padovani on 05/11/21.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>

#include "../engine/OpenGLEngine.h"
#include "../engine/Time.h"
#include "Frustum.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Camera {
private:
    const float movementSpeed = 100.f;
    float speedFromHeightModifier = 1; 
    
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
    
    Frustum cameraFrustum; 
    
private:
    void recalculateCameraDirection();
    void recalculateCameraView();
    void recalculateSpeedFromHeight();
    
public:
    Camera(int x, int y, int z);

    void UpdatePosition();
    void UpdateDirection();
    
    glm::mat4 GetProjection() const { return proj; };
    glm::mat4 GetView() const { return view; };
    glm::vec3 GetPosition() const { return position; };
    glm::vec3 GetDirection() const { return direction; };

    bool HasMoved() const { return hasMoved; };
    bool HasRotated() const { return directionHasChanged; };
    
    bool PointIsVisibleFromCamera(const glm::vec3& minPoint, const glm::vec3& maxPoint) const;
};

#endif /* Camera_hpp */
