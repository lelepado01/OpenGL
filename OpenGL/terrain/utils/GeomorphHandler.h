//
//  GeomorphHandler.h
//  OpenGL
//
//  Created by Gabriele Padovani on 01/01/22.
//

#ifndef GeomorphHandler_h
#define GeomorphHandler_h

#include <stdio.h>
#include "../../engine/Time.h"

class GeomorphHandler {
private:
    bool isBeingAnimated;
    bool isDelayed;  
    
    float incrementalTimeHeightMultiplier;
    long timeOfBuildCall;
    
    const long transitionTimeInMilliseconds = 1000;
    const long timeDelay = 2000; 
    
public:
    GeomorphHandler();
    
    bool TerrainIsBeingAnimated() { return isBeingAnimated; };
    
    void Reset();
    void Update();
    
    float GetAnimationPercentage();
};


#endif /* GeomorphHandler_h */
