//
//  LODHandler.h
//  OpenGL
//
//  Created by Gabriele Padovani on 04/12/21.
//

#ifndef LODHandler_h
#define LODHandler_h

#include <stdio.h>
#include <vector>

#include "../settings/QuadtreeSettings.h"

class LODHandler {
private:
    static std::vector<float> distances;
    static std::vector<int> levelOfDetails;
    
public:
    static void Init();
    static int GetLevelOfDetail(float distance);
};

#endif /* LODHandler_h */

