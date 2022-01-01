//
//  TerrainFaceUtils.h
//  OpenGL
//
//  Created by Gabriele Padovani on 23/11/21.
//

#ifndef TerrainFaceUtils_h
#define TerrainFaceUtils_h

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../../engine/Time.h"

enum TerrainFaceDirection {
    Top, Bottom, Front, Back, Left, Right
};

class TerrainFace {
public:
    
    static bool IsBackFace(TerrainFaceDirection direction){
        return direction == TerrainFaceDirection::Back
            || direction == TerrainFaceDirection::Bottom
            || direction == TerrainFaceDirection::Right;
    }
    
    static glm::mat3x3 GetAxisRotationMatrix(TerrainFaceDirection direction){
        if (direction == TerrainFaceDirection::Bottom){
            return glm::mat3x3(1,0,0,0,-1,0,0,0,1);
        } else if (direction == TerrainFaceDirection::Front){
            return glm::mat3x3(1,0,0,0,0,-1,0,1,0);
        } else if (direction == TerrainFaceDirection::Back){
            return glm::mat3x3(1,0,0,0,0,1,0,1,0);
        } else if (direction == TerrainFaceDirection::Left){
            return glm::mat3x3(0,1,0,-1,0,0,0,0,1);
        } else if (direction == TerrainFaceDirection::Right){
            return glm::mat3x3(0,1,0,+1,0,0,0,0,1);
        }
        
        return glm::identity<glm::mat3x3>();
    }

};

#endif /* TerrainFaceUtils_h */
