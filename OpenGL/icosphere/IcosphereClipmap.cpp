//
//  IcosphereClipmap.cpp
//  VulkanEngine
//
//  Created by Gabriele Padovani on 01/02/22.
//

#include "IcosphereClipmap.h"

IcosphereClipmap::IcosphereClipmap(){
        
    float t = (1.0 + sqrt(5.0)) / 2.0;
    
    std::vector<glm::vec3> initialPoints{};

    initialPoints.push_back(glm::vec3(-1,  t,  0));
    initialPoints.push_back(glm::vec3( 1,  t,  0));
    initialPoints.push_back(glm::vec3(-1, -t,  0));
    initialPoints.push_back(glm::vec3( 1, -t,  0));
    initialPoints.push_back(glm::vec3( 0, -1,  t));
    initialPoints.push_back(glm::vec3( 0,  1,  t));
    initialPoints.push_back(glm::vec3( 0, -1, -t));
    initialPoints.push_back(glm::vec3( 0,  1, -t));
    initialPoints.push_back(glm::vec3( t,  0, -1));
    initialPoints.push_back(glm::vec3( t,  0,  1));
    initialPoints.push_back(glm::vec3(-t,  0, -1));
    initialPoints.push_back(glm::vec3(-t,  0,  1));
    
//    terrainPatches.push_back(IcospherePatch(engine, initialPoints[0], initialPoints[11], initialPoints[5],  5));
//    terrainPatches.push_back(IcospherePatch(engine, initialPoints[0], initialPoints[5],  initialPoints[1],  5));
//    terrainPatches.push_back(IcospherePatch(engine, initialPoints[0], initialPoints[1],  initialPoints[7],  5));
//    terrainPatches.push_back(IcospherePatch(engine, initialPoints[0], initialPoints[7],  initialPoints[10], 5));
//    terrainPatches.push_back(IcospherePatch(engine, initialPoints[0], initialPoints[10], initialPoints[11], 5));

    terrainPatches.push_back(IcospherePatch(initialPoints[1], initialPoints[5], initialPoints[9],    4));
    terrainPatches.push_back(IcospherePatch(initialPoints[5], initialPoints[11],  initialPoints[4],  4));
    terrainPatches.push_back(IcospherePatch(initialPoints[11], initialPoints[10],  initialPoints[2], 4));
    terrainPatches.push_back(IcospherePatch(initialPoints[10], initialPoints[7],  initialPoints[6],  4));
    terrainPatches.push_back(IcospherePatch(initialPoints[7], initialPoints[1], initialPoints[8],    4));

    terrainPatches.push_back(IcospherePatch(initialPoints[3], initialPoints[9], initialPoints[4],  7));
    terrainPatches.push_back(IcospherePatch(initialPoints[3], initialPoints[4],  initialPoints[2], 7));
    terrainPatches.push_back(IcospherePatch(initialPoints[3], initialPoints[2],  initialPoints[6], 7));
    terrainPatches.push_back(IcospherePatch(initialPoints[3], initialPoints[6],  initialPoints[8], 7));
    terrainPatches.push_back(IcospherePatch(initialPoints[3], initialPoints[8], initialPoints[9],  7));
    
    terrainPatches.push_back(IcospherePatch(initialPoints[4], initialPoints[9], initialPoints[5],   4));
    terrainPatches.push_back(IcospherePatch(initialPoints[2], initialPoints[4],  initialPoints[11], 4));
    terrainPatches.push_back(IcospherePatch(initialPoints[6], initialPoints[2],  initialPoints[10], 4));
    terrainPatches.push_back(IcospherePatch(initialPoints[8], initialPoints[6],  initialPoints[7],  4));
    terrainPatches.push_back(IcospherePatch(initialPoints[9], initialPoints[8], initialPoints[1],   4));

}


void IcosphereClipmap::Render(const Camera& camera){
    for (IcospherePatch patch : terrainPatches) {
        patch.Render();
    }
}
