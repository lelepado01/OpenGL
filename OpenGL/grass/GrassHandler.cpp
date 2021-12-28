//
//  GrassHandler.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 16/12/21.
//

#include "GrassHandler.h"


int GrassHandler::areaDiameter = 10.0f;
float GrassHandler::areaRadius = (float)areaDiameter / 2;
int GrassHandler::instantiatedModelNumber = (areaDiameter / 2)* (areaDiameter /2 )* (areaDiameter/2);

ModelMesh* GrassHandler::model = nullptr;
std::vector<glm::vec3> GrassHandler::grassPositions = std::vector<glm::vec3>();

void GrassHandler::Init(){
    srand((unsigned int)time(NULL));
    
    model = new ModelMesh();
    ModelLoader::Load("/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/models/grass.obj", *model);

    for (float x = -areaRadius; x < areaRadius; x++) {
        for (float y = -areaRadius; y < areaRadius; y++) {
            for (float z = -areaRadius; z < areaRadius; z++) {
                grassPositions.push_back(glm::vec3());
            }
        }
    }
}

void GrassHandler::Update(Camera camera){
    glm::vec3 cameraPos = camera.GetPosition();
    
    int i = 0;
    for (float x = -areaRadius; x < areaRadius; x++) {
        for (float y = -areaRadius; y < areaRadius; y++) {
            for (float z = -areaRadius; z < areaRadius; z++) {
                glm::vec3 cameraPointOnSurface = glm::normalize(cameraPos + glm::vec3(x, y, z)) * (float)PlanetSettings::PlanetRadius;

                glm::vec3 pointHeight = MeshHeightHandler::GetHeight(cameraPointOnSurface.x,
                                                                     cameraPointOnSurface.y,
                                                                     cameraPointOnSurface.z,
                                                                     5) * glm::normalize(cameraPointOnSurface);
                glm::vec3 newTreePosition = cameraPointOnSurface + pointHeight;
    
    
                if (glm::distance(newTreePosition, cameraPos) < areaDiameter && glm::distance(newTreePosition, glm::vec3()) > PlanetSettings::SeaLevel) {
                    grassPositions[i] = newTreePosition;
                } else {
                    grassPositions[i] = glm::vec3();
                }

                i++;
            }
        }
    }
}

void GrassHandler::Render(){
    ActiveShaders::GrassModelShader->SetUniform3fv("instancePositions", glm::value_ptr(grassPositions[0]), instantiatedModelNumber);
    model->Render(*ActiveShaders::GrassModelShader, instantiatedModelNumber);
}
