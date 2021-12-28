//
//  GrassHandler.hpp
//  OpenGL
//
//  Created by Gabriele Padovani on 16/12/21.
//

#ifndef GrassHandler_h
#define GrassHandler_h

#include <stdio.h>
#include <random>

#include "../camera/Camera.h"
#include "../shaders/ActiveShaders.h"
#include "../mesh/ModelMesh.h"
#include "../mesh/ModelLoader.h"
#include "../terrain/TerrainPatch.h"
#include "../terrain/utils/MeshHeightHandler.h"
#include "../settings/PlanetSettings.h"

class GrassHandler {
private:
    static std::vector<glm::vec3> grassPositions;
    
    static ModelMesh* model;
    inline static std::default_random_engine generator;
    
    static int areaDiameter;
    static float areaRadius;
    static int instantiatedModelNumber;

public:
    static void Init();
    static void Update(Camera camera);
    static void Render();
};

#endif /* GrassHandler_h */
