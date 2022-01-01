//
//  GeomorphHandler.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 01/01/22.
//

#include "GeomorphHandler.h"


GeomorphHandler::GeomorphHandler(){
    Reset(); 
}

void GeomorphHandler::Reset(){
    isBeingAnimated = true;
    timeOfBuildCall = Time::GetMillisecondsFromEpoch();
    incrementalTimeHeightMultiplier = 0.0f;
}

void GeomorphHandler::Update(){
    long timeNow = Time::GetMillisecondsFromEpoch();
    if (timeNow - timeOfBuildCall >= transitionTimeInMilliseconds){
        isBeingAnimated = false;
    } else {
        incrementalTimeHeightMultiplier = ((float)(timeNow - timeOfBuildCall)) / transitionTimeInMilliseconds;
//            std::cout << incrementalTimeHeightMultiplier << "\n";

    }
}

float GeomorphHandler::GetAnimationPercentage(){
    if (isBeingAnimated){
        return incrementalTimeHeightMultiplier;
    } else {
        return 1.0f;
    }
};
