//
//  IcospherePatch.h
//  VulkanEngine
//
//  Created by Gabriele Padovani on 01/02/22.
//

#ifndef IcospherePatch_h
#define IcospherePatch_h

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <stdio.h>
#include <memory>
#include <unordered_map>

#include "../engine/OpenGLEngine.h"
#include "../mesh/Vertex.h"
#include "../mesh/ClipmapMesh.h"
#include "../shaders/ActiveShaders.h"

class IcospherePatch {
private:
    std::unordered_map<glm::vec3, unsigned int> indexVertexPair{}; 
    std::vector<ClipmapVertex> vertices{};
    std::vector<unsigned int> indices{};
    
    unsigned int vertexIndex = 0;
    int maxSubdivisionLevel;
    float sphereRadius = 1000;

    std::shared_ptr<ClipmapMesh> terrainPatchMesh;
    
private:
    void subdivideTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, int subdivisionLevel);
    void addVertexAndIndex(glm::vec3 v); 
    
public:
    IcospherePatch(glm::vec3 a, glm::vec3 b, glm::vec3 c, int maxsubs);
    
    void Render(); 
};

#endif /* IcospherePatch_hpp */
