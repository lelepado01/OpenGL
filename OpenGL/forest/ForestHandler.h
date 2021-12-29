//
//  ForestHandler.h
//  OpenGL
//
//  Created by Gabriele Padovani on 15/12/21.
//

#ifndef ForestHandler_h
#define ForestHandler_h

#include <stdio.h>
#include <vector>
#include <random>

#include "../camera/Camera.h"
#include "../shaders/ActiveShaders.h"
#include "../mesh/ModelMesh.h"
#include "../mesh/ModelLoader.h"
#include "../terrain/TerrainPatch.h"
#include "../terrain/utils/MeshHeightHandler.h"
#include "../settings/PlanetSettings.h"

struct TreeAttribute {
    glm::vec3 position;
    float size;
    float rotation;
}; 

class ForestHandler {
private:
    std::vector<TreeAttribute> treeAttributes = std::vector<TreeAttribute>();
    std::vector<glm::mat4> treeModelMatrices = std::vector<glm::mat4>();

    ModelMesh* modelTree;
    VertexBuffer* treeAttributesBuffer; 
    
    int areaRadius;
    int treeNumber;
    
    std::default_random_engine generator;

public:
    ForestHandler();
    ~ForestHandler();
    
    void Update(const Camera& camera);
    void Render();
};

#endif /* ForestHandler_h */
