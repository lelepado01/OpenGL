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

enum TerrainPatchAnimation {
    Upscale, Downscale
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

class TerrainPopHandler {
public:
    bool isBeingAnimated;
    float incrementalTimeHeightMultiplier;
    long timeOfBuildCall;
    const int transitionTimeInMilliseconds = 1000;
    
public:
    bool TerrainIsBeingAnimated(){
        return isBeingAnimated; 
    }
    
    void Update(){
        long timeNow = Time::GetMillisecondsFromEpoch();
        if (timeNow - timeOfBuildCall >= transitionTimeInMilliseconds){
            isBeingAnimated = false;
            return;
        }
        
        incrementalTimeHeightMultiplier = ((float)(timeNow - timeOfBuildCall)) / transitionTimeInMilliseconds;
    }
    
    float GetAnimationPercentage(){
        if (isBeingAnimated){
            return incrementalTimeHeightMultiplier;
        } else {
            return -1.0f;
        }
    };
};




#endif /* TerrainFaceUtils_h */
