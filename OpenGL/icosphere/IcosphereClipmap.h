//
//  IcosphereClipmap.h
//  VulkanEngine
//
//  Created by Gabriele Padovani on 01/02/22.
//

#ifndef IcosphereClipmap_h
#define IcosphereClipmap_h

#include <stdio.h>
#include <vector>

#include "IcospherePatch.h"
#include "../camera/Camera.h"

class IcosphereClipmap {
private:
    
    std::vector<IcospherePatch> terrainPatches;
    
public:
    IcosphereClipmap();
    
    void Render(const Camera& camera); 
    
};


#endif /* IcosphereClipmap_hpp */
